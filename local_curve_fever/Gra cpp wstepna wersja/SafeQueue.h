#ifndef SERVER_CURVE_FEVER_SAFE_QUEUE_H
#define SERVER_CURVE_FEVER_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template<class T> class SafeQueue {
private:
    std::queue<T> q;
    std::mutex m;
    std::condition_variable c;

public:
    void push(T val);
    T get();
};

template<class T> void SafeQueue<T>::push(T val) {
    std::lock_guard<std::mutex> lock(m);
    q.push(val);
    c.notify_one();
}

template<class T> T SafeQueue<T>::get() {
    std::unique_lock<std::mutex> l(m);
    while (q.empty())
    {
        c.wait(l);
    }
    T val = q.front();
    q.pop();
    return val;
}

#endif //SERVER_CURVE_FEVER_SAFE_QUEUE_H