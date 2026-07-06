#pragma once

/**
 * Single Producer Single Consumer lock-free version of ring buffer
 */

#include <atomic>
#include <cstdint>
#include <cstring>

template<typename T>
class SPSCRingBuffer {
    static constexpr size_t CACHELINE = 64;  // per false sharing, opzionale
    alignas(CACHELINE) std::atomic<uint32_t> head_{0};
    alignas(CACHELINE) std::atomic<uint32_t> tail_{0};
    uint32_t size_;
    T* buffer_;

public:
    explicit SPSCRingBuffer(uint32_t size) : size_(size + 1) { // +1 per slot sentinella
        buffer_ = new T[size_]();
    }

    ~SPSCRingBuffer() { delete[] buffer_; }

    // No copy, no move
    SPSCRingBuffer(const SPSCRingBuffer&) = delete;
    SPSCRingBuffer& operator=(const SPSCRingBuffer&) = delete;

    bool push(const T& item) {
        uint32_t head = head_.load(std::memory_order_relaxed);
        uint32_t next_head = (head + 1) % size_;

        if (next_head == tail_.load(std::memory_order_acquire)) {
            return false; // pieno
        }

        buffer_[head] = item;
        head_.store(next_head, std::memory_order_release);

        return true;
    }

    bool pop(T& item) {
        uint32_t tail = tail_.load(std::memory_order_relaxed);

        if (tail == head_.load(std::memory_order_acquire)) {
            return false; // vuoto
        }

        item = buffer_[tail];
        tail_.store((tail + 1) % size_, std::memory_order_release);

        return true;
    }

    bool isEmpty() const {
        return head_.load(std::memory_order_acquire) == tail_.load(std::memory_order_relaxed);
    }

    bool isFull() const {
        uint32_t next = (head_.load(std::memory_order_relaxed) + 1) % size_;
        return next == tail_.load(std::memory_order_acquire);
    }
};