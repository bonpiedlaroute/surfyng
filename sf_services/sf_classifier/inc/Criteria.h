/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_CRITERIA_H_
#define INC_CRITERIA_H_
#include "RealEstateAd.h"

namespace surfyn
{
namespace classifier
{

extern const char* RealEstatePrice;
extern const char* RealEstateSurface;
extern const char* RealEstateConstructionYear;
extern const char* RealEstateTimeToPublicTransport;
extern const char* RealEstateBeds;
extern const char* RealEstateCity;
extern const char* RealEstateType;
extern const char* RealEstateRooms;
extern const char* RealEstateTypeOfHeating;
extern const char* RealEstateCellar;
extern const char* RealEstateFloor;
extern const char* RealEstateParking;
extern const char* RealEstateBox;
extern const char* RealEstateSearchType;
extern const char* RealEstateLandSurface;
extern const char* RealEstateLift;
extern const char* RealEstateBalcony;
extern const char* RealEstateKey;
extern const char* RealEstateSimilarAd;

struct CriteriaID
{
   enum Value
   {
      Price,
      Surface,
      ConstructionYear,
      TimeToPublicTransport,
      Beds,
      City,
      PropertyType,
      Rooms,
      TypeOfHeating,
      Cellars,
      Floor,
      Parking,
      Box,
      SearchType,
      LandSurface,
      Lift,
      Balcony,
      CriteriaMax
   };
};

class Criteria
{
public:
   Criteria(CriteriaID::Value id);
   virtual bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const = 0;
   void processSameProperties(const RealEstateAd& p1, const RealEstateAd& p2);
   void process(const RealEstateAd& p1, const RealEstateAd& p2);
   double  getProbaCritOKKnowingThatSameProperty() const;
   double  getProbaCritNOKKnowingThatSameProperty() const;
   double  getProbaCritOK() const;
   double  getProbaCritNOK() const;
   CriteriaID::Value getId() const;
   virtual bool    isValid(const RealEstateAd& p) const = 0;

   void    computeProbabilities();

   virtual ~Criteria();

   int64_t  m_critOKAndSameAd;
   int64_t  m_critNOKAndSameAd;
   int64_t  m_critOK;
   int64_t  m_critNOK;
protected:
   CriteriaID::Value  m_id;

   double   m_probaCritOKKnowingThatSameAd;
   double   m_probaCritNOKKnowingThatSameAd;
   double   m_probaCritOK;
   double   m_probaCritNOK;
};

class CriteriaPrice : public Criteria
{
public:
 CriteriaPrice();
 bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
 bool isValid(const RealEstateAd& p) const override;
};

class CriteriaSurface : public Criteria
{
public:
   CriteriaSurface();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaConstructionYear : public Criteria
{
public:
   CriteriaConstructionYear();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaTimeToPublicTransport : public Criteria
{
public:
   CriteriaTimeToPublicTransport();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaBeds : public Criteria
{
public:
   CriteriaBeds();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};


class CriteriaCity : public Criteria
{
public:
   CriteriaCity();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaPropertyType : public Criteria
{
public:
   CriteriaPropertyType();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaRooms : public Criteria
{
public:
   CriteriaRooms();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaTypeOfHeating : public Criteria
{
public:
   CriteriaTypeOfHeating();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaCellars : public Criteria
{
public:
   CriteriaCellars();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaFloor : public Criteria
{
public:
   CriteriaFloor();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaParking : public Criteria
{
public:
   CriteriaParking();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaBox : public Criteria
{
public:
   CriteriaBox();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaSearchType : public Criteria
{
public:
   CriteriaSearchType();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaLandSurface : public Criteria
{
public:
   CriteriaLandSurface();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaLift : public Criteria
{
public:
   CriteriaLift();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

class CriteriaBalcony : public Criteria
{
public:
   CriteriaBalcony();
   bool isEquals(const RealEstateAd& p1, const RealEstateAd& p2) const override;
   bool isValid(const RealEstateAd& p) const override;
};

}
}

#endif /* INC_CRITERIA_H_ */
