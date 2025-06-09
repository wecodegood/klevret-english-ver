#pragma once

#include <vector>
#include <cstdint>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include "HardwareAddress.hpp"
#include "NetworkInterface.hpp"

class UdpServer{
public:
    UdpServer(int port);
    ~UdpServer();
    std::vector<uint8_t> get_next_datagram();
    bool is_input_queue_blank();
    void stop();
    void send_to(std::vector<uint8_t> data);
private:
    int _socket;
    std::queue<std::vector<uint8_t>> _queue;
    std::mutex _mutex;
    std::atomic<bool> _stop_flag;
    std::thread* _pthread;
    std::vector<NetworkInterface> _host_interfaces;
    static void thread_func(UdpServer* udp_listener);
};
