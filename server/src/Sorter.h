#pragma once
#include <vector>

class Sorter
{
public:
    Sorter() = delete;
    Sorter(const Sorter&) = delete;
    Sorter& operator=(const Sorter&) = delete;
    Sorter(Sorter&&) = delete;
    Sorter& operator=(Sorter&&) = delete;

    static std::vector<int> mergeSort(const std::vector<int>& v);
private:
    static std::vector<int> mergeSortRec(const std::vector<int>& v, size_t l, size_t r);
    static void mergeVec(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& out);
};
