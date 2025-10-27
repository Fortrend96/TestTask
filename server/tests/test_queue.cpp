#include <gtest/gtest.h>
#include "../src/IntQueue.h"

TEST(IntQueueTest, PushPop) {
    IntQueue q;
    q.push(10);
    q.push(20);
    auto a = q.pop();
    ASSERT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 10);
    auto b = q.pop();
    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 20);
    auto c = q.pop();
    EXPECT_FALSE(c.has_value());
}

TEST(IntQueueTest, ToVectorClear) {
    IntQueue q;
    q.push(1);
    q.push(2);
    q.push(3);
    auto v = q.to_vector();
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    q.clear();
    EXPECT_EQ(q.size(), 0);
}
