/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "RealEstateAdClassifier.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "Config.h"
#include "Str.h"
#include "Algorithm.h"
#include <fstream>
#include <utility>
#include "CriteriaManager.h"
#include <iostream>
#include <math.h>
#include <iterator>

namespace surfyn
{
namespace classifier
{

double classifier_epsilon = pow(10,-6);

RealEstateAdClassifier::RealEstateAdClassifier()
{
   surfyn::utils::Config classifier_conf("sf_classifier.ini");
   classifier_conf.loadconfig();
   m_realEstateDataFile = classifier_conf.getStringValue("realestate_data_file");
   m_similarAdFile = classifier_conf.getStringValue("similar_ad_file");
   m_classifierThreshold = classifier_conf.getDoubleValue("classifier_threshold");

   init();

   m_state = ClassifierState::Training;
}

void RealEstateAdClassifier::loadAd(const std::string& filename)
{
   using boost::property_tree::ptree;

   ptree pt;
   read_json(filename, pt);

   for (auto & array_element: pt)
   {
      std::shared_ptr<RealEstateAd> realEstateAd = std::make_shared<RealEstateAd>(0);
      for (auto & property: array_element.second)
      {
         if( property.first == RealEstateKey)
         {
            realEstateAd->setId(atoi(property.second.get_value <std::string >().c_str()));
         }
         else
         {
            realEstateAd->setDescription(property.first,property.second.get_value < std::string > ());
         }
      }
      if(realEstateAd->getId() != 0 )
      {
         m_properties[realEstateAd->getId()] = realEstateAd;
      }
   }

}

void RealEstateAdClassifier::loadSameAd(const std::string& filename)
{
   std::ifstream file;
   file.open(filename);
   std::string line;

   while(std::getline(file, line))
   {
      std::vector<std::string> results;

      surfyn::utils::split(line, ";", results);

      std::vector<int> values;

      for(auto val: results)
      {
         values.emplace_back(atoi(val.c_str()));
      }

      m_similarAd.emplace_back(values);
   }
}


void RealEstateAdClassifier::init()
{
   m_criteriaList.emplace_back(std::make_shared<CriteriaPrice>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaSurface>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaConstructionYear>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaTimeToPublicTransport>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaBeds>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaCity>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaPropertyType>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaRooms>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaTypeOfHeating>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaCellars>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaFloor>());
   m_criteriaList.emplace_back(std::make_shared<CriteriaParking>());
}

void RealEstateAdClassifier::trainClassifier()
{
   if (!m_realEstateDataFile.empty())
   {
      loadAd(m_realEstateDataFile);
   }
   if (!m_similarAdFile.empty())
   {
      loadSameAd(m_similarAdFile);
   }

   std::vector<AdPair> pairOfSimilarAd;

   // generate all pair of similar ad
   for(auto adlist : m_similarAd)
   {
      for(int i = 0; i < adlist.size(); ++i)
      {
         for(int j = i+1;  j < adlist.size(); ++j)
         {
            pairOfSimilarAd.emplace_back(std::make_pair(adlist[i], adlist[j]));
         }
      }
   }

   CriteriaManager::setNbPairsOfSimilarProperty(pairOfSimilarAd.size());

   std::vector<AdPair> pairOfAd;

   //generate all pair of ad
   for(auto iter = m_properties.begin(); iter != m_properties.end(); ++iter)
   {
      for(auto it = std::next(iter, 1); it != m_properties.end(); ++it)
      {
         pairOfAd.emplace_back(std::make_pair(iter->first, it->first));
      }
   }

   //process all pair of similar ad
   processAd(pairOfSimilarAd, AdType::Value::SimilarAd);

   std::vector<AdPair> pairOfNonSimilarAd;

   surfyn::utils::pair_difference(pairOfAd, pairOfSimilarAd, pairOfNonSimilarAd);

   //process all pair of ad
   processAd(pairOfNonSimilarAd, AdType::Value::NonSimilarAd);

   CriteriaManager::setNbPairsOfProperty(pairOfAd.size());

   CriteriaManager::setNbPairsOfNonSimilarProperty(pairOfNonSimilarAd.size());

}

void RealEstateAdClassifier::processAd(const std::vector<AdPair>& pairOfAd, AdType::Value adType)
{
   for( auto similarAd : pairOfAd)
   {
      for( auto crit : m_criteriaList)
      {
         auto iter_prop1 = m_properties.find(similarAd.first);
         auto iter_prop2 = m_properties.find(similarAd.second);

         if( iter_prop1 != m_properties.end() && iter_prop2 != m_properties.end())
         {
            if( crit->isValid(*(iter_prop1->second)) && crit->isValid(*(iter_prop2->second)) )
            {
               if( crit->isEquals(*(iter_prop1->second), *(iter_prop2->second)) )
               {
                  CriteriaManager::setData(crit->getId(), adType, std::make_pair(iter_prop1->first, iter_prop2->first) );
               }

            }
         }
      }
   }
}

void RealEstateAdClassifier::computeSetCritOKWithCondition(const std::vector<CriteriaID::Value>& criteriaIds, const AdType::Value adtype,
                                                           std::vector<AdPair>& result)
{
   auto iter = criteriaIds.begin();

   result = CriteriaManager::getData(*iter, adtype);

   for(iter = std::next(iter, 1); iter != criteriaIds.end() && !result.empty(); ++iter)
   {
      std::vector<AdPair> temp;
      surfyn::utils::pair_intersection(result,CriteriaManager::getData(*iter, adtype), temp);
      result = temp;
   }
}

void RealEstateAdClassifier::computeSetCritOK(const std::vector<CriteriaID::Value>& criteriaIds, std::vector<AdPair>& setCritOK)
{
   auto iter = criteriaIds.begin();

   auto critokSimAdList = CriteriaManager::getData(*iter, AdType::Value::SimilarAd);
   auto critokNonSimAdList = CriteriaManager::getData(*iter, AdType::Value::NonSimilarAd);

   setCritOK.reserve(critokSimAdList.size() + critokNonSimAdList.size());

   setCritOK.insert(setCritOK.end(), critokSimAdList.begin(), critokSimAdList.end());
   setCritOK.insert(setCritOK.end(), critokNonSimAdList.begin(), critokNonSimAdList.end());

   for( iter = std::next(iter, 1); iter != criteriaIds.end() && !setCritOK.empty(); ++iter)
   {
      std::vector<AdPair> temp, result;
      critokSimAdList = CriteriaManager::getData(*iter, AdType::Value::SimilarAd);
      critokNonSimAdList = CriteriaManager::getData(*iter, AdType::Value::NonSimilarAd);

      temp.reserve(critokSimAdList.size() + critokNonSimAdList.size());

      temp.insert(temp.end(), critokSimAdList.begin(), critokSimAdList.end());
      temp.insert(temp.end(), critokNonSimAdList.begin(), critokNonSimAdList.end());

      surfyn::utils::pair_intersection(setCritOK, temp, result);

      setCritOK = result;
   }
}

bool RealEstateAdClassifier::isSame(const RealEstateAd& propa, const RealEstateAd& propb)
{
   std::vector<CriteriaID::Value> criteriaIds;
   for(auto crit: m_criteriaList )
   {
      if( crit->isValid(propa) && crit->isValid(propb))
      {
         if(crit->isEquals(propa, propb))
         {
            criteriaIds.emplace_back(crit->getId());
         }
      }
   }

   if( !criteriaIds.empty())
   {
      std::vector<AdPair> setCritOKAndSameProperty;

      computeSetCritOKWithCondition(criteriaIds, AdType::Value::SimilarAd, setCritOKAndSameProperty);

      double probaCritOKKnowingThatSameProperty = static_cast<double>(setCritOKAndSameProperty.size())/ static_cast<double>(CriteriaManager::getNbPairsOfSimilarProperty()) ;

      std::vector<AdPair> setCritOK;
      computeSetCritOK(criteriaIds, setCritOK);

      double probaCritOK = static_cast<double>(setCritOK.size())/ static_cast<double> (CriteriaManager::getNbPairsOfProperty());

      double probaSimilarAd = static_cast<double> (CriteriaManager::getNbPairsOfSimilarProperty()) / static_cast<double>( CriteriaManager::getNbPairsOfProperty());

      double probaSamePropertyKnowingThatCritOK = (probaCritOKKnowingThatSameProperty * probaSimilarAd) / probaCritOK;


      std::vector<AdPair> setCritOKAndNotSameProperty;
      computeSetCritOKWithCondition(criteriaIds, AdType::Value::NonSimilarAd, setCritOKAndNotSameProperty);


      double probaCritOKKnowingThatNotSameProperty = static_cast<double>(setCritOKAndNotSameProperty.size())/ static_cast<double>(CriteriaManager::getNbPairsOfNonSimilarProperty()) ;
      double probaNonSimilarAd = static_cast<double> (CriteriaManager::getNbPairsOfNonSimilarProperty()) / static_cast<double>( CriteriaManager::getNbPairsOfProperty());

      double probaNotSamePropertyKnowingThatCritOK = (probaCritOKKnowingThatNotSameProperty * probaNonSimilarAd) / probaCritOK;

      return probaSamePropertyKnowingThatCritOK > probaNotSamePropertyKnowingThatCritOK ;
   }
   else
   {
      return false;
   }

}

}
}
