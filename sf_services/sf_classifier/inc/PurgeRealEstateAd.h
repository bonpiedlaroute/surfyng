/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_PURGEREALESTATEAD_H_
#define INC_PURGEREALESTATEAD_H_

#include <memory>
#include "surfyng/sf_services/dynamodb_access/thrift_generated/dynamodb_access.h"

extern void purgeRealEstateAd(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tablename);
#endif /* INC_PURGEREALESTATEAD_H_ */
