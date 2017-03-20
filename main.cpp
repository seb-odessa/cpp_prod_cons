#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "queue.hpp"
#include "worker.hpp"

template<typename T>
void load_results(Queue<T> &queue, const T &quit) {
    using namespace std;
    const static chrono::milliseconds wait{10};
    mutex mtx;
    unique_lock<mutex> lck(mtx);
    condition_variable cv;

    while(true) {
        if (queue.empty()) {
            cv.wait_for(lck, wait);
        } else if (auto record = queue.pop()) {
            const T value = *record;  
            if (*record == quit)
                return;
            cout << "Result: " << value << "\n";
        }
    }
}

int main() {
    using namespace std;

    const static string quit{"quit"};
    deque<thread> threads;
    Queue<string> q1, q2, q3, q4;

    threads.push_back(move(thread{move(Worker<string>(q1, q2, 20, quit))}));
    threads.push_back(move(thread{move(Worker<string>(q2, q3, 60, quit))}));
    threads.push_back(move(thread{move(Worker<string>(q2, q3, 60, quit))}));
    threads.push_back(move(thread{move(Worker<string>(q3, q4, 20, quit))}));

    q1.push("one");
    q1.push("two");
    q1.push("three");
    q1.push("four");
    q1.push("five");
    q1.push("quit");

    load_results(q4, quit);

    while(!threads.empty()) {
        if (threads.front().joinable())
            threads.front().join();
        threads.pop_front();
    }

    return 0;
}