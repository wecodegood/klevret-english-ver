#pragma once

#include <thread>
#include <mutex>
#include <memory>

// синглтон
class ApiServer{
public:
    static ApiServer& instance();
    void start();
private:
    static ApiServer *_api_server;
    static std::mutex _mutex;
    ApiServer()=default;
    ApiServer(const ApiServer&)=delete;
    ApiServer(ApiServer&&)=delete;
    ApiServer& operator=(const ApiServer&)=delete;
};
