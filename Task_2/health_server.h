#ifndef TASK_2_HEALTH_SERVER_H
#define TASK_2_HEALTH_SERVER_H

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>

class HealthServer
{
public:
    static const uint16_t DEFAULT_PORT = 8080;

    HealthServer();
    ~HealthServer();

    HealthServer(const HealthServer&) = delete;
    HealthServer& operator=(const HealthServer&) = delete;

    bool start(uint16_t port, std::string* error = nullptr);
    void stop();

    void setReady(bool ready);
    bool isReady() const;
    bool isRunning() const;
    uint16_t port() const;

private:
    void run();
    void handleClient(int client_fd);
    void closeListenSocket();

    mutable std::mutex lifecycle_mutex_;
    std::thread worker_;
    std::atomic<bool> running_;
    std::atomic<bool> ready_;
    int listen_fd_;
    uint16_t port_;
};

#endif
