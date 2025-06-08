#pragma once

#include <functional>
#include <mutex>
#include <list>
#include <thread>
#include <memory>

#include <AppConcepts.h>

namespace AppNamespace {

template <typename ... Args> class AsyncConnection {
public:
    AsyncConnection(std::function<void(Args...)> callable, bool runAsync = true)
        : callableT{std::move(callable)}, _runAsync{runAsync} {
    }
    AsyncConnection(const AsyncConnection& a) {
        _runAsync = a._runAsync;
        callableT = a.callableT;
    };
    AsyncConnection(AsyncConnection&& a) {
        _runAsync = a._runAsync;
        callableT = std::move(a.callableT);
    };
    std::function<void(Args...)> callableT;
    bool _runAsync;

    AsyncConnection& operator=(const AsyncConnection&)  = default;
    AsyncConnection& operator=(AsyncConnection&&) = default;
};

template <typename... Args> class Signal {
public:
    using HandlerId = std::size_t;
    static constexpr HandlerId MaxId = std::numeric_limits<HandlerId>::max();

    Signal() : currentId{0}, syncMutex{std::make_shared<std::recursive_mutex>()} {
    }

    Signal(const Signal& s) {
        currentId = s.currentId;
        handlersDetails = s.handlersDetails;
    };
    Signal(Signal&& s) {
        currentId = s.currentId;
        handlersDetails = std::move(s.handlersDetails);
    };

    size_t ConnectionCount() const {
        std::lock_guard<std::recursive_mutex> lock(*syncMutex);
        return handlersDetails.size();
    }
    template <typename T>
    [[nodiscard]] HandlerId AddHandler(T&& sub) {
        std::lock_guard<std::recursive_mutex> lock(*syncMutex);
        if(ConnectionCount() > MaxId-1)
            return std::numeric_limits<HandlerId>::max();
        auto id = currentId++;
        handlersDetails.emplace_back(
          std::pair<std::size_t,activate>
          {
              id,
              std::move(activate {
                sub._runAsync,
                id,
                [s = std::forward<decltype(sub.callableT)>(sub.callableT)](Args... args) { s(args...); }
                })
          });
        return id;
    }
    [[nodiscard]] bool RemoveHandler(HandlerId id) {
        std::lock_guard<std::recursive_mutex> lock(*syncMutex);
        auto it = std::find_if(handlersDetails.begin(), handlersDetails.end(), [&](auto& i) {
            return i.first == id;
        });
        if(it != handlersDetails.end()) {
            handlersDetails.erase(it);
            return true;
        }
        return false;
    }
    template <typename... Args2>
    void operator()(Args2&&... args) {
        Emit(std::forward<Args2>(args) ...);
    }
    template<typename T>
    [[nodiscard]] HandlerId operator+=(T&& t) {
        return AddHandler(std::forward<T>(t));
    }
    void operator -= (HandlerId id) {
        RemoveHandler(id);
    }

    void TryJoinOnAll() {
        for(auto& detail: handlersDetails) {
            if(detail.second.runAsync) {
                if(detail.second.thread != nullptr && detail.second.thread->joinable()) {
                    detail.second.thread->join();
                }
            }
        }
    }

    template <typename... Args2>
    void Emit(Args2&&... args) {
        for(auto& detail: handlersDetails) {
            if(detail.second.runAsync) {
                if(detail.second.thread != nullptr && detail.second.thread->joinable()) {
                    continue;
                } else {
                    detail.second.thread =
                        std::make_unique<std::thread>([&] () {
                            detail.second.handlerFunc(args...);
                        });
                }
            } else {
                detail.second.handlerFunc(args...);
            }
        }
    }
    void ClearHandlers() {
        std::lock_guard<std::recursive_mutex> lock(*syncMutex);
        handlersDetails.clear();
    }
    void DisconnectFromAllSignals() {
        ClearHandlers();
    }
private:
    struct activate {
        activate(bool async, std::size_t id, std::function<void(Args...)> h)
            : runAsync{async}, handlerFunc{h}, thread{}, _id{id} {
        }
        activate(const activate& a) {
            DoAssignments(a);
        }
        activate(activate&& a) {
            DoAssignments(a);
        }
        ~activate() {
            if(thread != nullptr)
              if(thread->joinable())
                thread->join();
        }
        activate& operator=(const activate& a) {
            DoAssignments(a);
        }
        activate& operator=(const activate&& a) {
            DoAssignments(a);
        }
        void DoAssignments(activate&& a) {
            handlerFunc = std::move(a.handlerFunc);
            _id = a._id;
            runAsync = a.runAsync;
        }
        void DoAssignments(const activate& a) {
            handlerFunc = a.handlerFunc;
            _id = a._id;
            runAsync = a.runAsync;
        }
        bool runAsync;
        std::function<void(Args ...)> handlerFunc;
        std::unique_ptr<std::thread> thread;
        std::size_t _id;
    };
    using details = std::pair<size_t, activate>;
    std::shared_ptr<std::recursive_mutex> syncMutex;
    size_t currentId;
    std::list<details> handlersDetails;
};
}



