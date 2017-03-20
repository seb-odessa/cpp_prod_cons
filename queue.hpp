#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>
#include <deque>
#include <boost/optional.hpp>

template <typename T>
class Queue {
    std::deque<T> _deque;
    std::mutex    _mutex;
public:
    Queue():_deque(), _mutex() {}
    Queue(const Queue&) = delete;    
    Queue& operator=(const Queue&) = delete;

    void push(const T& elem) {
        std::lock_guard<std::mutex> lck(_mutex);
        _deque.push_back(elem);
    }

    void push(const T&& elem) {
        std::lock_guard<std::mutex> lck(_mutex);
        _deque.push_back(std::move(elem));
    }

    boost::optional<T> pop() {
        std::lock_guard<std::mutex> lck(_mutex);
        if (_deque.empty())
            return boost::optional<T>();

        T value = std::move(_deque.front());
        _deque.pop_front();
        return boost::optional<T>(value);
    }

    bool empty() {
        std::lock_guard<std::mutex> lck(_mutex);
        return _deque.empty();
    }
};
#endif