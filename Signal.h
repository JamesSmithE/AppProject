#pragma once

#include <functional>
#include <mutex>
#include <list>
#include <memory>
#include <AppConcepts.h>

namespace AppNamespace {

template <AppConcepts::HasMethod T, typename ... Args> class Connection {
public:
    Connection(T& inst, void(T::*method)(Args...)) : t{inst}, handler{method}  {}
    T& t;
    void(T::*handler)(Args...);
};

template <typename... Args>  class Signal {
public:

    using HandlerId = std::size_t;

    static constexpr HandlerId MaxId = std::size_t(128);

    Signal() : currentId{0}, syncMutex{std::make_shared<std::recursive_mutex>()} {
        handlersDetails.resize(128);
    }

    size_t ConnectionCount() const {
        std::lock_guard<std::recursive_mutex> lock(*syncMutex);
        return handlersDetails.size();
    }

    template <AppConcepts::DefaultConstructible T>
    [[nodiscard]] HandlerId AddHandler(T&& sub) {

        std::lock_guard<std::recursive_mutex> lock(*syncMutex);

        if(ConnectionCount() > MaxId-1)
            return std::numeric_limits<HandlerId>::max();

        auto id = currentId++;
        handlersDetails.emplace_back(id, [s = std::forward<T>(sub)](Args... args) { s(args...); });

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

    template<AppConcepts::DefaultConstructible T>
    [[nodiscard]] HandlerId operator+=(T&& t) {

        return AddHandler(std::forward<T>(t));
    }

    void operator -= (HandlerId id) {

        RemoveHandler(id);
    }

    template <AppConcepts::DefaultConstructible... Args2>
    void Emit(Args2&&... args) {

        for(auto& detail: handlersDetails) {

            detail.second(std::forward<Args2>(args)...);
        }
    }

    void ClearHandlers() {
        std::lock_guard<std::recursive_mutex> lock(*syncMutex);

        handlersDetails.clear();
    }


    template<AppConcepts::DefaultConstructible T>
    HandlerId operator+=(Connection<T,Args ...> &conn)  {

        auto f = [&conn] (Args... args) mutable {
            (conn.t.*conn.handler)(args...);
        };

        return AddHandler(f);
    }

    void Emit(Args... args) {

        Emit(args...);
    }

    void DisconnectFromAllSignals() {
        ClearHandlers();
    }

private:

    using activate = std::function<void(Args...)>;
    using details = std::pair<size_t, activate>;

    std::shared_ptr<std::recursive_mutex> syncMutex;
    size_t currentId;
    std::list<details> handlersDetails;

};
}



