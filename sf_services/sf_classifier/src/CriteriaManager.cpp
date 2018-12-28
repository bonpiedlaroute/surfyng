/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/

#include "CriteriaManager.h"
#include <iostream>

namespace surfyn
{
namespace classifier
{

uint64_t CriteriaManager::m_nbPairsOfProperty = 0;
uint64_t CriteriaManager::m_nbPairsOfSimilarProperty = 0;
uint64_t CriteriaManager::m_nbPairsOfNonSimilarProperty = 0;
double  CriteriaManager::m_probabilitySimilarProperty = 0.0;
std::vector<AdPair> CriteriaManager::m_datas[CriteriaID::Value::CriteriaMax][AdType::Value::AdTypeMax];

void CriteriaManager::setNbPairsOfProperty(uint64_t nbProperty)
{
   m_nbPairsOfProperty = nbProperty;
}

void CriteriaManager::setNbPairsOfSimilarProperty(uint64_t nbPairsOfSimilarProperty)
{
   m_nbPairsOfSimilarProperty = nbPairsOfSimilarProperty;
}

uint64_t CriteriaManager::getNbPairsOfProperty()
{
   return m_nbPairsOfProperty;
}

uint64_t CriteriaManager::getNbPairsOfSimilarProperty()
{
   return m_nbPairsOfSimilarProperty;
}

void CriteriaManager::computeProbability()
{
   m_probabilitySimilarProperty = static_cast<double>(m_nbPairsOfSimilarProperty) / static_cast<double>(m_nbPairsOfProperty);
}


double CriteriaManager::getProbabilitySimilarProperty()
{
   return m_probabilitySimilarProperty;
}

void CriteriaManager::setData(uint64_t i, uint64_t j, AdPair p)
{
   m_datas[i][j].emplace_back(p);
}

std::vector<AdPair> CriteriaManager::getData(uint64_t i, uint64_t j)
{
   return m_datas[i][j];
}

void CriteriaManager::setNbPairsOfNonSimilarProperty(uint64_t nbPairsOfNonSimilarProperty)
{
   m_nbPairsOfNonSimilarProperty = nbPairsOfNonSimilarProperty;
}
uint64_t CriteriaManager::getNbPairsOfNonSimilarProperty()
{
   return m_nbPairsOfNonSimilarProperty;
}

void CriteriaManager::showData()
{
   for(int i = 0 ; i < CriteriaID::Value::CriteriaMax; ++i)
   {
      for(int j = 0; j < AdType::Value::AdTypeMax; ++j)
      {

         for(auto pair : m_datas[i][j])
         {
            std::cout << " ";
            std::cout << pair.first << ";" << pair.second;
         }
         std::cout << "| ";
      }
      std::cout << "\n";
   }
}

}
}
