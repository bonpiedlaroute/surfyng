/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include <gtest/gtest.h>
#include "Algorithm.h"

using namespace surfyn::utils;

TEST(TestPairIntersection, should_return_one_pair_when_only_one_match)
{
   std::vector<std::pair<uint64_t, uint64_t>> A {{1,2}, {1,9}, {2,13}, {3,5}};
   std::vector<std::pair<uint64_t, uint64_t>> B {{8,7}, {4,2}, {2,10} ,{1,9}};

   std::vector<std::pair<uint64_t, uint64_t>> C;
   std::vector<std::pair<uint64_t, uint64_t>> R {{1,9}};

   pair_intersection(A, B, C);

   EXPECT_TRUE(R == C);
}


TEST(TestPairIntersection, should_return_two_pair_when_only_two_match)
{
   std::vector<std::pair<uint64_t, uint64_t>> A {{1,2}, {1,9}, {2,13}, {3,5}};
   std::vector<std::pair<uint64_t, uint64_t>> B {{8,7}, {2,13}, {4,2}, {2,10} ,{1,9}};

   std::vector<std::pair<uint64_t, uint64_t>> C;
   std::vector<std::pair<uint64_t, uint64_t>> R {{1,9}, {2,13}};

   pair_intersection(A, B, C);

   EXPECT_TRUE(R == C);
}


TEST(TestPairIntersection, should_return_empty_vector_when_no_match)
{
   std::vector<std::pair<uint64_t, uint64_t>> A {{1,2}, {1,10}, {2,13}, {3,5}};
   std::vector<std::pair<uint64_t, uint64_t>> B {{8,7}, {2,11}, {4,2}, {2,10} ,{1,9}};

   std::vector<std::pair<uint64_t, uint64_t>> C;

   pair_intersection(A, B, C);

   EXPECT_TRUE(C.empty());
}

TEST(TestPairIntersection, should_return_one_pair_when_only_one_match_in_desorder)
{
   std::vector<std::pair<uint64_t, uint64_t>> A {{1,2}, {9,1}, {2,13}, {3,5}};
   std::vector<std::pair<uint64_t, uint64_t>> B {{8,7}, {4,2}, {2,10} ,{1,9}};

   std::vector<std::pair<uint64_t, uint64_t>> C;
   std::vector<std::pair<uint64_t, uint64_t>> R {{1,9}};

   pair_intersection(A, B, C);

   EXPECT_TRUE(R == C);
}

TEST(TestPairIntersection, should_return_two_pairs_when_only_two_match_in_desorder)
{
   std::vector<std::pair<uint64_t, uint64_t>> A {{1,2}, {9,1}, {2,13}, {3,5}, {2,4}};
   std::vector<std::pair<uint64_t, uint64_t>> B {{8,7}, {4,2}, {2,10} ,{1,9}};

   std::vector<std::pair<uint64_t, uint64_t>> C;
   std::vector<std::pair<uint64_t, uint64_t>> R {{1,9}, {2,4}};

   pair_intersection(A, B, C);

   EXPECT_TRUE(R == C);
}

TEST(TestPairDifference, should_return_remaining_set_when_match)
{
   std::vector<std::pair<uint64_t, uint64_t>> A {{1,2}, {9,1}, {2,13}, {3,5}, {2,4}};
   std::vector<std::pair<uint64_t, uint64_t>> B {{2,13} ,{1,9}};

   std::vector<std::pair<uint64_t, uint64_t>> C;
   std::vector<std::pair<uint64_t, uint64_t>> R {{1,2}, {2,4}, {3,5}};

   pair_difference(A, B, C);

   EXPECT_TRUE(R == C);
}


TEST(TestPairDifference, should_return_the_first_set_when_no_match)
{
   std::vector<std::pair<uint64_t, uint64_t>> A {{1,2}, {1,9}, {2,4}, {2,13}, {3,5}};
   std::vector<std::pair<uint64_t, uint64_t>> B {};

   std::vector<std::pair<uint64_t, uint64_t>> C;

   pair_difference(A, B, C);

   EXPECT_TRUE(A == C);
}


