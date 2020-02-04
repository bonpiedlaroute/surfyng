/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_PURGEREALESTATEAD_H_
#define INC_PURGEREALESTATEAD_H_

#include <memory>
#include "dynamodb_access.h"

namespace surfyn
{
namespace classifier
{
extern void purgeRealEstateAd(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tablename, const std::string& city);
}
}

#endif /* INC_PURGEREALESTATEAD_H_ */
