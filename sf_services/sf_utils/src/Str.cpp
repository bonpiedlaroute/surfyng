/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "Str.h"

namespace surfyn
{
namespace utils
{
   void split(const std::string& str, const std::string& delim, std::vector<std::string>& tokens)
   {
      std::string::size_type new_pos = 0, old_pos = 0;
      std::string token;

      while( (new_pos = str.find(delim, old_pos)) != std::string::npos )
      {
         token = str.substr(old_pos, new_pos-old_pos);

         if(!token.empty())
            tokens.push_back(token);

         old_pos = new_pos + delim.length();
      }

      token = str.substr(old_pos);

      if(!token.empty())
         tokens.push_back(token);
   }
}
}
