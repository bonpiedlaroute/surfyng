/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_DBACCESS_H_
#define INC_DBACCESS_H_
#include "stdafx.h"

namespace surfyn
{
namespace rest_server
{

class DBaccess
{
public:
   DBaccess() = default;
   void fetchData(utility::stringstream_t& sstream);
};

}
}


#endif /* INC_DBACCESS_H_ */
