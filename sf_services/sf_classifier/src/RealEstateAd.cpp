/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "../inc/RealEstateAd.h"

#include <utility>

namespace surfyn
{
namespace classifier
{

RealEstateAd::RealEstateAd(int64_t id) : m_id(std::move(id))
{

}

void RealEstateAd::setId(int64_t id)
{
   m_id = std::move(id);
}
int64_t RealEstateAd::getId() const
{
   return m_id;
}

void RealEstateAd::setDescription(const std::string& id, const std::string& value)
{
   m_adDescription[id] = value;
}

std::string RealEstateAd::getDescription(const std::string& id) const
{
   auto iter =  m_adDescription.find(id);

   if( iter != m_adDescription.end() )
   {
      return iter->second;
   }
   else
   {
      return "";
   }

}

}
}
