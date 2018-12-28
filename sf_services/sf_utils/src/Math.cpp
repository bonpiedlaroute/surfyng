/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "Math.h"

namespace surfyn
{
namespace utils
{

uint64_t NchooseK(uint64_t n, uint64_t k)
{
   if( k > n ) return 0;
   if( k == 0 ) return 1;

   uint64_t result = n;
   for(uint64_t i = 2;  i < k; ++i)
   {
      result *= (n - i + 1);
      result /= i;
   }

   return result;
}

}
}
