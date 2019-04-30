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

namespace surfyn
{
namespace rest_server
{

class DBaccess
{
private:
   std::shared_ptr<dynamodb_accessClient> m_client;
public:
   DBaccess();
   void fetchSummary(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query );
   void fetchDetails(utility::stringstream_t& sstream, const std::map<utility::string_t,  utility::string_t>& query );
private:
   static void fillFilterExprAndExprValue(std::stringstream &filterexpression, std::map<std::string, ValueType> &expressionValue,
                                      const std::string &param, const std::string& paramvalue, const std::string & value,
                                      const std::string& Operator);
};

}
}


#endif /* INC_DBACCESS_H_ */
