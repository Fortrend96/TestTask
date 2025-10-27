#include "Sorter.h"

void Sorter::mergeVec(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& out) {
    out.clear();
    out.reserve(a.size() + b.size());
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) out.push_back(a[i++]);
        else out.push_back(b[j++]);
    }
    while (i < a.size()) out.push_back(a[i++]);
    while (j < b.size()) out.push_back(b[j++]);
}

std::vector<int> Sorter::mergeSort(const std::vector<int>& v) {
    if (v.empty()) return {};
    return mergeSortRec(v, 0, v.size());
}

std::vector<int> Sorter::mergeSortRec(const std::vector<int>& v, size_t l, size_t r) {
    if (r - l <= 1) return std::vector<int>(v.begin() + l, v.begin() + r);
    size_t m = l + (r - l) / 2;
    auto left = mergeSortRec(v, l, m);
    auto right = mergeSortRec(v, m, r);
    std::vector<int> res;
    mergeVec(left, right, res);
    return res;
}
