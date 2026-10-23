#include "health_server.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace
{
const int kBacklog = 16;
const size_t kMaxRequestLine = 4096;

std::string reasonPhrase(int status_code)
{
    switch (status_code)
    {
    case 200:
        return "OK";
    case 400:
        return "Bad Request";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 503:
        return "Service Unavailable";
    default:
        return "Internal Server Error";
    }
}

void sendAll(int fd, const std::string& response)
{
    const char* data = response.c_str();
    size_t remaining = response.size();
    while (remaining > 0)
    {
        ssize_t written = send(fd, data, remaining, MSG_NOSIGNAL);
        if (written <= 0)
        {
            if (written < 0 && errno == EINTR)
            {
                continue;
            }
            break;
        }
        data += written;
        remaining -= static_cast<size_t>(written);
    }
}

std::string buildResponse(int status_code, const std::string& body)
{
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " " << reasonPhrase(status_code) << "\r\n"
             << "Content-Type: application/json\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << body;
    return response.str();
}

bool readRequestLine(int fd, std::string& line)
{
    line.clear();
    while (line.size() < kMaxRequestLine)
    {
        char ch = '\0';
        ssize_t n = recv(fd, &ch, 1, 0);
        if (n == 0)
        {
            return false;
        }
        if (n < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            return false;
        }
        if (ch == '\n')
        {
            if (!line.empty() && line[line.size() - 1] == '\r')
            {
                line.erase(line.size() - 1);
                return true;
            }
            return false;
        }
        line.push_back(ch);
    }
    return false;
}
}

const uint16_t HealthServer::DEFAULT_PORT;

HealthServer::HealthServer()
    : running_(false), ready_(false), listen_fd_(-1), port_(0)
{
}

HealthServer::~HealthServer()
{
    stop();
}

bool HealthServer::start(uint16_t port, std::string* error)
{
    std::lock_guard<std::mutex> lock(lifecycle_mutex_);
    if (running_.load())
    {
        return true;
    }

    ready_.store(false);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        if (error)
        {
            *error = std::string("health socket failed: ") + std::strerror(errno);
        }
        return false;
    }

    int optval = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);

    if (bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
    {
        if (error)
        {
            *error = std::string("health bind failed: ") + std::strerror(errno);
        }
        ::close(fd);
        return false;
    }

    if (listen(fd, kBacklog) < 0)
    {
        if (error)
        {
            *error = std::string("health listen failed: ") + std::strerror(errno);
        }
        ::close(fd);
        return false;
    }

    socklen_t address_len = sizeof(address);
    if (getsockname(fd, reinterpret_cast<sockaddr*>(&address), &address_len) < 0)
    {
        if (error)
        {
            *error = std::string("health getsockname failed: ") + std::strerror(errno);
        }
        ::close(fd);
        return false;
    }

    listen_fd_ = fd;
    port_ = ntohs(address.sin_port);
    running_.store(true);

    try
    {
        worker_ = std::thread(&HealthServer::run, this);
    }
    catch (...)
    {
        running_.store(false);
        closeListenSocket();
        port_ = 0;
        if (error)
        {
            *error = "health worker thread failed to start";
        }
        return false;
    }

    return true;
}

void HealthServer::stop()
{
    std::lock_guard<std::mutex> lock(lifecycle_mutex_);
    ready_.store(false);
    if (!running_.exchange(false) && !worker_.joinable())
    {
        closeListenSocket();
        port_ = 0;
        return;
    }
    closeListenSocket();
    if (worker_.joinable())
    {
        worker_.join();
    }
    port_ = 0;
}

void HealthServer::setReady(bool ready)
{
    ready_.store(ready);
}

bool HealthServer::isReady() const
{
    return ready_.load();
}

bool HealthServer::isRunning() const
{
    return running_.load();
}

uint16_t HealthServer::port() const
{
    std::lock_guard<std::mutex> lock(lifecycle_mutex_);
    return port_;
}

void HealthServer::run()
{
    while (running_.load())
    {
        sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_fd = accept(listen_fd_, reinterpret_cast<sockaddr*>(&client_address), &client_len);
        if (client_fd < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            if (!running_.load())
            {
                break;
            }
            continue;
        }

        handleClient(client_fd);
        ::shutdown(client_fd, SHUT_RDWR);
        ::close(client_fd);
    }
}

void HealthServer::handleClient(int client_fd)
{
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    std::string request_line;
    if (!readRequestLine(client_fd, request_line))
    {
        sendAll(client_fd, buildResponse(400, "{\"status\":\"bad_request\"}"));
        return;
    }

    std::istringstream parser(request_line);
    std::string method;
    std::string path;
    std::string version;
    std::string extra;
    parser >> method >> path >> version;

    int status_code = 404;
    std::string body = "{\"status\":\"not_found\"}";

    if (!parser || (parser >> extra) || (version != "HTTP/1.0" && version != "HTTP/1.1"))
    {
        status_code = 400;
        body = "{\"status\":\"bad_request\"}";
    }
    else if (method != "GET")
    {
        status_code = 405;
        body = "{\"status\":\"method_not_allowed\"}";
    }
    else if (path == "/health/live")
    {
        status_code = 200;
        body = "{\"status\":\"ok\"}";
    }
    else if (path == "/health/ready")
    {
        if (ready_.load())
        {
            status_code = 200;
            body = "{\"status\":\"ready\"}";
        }
        else
        {
            status_code = 503;
            body = "{\"status\":\"not_ready\"}";
        }
    }

    sendAll(client_fd, buildResponse(status_code, body));
}

void HealthServer::closeListenSocket()
{
    if (listen_fd_ >= 0)
    {
        ::shutdown(listen_fd_, SHUT_RDWR);
        ::close(listen_fd_);
        listen_fd_ = -1;
    }
}
