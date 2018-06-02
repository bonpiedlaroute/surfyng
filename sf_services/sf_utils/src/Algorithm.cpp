/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "Algorithm.h"
#include <algorithm>

namespace surfyng
{
namespace utils
{

struct InOrder
{
   std::pair<uint64_t, uint64_t> operator()(std::pair<uint64_t, uint64_t> pair)
   {
      if( pair.second < pair.first )
      {
         return std::make_pair(pair.second, pair.first);
      }
      else
      {
         return pair;
      }
   }
};

void pair_intersection(const std::vector<std::pair<uint64_t, uint64_t>>& A, const std::vector<std::pair<uint64_t, uint64_t>>& B,
                       std::vector<std::pair<uint64_t, uint64_t>>& C)
{
   std::vector<std::pair<uint64_t, uint64_t>> tempA, tempB;
   tempA.resize(A.size());
   tempB.resize(B.size());

   std::transform(A.begin(), A.end(),tempA.begin(), InOrder());
   std::transform(B.begin(), B.end(),tempB.begin(), InOrder());

   std::sort(tempA.begin(), tempA.end());
   std::sort(tempB.begin(), tempB.end());

   std::set_intersection(tempA.begin(), tempA.end(), tempB.begin(), tempB.end(), std::back_inserter(C));
}

void pair_difference(const std::vector<std::pair<uint64_t, uint64_t>>& A, const std::vector<std::pair<uint64_t, uint64_t>>& B,
                       std::vector<std::pair<uint64_t, uint64_t>>& C)
{
   std::vector<std::pair<uint64_t, uint64_t>> tempA, tempB;
   tempA.resize(A.size());
   tempB.resize(B.size());

   std::transform(A.begin(), A.end(),tempA.begin(), InOrder());
   std::transform(B.begin(), B.end(),tempB.begin(), InOrder());

   std::sort(tempA.begin(), tempA.end());
   std::sort(tempB.begin(), tempB.end());

   std::set_difference(tempA.begin(), tempA.end(), tempB.begin(), tempB.end(), std::back_inserter(C));
}

}
}
