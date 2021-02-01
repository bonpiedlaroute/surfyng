#include <bits/stdint-intn.h>
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
#include <unordered_set>

namespace surfyn
{
   class DataFormater
   {
   public:
      DataFormater();
      ~DataFormater();

      void ReadTableAndFormatEntries(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName, const std::string& city);
      void CheckSimilarAnnounces();
      void PutTargetTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName);
      void ReadSummaryTable(const std::shared_ptr<dynamodb_accessClient>& client, const std::string& tableName, const std::string& city);

   private:
      void ReadSelogerJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadLeboncoinJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadLogicImmoJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadBienIciJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadLaForetJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadOrpiJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadStephanePlazaImoJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadFonciaJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadCentury21JSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadGuyHoquetJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadArthurImmoJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadEraImmoJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadPapJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadIadFranceJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadParuVenduJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadAvendreAlouerJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadNestennJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadAgencePrincipaleJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadLeFigaroImmobilierJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadEtreProprioJSON(const std::string& json, classifier::RealEstateAd* realEstate);
      void ReadEfficityJSON(const std::string& json, classifier::RealEstateAd* realEstate);

      void PopulateValuesExtractFromDescription(const std::string& desc, classifier::RealEstateAd* realEstate) const;
   private:
      std::unordered_map<int64_t, classifier::RealEstateAd*> m_AnnouncesByID;
      std::unordered_set<int64_t> m_SummaryId;
      std::unordered_map<std::string, std::function<void (const std::string&, classifier::RealEstateAd* realEstate)>> m_ReaderBySources;
   };
}
