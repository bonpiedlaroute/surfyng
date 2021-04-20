/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_DBACCESS
#define INC_DBACCESS
#include "stdafx.h"
#include <bits/stdint-intn.h>
#include <map>
#include "sf_services/dynamodb_access/thrift_generated/dynamodb_access.h"
#include <memory>
#include <unordered_set>
#include "sf_services/sf_utils/inc/Geolocal.h"

namespace surfyn
{
namespace rest_server
{

class DBaccess
{
private:
   std::shared_ptr<dynamodb_accessClient> m_client;
public:
   DBaccess(const std::string& host, int port);
   void fetchSummary(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query );
   void fetchDetails(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query, 
   const std::shared_ptr<surfyn::utils::GeoLocal>&  geolocalservice);
   void fetchHighAndLowPriceByM2(const std::string& cityId, bool isFlat, std::string& highpricebym2, std::string& lowpricebym2);
   void fetchCityInfo(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query, 
   const std::shared_ptr<surfyn::utils::GeoLocal>&  geolocalservice);
private:
   static void fillFilterExprAndExprValue(std::stringstream &filterexpression, std::map<std::string, ValueType> &expressionValue,
                                      const std::string &param, const std::string& paramvalue, const std::string & value,
                                      const std::string& Operator);
   static bool isAlreadyProvided(const std::string& id, const std::unordered_set<std::string>& adprovided);
   static void updateProvidedAd(const std::string& dup,  std::unordered_set<std::string>& adprovided);
   static void buildJsonString(utility::stringstream_t& sstream,
                                            std::vector<std::map<std::string, std::string>>& results);
};

}
}


#endif /* INC_DBACCESS */
