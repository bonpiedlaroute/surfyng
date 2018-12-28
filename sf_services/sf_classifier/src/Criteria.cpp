/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "Criteria.h"
#include "CriteriaManager.h"


namespace surfyn
{
namespace classifier
{

const char* RealEstatePrice = "PRICE";
const char* RealEstateSurface = "SURFACE";
const char* RealEstateConstructionYear = "CONSTRUCTION_YEAR";
const char* RealEstateTimeToPublicTransport = "TIME_TO_PUBLICTRANSPORT";
const char* RealEstateBeds = "BEDS";
const char* RealEstateCity = "CITY";
const char* RealEstateType = "PROPERTY_TYPE";
const char* RealEstateRooms = "ROOMS";
const char* RealEstateTypeOfHeating = "TYPE_OF_HEATING";
const char* RealEstateCellars = "CELLARS";
const char* RealEstateFloor = "FLOOR";
const char* RealEstateParking = "PARKING";
const char* RealEstateKey = "ID";
const char* RealEstateSimilarAd = "SIMILAR_AD";


Criteria::Criteria(CriteriaID::Value id) : m_id(std::move(id)), m_critOKAndSameAd(0),
      m_critNOKAndSameAd(0), m_critOK(0), m_critNOK(0), m_probaCritOKKnowingThatSameAd(0),
      m_probaCritNOKKnowingThatSameAd(0),m_probaCritOK(0), m_probaCritNOK(0)
{

}
CriteriaID::Value Criteria::getId() const
{
   return m_id;
}
double  Criteria::getProbaCritOKKnowingThatSameProperty() const
{
   return m_probaCritOKKnowingThatSameAd;
}
double  Criteria::getProbaCritNOKKnowingThatSameProperty() const
{
   return m_probaCritNOKKnowingThatSameAd;
}

double  Criteria::getProbaCritOK() const
{
   return m_probaCritOK;
}
double  Criteria::getProbaCritNOK() const
{
   return m_probaCritNOK;
}

void Criteria::processSameProperties(const RealEstateAd& p1, const RealEstateAd& p2)
{
   if( isValid(p1) && isValid(p2) )
   {
      if( isEquals(p1, p2) )
      {
         m_critOKAndSameAd++;
      }

   }
}

void Criteria::process(const RealEstateAd& p1, const RealEstateAd& p2)
{
   if( isValid(p1) && isValid(p2) )
   {
      if( isEquals(p1, p2) )
      {
         m_critOK++;
      }
      else
      {
         m_critNOK++;
      }
   }
}

void Criteria::computeProbabilities()
{
   m_probaCritOKKnowingThatSameAd = static_cast<double>(m_critOKAndSameAd) / static_cast<double>(CriteriaManager::getNbPairsOfSimilarProperty());

   if(m_critNOKAndSameAd != 0 )
      m_probaCritNOKKnowingThatSameAd = static_cast<double>(m_critNOKAndSameAd) / static_cast<double>(CriteriaManager::getNbPairsOfSimilarProperty());
   else
   {
      m_probaCritNOKKnowingThatSameAd = 1- m_probaCritOKKnowingThatSameAd;
   }

   m_probaCritOK = static_cast<double>(m_critOK) / static_cast<double>(CriteriaManager::getNbPairsOfProperty());

   if(m_critNOK != 0)
      m_probaCritNOK = static_cast<double>(m_critNOK) / static_cast<double>(CriteriaManager::getNbPairsOfProperty());
   else
   {
      m_probaCritNOK = 1 - m_probaCritOK;
   }
}

Criteria::~Criteria()
{

}

CriteriaPrice::CriteriaPrice() : Criteria(CriteriaID::Value::Price)
{

}

bool CriteriaPrice::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstatePrice) == p2.getDescription(RealEstatePrice) ;
}

bool CriteriaPrice::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstatePrice) != "";
}

CriteriaSurface::CriteriaSurface() : Criteria(CriteriaID::Value::Surface)
{

}

bool CriteriaSurface::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateSurface) == p2.getDescription(RealEstateSurface) ;
}

bool CriteriaSurface::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateSurface) != "";
}

CriteriaConstructionYear::CriteriaConstructionYear() : Criteria(CriteriaID::Value::ConstructionYear)
{

}

bool CriteriaConstructionYear::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateConstructionYear) == p2.getDescription(RealEstateConstructionYear) ;
}

bool CriteriaConstructionYear::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateConstructionYear) != "";
}

CriteriaTimeToPublicTransport::CriteriaTimeToPublicTransport() : Criteria(CriteriaID::Value::TimeToPublicTransport)
{

}

bool CriteriaTimeToPublicTransport::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateTimeToPublicTransport) == p2.getDescription(RealEstateTimeToPublicTransport) ;
}

bool CriteriaTimeToPublicTransport::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateTimeToPublicTransport) != "";
}

CriteriaBeds::CriteriaBeds() : Criteria(CriteriaID::Value::Beds)
{

}

bool CriteriaBeds::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateBeds) == p2.getDescription(RealEstateBeds) ;
}

bool CriteriaBeds::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateBeds) != "";
}

CriteriaCity::CriteriaCity() : Criteria(CriteriaID::Value::City)
{

}

bool CriteriaCity::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateCity) == p2.getDescription(RealEstateCity) ;
}

bool CriteriaCity::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateCity) != "";
}

CriteriaPropertyType::CriteriaPropertyType() : Criteria(CriteriaID::Value::PropertyType)
{

}

bool CriteriaPropertyType::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateType) == p2.getDescription(RealEstateType) ;
}

bool CriteriaPropertyType::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateType) != "";
}

CriteriaRooms::CriteriaRooms() : Criteria(CriteriaID::Value::Rooms)
{

}

bool CriteriaRooms::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateRooms) == p2.getDescription(RealEstateRooms) ;
}

bool CriteriaRooms::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateRooms) != "";
}

CriteriaTypeOfHeating::CriteriaTypeOfHeating() : Criteria(CriteriaID::Value::TypeOfHeating)
{

}

bool CriteriaTypeOfHeating::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateTypeOfHeating) == p2.getDescription(RealEstateTypeOfHeating) ;
}

bool CriteriaTypeOfHeating::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateTypeOfHeating) != "";
}

CriteriaCellars::CriteriaCellars() : Criteria(CriteriaID::Value::Cellars)
{

}

bool CriteriaCellars::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateCellars) == p2.getDescription(RealEstateCellars) ;
}

bool CriteriaCellars::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateCellars) != "";
}

CriteriaFloor::CriteriaFloor() : Criteria(CriteriaID::Value::Floor)
{

}

bool CriteriaFloor::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateFloor) == p2.getDescription(RealEstateFloor) ;
}

bool CriteriaFloor::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateFloor) != "";
}

CriteriaParking::CriteriaParking() : Criteria(CriteriaID::Value::Parking)
{

}

bool CriteriaParking::isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const
{
   return p1.getDescription(RealEstateParking) == p2.getDescription(RealEstateParking) ;
}

bool CriteriaParking::isValid(const RealEstateAd& p) const
{
   return p.getDescription(RealEstateParking) != "";
}

}
}
