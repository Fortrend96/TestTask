#pragma once
#include <cstddef>
#include <vector>
#include <mutex>
#include <optional>

class IntQueue
{
public:
    IntQueue();
    ~IntQueue();

    void push(int val);
    std::optional<int> pop();
    std::vector<int> to_vector() const;
    void clear();
    size_t size() const;

private:
    struct Node
    {
        int value;
        Node* next;
        Node* prev;
    };

    Node* head_;
    Node* tail_;
    size_t size_;
    mutable std::mutex mtx_;
};
