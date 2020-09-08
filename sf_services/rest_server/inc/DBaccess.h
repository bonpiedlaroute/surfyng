/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_DBACCESS_H_
#define INC_DBACCESS_H_
#include "stdafx.h"
#include <map>
#include "sf_services/dynamodb_access/thrift_generated/dynamodb_access.h"
#include <memory>
#include <set>
#include "sf_services/sf_utils/inc/Geolocal.h"

namespace surfyn
{
namespace rest_server
{

class DBaccess
{
private:
   std::shared_ptr<dynamodb_accessClient> m_client;
   std::shared_ptr<surfyn::utils::GeoLocal> m_geolocal;
public:
   DBaccess(const std::string& host, int port);
   void fetchSummary(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query );
   void fetchDetails(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query );
private:
   static void fillFilterExprAndExprValue(std::stringstream &filterexpression, std::map<std::string, ValueType> &expressionValue,
                                      const std::string &param, const std::string& paramvalue, const std::string & value,
                                      const std::string& Operator);
   static bool isAlreadyProvided(const std::map<std::string, std::string>& table_entry, const std::set<int64_t>& adprovided);
   static void updateProvidedAd(const std::map<std::string, std::string>& table_entry,  std::set<int64_t>& adprovided);
   static void buildJsonString(utility::stringstream_t& sstream, std::vector<std::pair<int, bool>>& announcesIds,
                                            std::unordered_map<int64_t, std::map<std::string, std::string> >& results);
};

}
}


#endif /* INC_DBACCESS_H_ */
