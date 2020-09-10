/*
   (c) copyright 2019
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_ESTIMATORACCESS_H_
#define INC_ESTIMATORACCESS_H_
#include "stdafx.h"
#include <map>
#include "sklearn/thrift_generated/house_prediction.h"
#include <memory>

namespace surfyn
{
namespace rest_server
{
   class EstimatorAccess
   {
   private:
      std::shared_ptr<house_predictionClient> m_client;
   public:
      EstimatorAccess(const std::string& host, int port);
      void fetchHousePrice(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query,
        bool isFlat, const std::string& highpricebym2, const std::string& lowpricebym2);

   };
}
}


#endif /* INC_ESTIMATORACCESS_H_ */
