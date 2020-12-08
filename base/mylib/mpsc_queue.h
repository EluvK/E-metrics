#pragma once
#include "mylib/basic_define.h"


#include <atomic>

NS_BEG1(mylib)

template <typename T>
struct node {
    std::atomic<node<T> *> next;
    T value;

    node(T v) : value(v) { this->next.exchange(std::atomic<node<T> *>(nullptr)); }
};

template <typename T>
class mpsc_queue {
private:
    node<T> * head;
    std::atomic<node<T> *> tail;

public:
    mpsc_queue() {
        auto _tmp = new node<T>(T{});
        this->tail.exchange(std::atomic<node<T> *>(_tmp));
        this->head = _tmp;
    }
    DELETED_COPY_AND_MOVE_SEMANTICS(mpsc_queue);

    ~mpsc_queue() {
        if (head != nullptr) {
            while (!this->empty()) {
                this->pop();
            }
            delete this->head;
        }
    }

    void push(T v) {
        auto n = new node<T>(v);
        auto prev = this->tail.exchange(n, std::memory_order_acq_rel);
        prev->next.store(n, std::memory_order_release);
    }

    T pop() {
        auto head = this->head;
        auto next = head->next.load(std::memory_order_acquire);
        if (next != nullptr) {
            delete this->head;
            this->head = next;
            auto ret = next->value;
            return ret;
        }
        return T{};
    }

    std::vector<T> pop_all() {
        std::vector<T> res;
        if (empty())
            return res;

        for (;;) {
            auto head = this->head;
            auto next = head->next.load(std::memory_order_acquire);
            if (next != nullptr) {
                delete this->head;
                this->head = next;
                auto ret = next->value;
                res.push_back(ret);
            } else {
                break;
            }
        }

        return res;
    }

    bool empty() const { return this->tail.load(std::memory_order_acquire) == head; }
};

NS_END1