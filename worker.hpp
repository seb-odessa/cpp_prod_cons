#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "queue.hpp"

template <typename T>
class Worker {    
    Queue<T> &_input;
    Queue<T> &_output;
    const std::chrono::milliseconds busy;
    const T quit;
    const long id;
    static long s_id;

    T process(T &&value) {
        std::cout<<id<<" process "<<value<<" started\n";
        std::this_thread::sleep_for(busy);
        std::cout<<id<<" process "<<value<<" done\n";
        return std::move(value);
    }

public:
    Worker(Queue<T> &input, Queue<T> &output, long ms, const T &quit): _input(input), _output(output), busy{ms}, quit{quit}, id{++s_id} {}

    void operator()() {
        using namespace std;
        const static chrono::milliseconds wait{10};
        mutex mtx;
        unique_lock<mutex> lck(mtx);
        condition_variable cv;

        while(true) {
            if (_input.empty()) {
                cv.wait_for(lck, wait);
            } else if (auto record = _input.pop()) {
                const bool stop = *record == quit;
                _output.push(process(move(*record)));
                if (stop) {
                    _input.push(quit);
                    break;
                }
            }
        }
    }
};

template<typename T>
long Worker<T>::s_id{0};

#endif

