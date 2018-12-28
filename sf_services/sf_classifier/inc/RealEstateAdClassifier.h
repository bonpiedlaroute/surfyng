/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_REALESTATEADCLASSIFIER_H_
#define INC_REALESTATEADCLASSIFIER_H_
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Criteria.h"
#include "RealEstateAd.h"


namespace surfyn
{
namespace classifier
{


enum class ClassifierState : char
{
   Training,
   Live
};

class RealEstateAdClassifier
{

public:
   RealEstateAdClassifier();
   bool isSame(const RealEstateAd& propa, const RealEstateAd& propb );
   void trainClassifier();
private:
   void loadAd(const std::string& filename);
   void loadSameAd(const std::string& filename);
   void init();
   void processAd(const std::vector<AdPair>& pairOfAd, AdType::Value adType);
   void computeSetCritOKWithCondition(const std::vector<CriteriaID::Value>& criteriaIds, const AdType::Value adtype,
                                                              std::vector<AdPair>& result);
   void computeSetCritOK(const std::vector<CriteriaID::Value>& criteriaIds, std::vector<AdPair>& setCritOK);
private:
   std::unordered_map<uint64_t, std::shared_ptr<RealEstateAd>> m_properties;
   ClassifierState                        m_state;
   std::vector<std::vector<int>>          m_similarAd;
   std::string                            m_realEstateDataFile;
   std::string                            m_similarAdFile;
   std::vector<std::shared_ptr<Criteria>> m_criteriaList;
   double                                 m_classifierThreshold;
};

}
}

#endif /* INC_REALESTATEADCLASSIFIER_H_ */
