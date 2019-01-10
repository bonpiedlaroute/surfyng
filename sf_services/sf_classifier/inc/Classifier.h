/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_CLASSIFIER_H_
#define INC_CLASSIFIER_H_
#include "dynamodb_access.h"
#include <string>

namespace surfyn
{
namespace classifier
{

extern void detectSimilarRealEstateAd(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tablename);

}
}


#endif /* INC_CLASSIFIER_H_ */
