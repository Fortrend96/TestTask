#include "IntQueue.h"
#include <cassert>

IntQueue::IntQueue() :
    head_(nullptr)
    , tail_(nullptr)
    , size_(0)
{}

IntQueue::~IntQueue()
{
    clear();
}

void IntQueue::push(int val)
{
    std::lock_guard<std::mutex> lock(mtx_);
    Node* n = new Node{val, nullptr, nullptr};    
    if (!tail_) {
        head_ = tail_ = n;
    } else {
        n->prev = tail_;
        tail_->next = n;
        tail_ = n;
    }
    ++size_;
}

std::optional<int> IntQueue::pop()
{
    std::lock_guard<std::mutex> lock(mtx_);

    if (!head_)
        return std::nullopt;

    Node* n = head_;
    int val = n->value;
    head_ = n->next;

    if (head_)
        head_->prev = nullptr;
    else
        tail_ = nullptr;

    delete n;
    --size_;
    return val;
}

std::vector<int> IntQueue::to_vector() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<int> out;    
    out.reserve(size_);
    Node* cur = head_;

    while (cur)
    {
        out.push_back(cur->value);
        cur = cur->next;
    }

    return out;
}

void IntQueue::clear()
{
    std::lock_guard<std::mutex> lock(mtx_);
    Node* cur = head_;

    while (cur)
    {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }

    head_ = tail_ = nullptr;
    size_ = 0;
}

size_t IntQueue::size() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return size_;
}
