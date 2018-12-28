/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_ALGORITHM_H_
#define INC_ALGORITHM_H_
#include <utility>
#include <vector>
#include <stdint.h>
#include <ctime>

namespace surfyn
{
namespace utils
{

/*
 * @brief  provides the resulting intersection between A and B
 *
 * @param   const std::vector<std::pair<uint64_t, uint64_t>>& A   input
 * @param   const std::vector<std::pair<uint64_t, uint64_t>>& B   input
 * @param   std::vector<std::pair<uint64_t, uint64_t>>& C         intersection result
 */
void pair_intersection(const std::vector<std::pair<uint64_t, uint64_t>>& A, const std::vector<std::pair<uint64_t, uint64_t>>& B,
                       std::vector<std::pair<uint64_t, uint64_t>>& C);

/*
 * @brief   provides the result of A - B
 *
 * @param   const std::vector<std::pair<uint64_t, uint64_t>>& A   input
 * @param   const std::vector<std::pair<uint64_t, uint64_t>>& B   input
 * @param   std::vector<std::pair<uint64_t, uint64_t>>& C         difference result
 */
void pair_difference(const std::vector<std::pair<uint64_t, uint64_t>>& A, const std::vector<std::pair<uint64_t, uint64_t>>& B,
                       std::vector<std::pair<uint64_t, uint64_t>>& C);

}
}
#endif /* INC_ALGORITHM_H_ */
