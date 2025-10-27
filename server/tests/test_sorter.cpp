#include <gtest/gtest.h>
#include "../src/Sorter.h"
#include <vector>

TEST(MergeSortTest, Empty) {
    std::vector<int> v;
    auto r = Sorter::mergeSort(v);
    EXPECT_TRUE(r.empty());
}

TEST(MergeSortTest, Sorted) {
    std::vector<int> v{1,2,3,4};
    auto r = Sorter::mergeSort(v);
    EXPECT_EQ(r, v);
}

TEST(MergeSortTest, Unsorted) {
    std::vector<int> v{5,1,4,2,3,2};
    auto r = Sorter::mergeSort(v);
    std::vector<int> expected{1,2,2,3,4,5};
    EXPECT_EQ(r, expected);
}


