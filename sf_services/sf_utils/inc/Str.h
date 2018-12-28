/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_STR_H_
#define INC_STR_H_
#include <string>
#include <vector>

namespace surfyn
{
namespace utils
{
   void split(const std::string& str, const std::string& delim, std::vector<std::string>& tokens);
}

}

#endif /* INC_STR_H_ */
