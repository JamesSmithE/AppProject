#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <atomic>
#include <unistd.h>
#include <sys/select.h>
#include <functional>
#include <string>
#include <memory>
#include <thread>

// #include "AppConcepts.h"
#include "DatagramReceiver.h"

using ResponseDataType = std::string;

template <typename Protocol = DatagramReceiver>
class WhoIsReceiver {
public:

    using Error = typename Protocol::Error;
    using ErrorHandler = typename Protocol::ErrorHandler;
    using DataReady = std::function<void(std::string)>;
    using StartTransmitter = std::function<void()>;

    WhoIsReceiver(const std::string& bindAddress,
                  const unsigned int portNum,
                  const std::string& multicastAddr) {

        protocol = std::make_unique<Protocol>(bindAddress,
                                              portNum,
                                              multicastAddr,
                                              std::move([this](std::string&&,Error)->void{}));
        protocol->readyToReceive = [this]()->void {
            startTransmitter();
        };
        protocol->searchDone = [&](std::string&& msg)->void {
            onSearchDone(msg);
        };
    }

    WhoIsReceiver(const WhoIsReceiver&) = delete;
    WhoIsReceiver(WhoIsReceiver&&) = delete;

    WhoIsReceiver operator=(const WhoIsReceiver&) = delete;
    WhoIsReceiver operator=(WhoIsReceiver&&) = delete;

    void startSearching() {
        setupThreadExec();
    }

    ErrorHandler onError;
    DataReady onDataReady;
    StartTransmitter startTransmitter;

private:
    void getResponseData() {
        std::string msg = protocol->getResponses();
    }
    void onMessageReceived(ResponseDataType&& data) {
        std::lock_guard<std::mutex> guard(callback_mutex);
        onDataReady(std::move(data));
    }
    void onSearchDone(const std::string& message) {
        onMessageReceived(message.c_str());
        thread_stopped = true;
    }
    void setupThreadExec() {
        thread_stopped = false;
        tryThreadJoin();
        whoIsSearchThread = std::make_unique<std::thread>([&] () {
            protocol->startListening();
        });
    }
    void tryThreadJoin() {
        if(whoIsSearchThread != nullptr) {
            if(whoIsSearchThread->joinable())
                whoIsSearchThread->join();
        }
    }
    std::unique_ptr<Protocol> protocol;
    std::atomic<bool> thread_stopped;
    std::mutex callback_mutex;
    std::unique_ptr<std::thread> whoIsSearchThread;
};
