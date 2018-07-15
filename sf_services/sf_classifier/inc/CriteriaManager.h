/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_CRITERIAMANAGER_H_
#define INC_CRITERIAMANAGER_H_
#include <utility>
#include "Criteria.h"
#include "RealEstateAd.h"
#include <vector>

namespace surfyng
{
namespace classifier
{

class CriteriaManager
{
public:
   static void setNbPairsOfProperty(uint64_t nbProperty);
   static void setNbPairsOfSimilarProperty(uint64_t nbPairsOfSimilarProperty);
   static void setNbPairsOfNonSimilarProperty(uint64_t nbPairsOfNonSimilarProperty);
   static uint64_t getNbPairsOfNonSimilarProperty();
   static uint64_t getNbPairsOfSimilarProperty();
   static uint64_t getNbPairsOfProperty();
   static void    computeProbability();
   static double  getProbabilitySimilarProperty();
   static void    setData(uint64_t i, uint64_t j, AdPair p);
   static std::vector<AdPair> getData(uint64_t i, uint64_t j);
   static void    showData();
private:
   static uint64_t m_nbPairsOfSimilarProperty;
   static uint64_t m_nbPairsOfNonSimilarProperty;
   static uint64_t m_nbPairsOfProperty;
   static double  m_probabilitySimilarProperty;
   static std::vector<AdPair> m_datas[CriteriaID::Value::CriteriaMax][AdType::Value::AdTypeMax];
};

}
}

#endif /* INC_CRITERIAMANAGER_H_ */
