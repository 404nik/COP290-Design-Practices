#include "health_server.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace
{
struct HttpResponse
{
    std::string raw;
    std::string status_line;
    std::vector<std::string> headers;
    std::string body;
};

void require(bool condition, const std::string& message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}

HttpResponse request(uint16_t port, const std::string& request_text)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    require(fd >= 0, std::string("socket failed: ") + std::strerror(errno));

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
    {
        std::string error = std::strerror(errno);
        ::close(fd);
        throw std::runtime_error("connect failed: " + error);
    }

    const char* data = request_text.c_str();
    size_t remaining = request_text.size();
    while (remaining > 0)
    {
        ssize_t sent = send(fd, data, remaining, MSG_NOSIGNAL);
        if (sent <= 0)
        {
            if (sent < 0 && errno == EINTR)
            {
                continue;
            }
            std::string error = std::strerror(errno);
            ::close(fd);
            throw std::runtime_error("send failed: " + error);
        }
        data += sent;
        remaining -= static_cast<size_t>(sent);
    }

    std::string raw;
    char buffer[256];
    while (true)
    {
        ssize_t n = recv(fd, buffer, sizeof(buffer), 0);
        if (n == 0)
        {
            break;
        }
        if (n < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            std::string error = std::strerror(errno);
            ::close(fd);
            throw std::runtime_error("recv failed: " + error);
        }
        raw.append(buffer, static_cast<size_t>(n));
    }

    char extra = '\0';
    ssize_t closed = recv(fd, &extra, 1, 0);
    require(closed == 0, "health server did not close the connection");
    ::close(fd);

    size_t split = raw.find("\r\n\r\n");
    require(split != std::string::npos, "response missing header/body separator");

    HttpResponse response;
    response.raw = raw;
    response.body = raw.substr(split + 4);

    std::istringstream header_stream(raw.substr(0, split));
    std::getline(header_stream, response.status_line);
    if (!response.status_line.empty() && response.status_line[response.status_line.size() - 1] == '\r')
    {
        response.status_line.erase(response.status_line.size() - 1);
    }

    std::string header;
    while (std::getline(header_stream, header))
    {
        if (!header.empty() && header[header.size() - 1] == '\r')
        {
            header.erase(header.size() - 1);
        }
        response.headers.push_back(header);
    }

    return response;
}

bool hasHeader(const HttpResponse& response, const std::string& expected)
{
    for (size_t i = 0; i < response.headers.size(); ++i)
    {
        if (response.headers[i] == expected)
        {
            return true;
        }
    }
    return false;
}

void expectResponse(const HttpResponse& response,
                    const std::string& status_line,
                    const std::string& body)
{
    require(response.status_line == status_line,
            "unexpected status line: " + response.status_line);
    require(hasHeader(response, "Content-Type: application/json"),
            "missing application/json content type");
    require(hasHeader(response, "Content-Length: " + std::to_string(body.size())),
            "missing exact content length for body " + body);
    require(hasHeader(response, "Connection: close"),
            "missing connection close header");
    require(response.body == body, "unexpected body: " + response.body);
}

void testLiveAndReadyContract()
{
    HealthServer server;
    std::string error;
    require(server.start(0, &error), "server failed to start: " + error);
    uint16_t port = server.port();
    require(port != 0, "ephemeral health port was not assigned");

    expectResponse(request(port, "GET /health/live HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"),
                   "HTTP/1.1 200 OK",
                   "{\"status\":\"ok\"}");

    expectResponse(request(port, "GET /health/ready HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"),
                   "HTTP/1.1 503 Service Unavailable",
                   "{\"status\":\"not_ready\"}");

    server.setReady(true);
    expectResponse(request(port, "GET /health/ready HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"),
                   "HTTP/1.1 200 OK",
                   "{\"status\":\"ready\"}");

    expectResponse(request(port, "GET /missing HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"),
                   "HTTP/1.1 404 Not Found",
                   "{\"status\":\"not_found\"}");

    expectResponse(request(port, "POST /health/live HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"),
                   "HTTP/1.1 405 Method Not Allowed",
                   "{\"status\":\"method_not_allowed\"}");

    server.stop();
    server.stop();
    require(!server.isRunning(), "server still marked running after stop");
    require(!server.isReady(), "server still marked ready after stop");
}

void testRestart()
{
    HealthServer server;
    std::string error;
    require(server.start(0, &error), "first server start failed: " + error);
    uint16_t first_port = server.port();
    expectResponse(request(first_port, "GET /health/live HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"),
                   "HTTP/1.1 200 OK",
                   "{\"status\":\"ok\"}");
    server.stop();

    require(server.start(0, &error), "second server start failed: " + error);
    uint16_t second_port = server.port();
    require(second_port != 0, "second ephemeral port was not assigned");
    expectResponse(request(second_port, "GET /health/live HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"),
                   "HTTP/1.1 200 OK",
                   "{\"status\":\"ok\"}");
    server.stop();
}
}

int main()
{
    try
    {
        testLiveAndReadyContract();
        testRestart();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "health_server_test failed: " << ex.what() << std::endl;
        return 1;
    }

    std::cout << "health_server_test passed" << std::endl;
    return 0;
}
