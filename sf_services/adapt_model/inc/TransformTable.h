#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <dynamodb_access.h>
#include <dynamodb_access_types.h>
#include "rapidjson/document.h"
#include <sf_services/sf_utils/inc/Logger.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/algorithm/string.hpp>
#include <numeric>
#include <set>
#include <locale>
#include <unordered_map>

#include "sf_services/sf_classifier/inc/RealEstateAd.h"

namespace surfyn
{
   class DataFormater
   {
   public:
      DataFormater() = default;
      ~DataFormater() = default;

      void ReadTableAndFormatEntries(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName);
      void CheckSimilarAnnounces();
      void PutTargetTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName);

   private:
      void ReadSelogerJSON(const std::string json, classifier::RealEstateAd& realEstate);
      void ReadLeboncoinJSON(const std::string json, classifier::RealEstateAd& realEstate);

   private:
      std::unordered_map<int64_t, classifier::RealEstateAd> m_AnnouncesByID;
   };
}
