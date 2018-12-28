/*
   (c) copyright 2018
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_REALESTATEAD_H_
#define INC_REALESTATEAD_H_
#include <unordered_map>
#include <string>


namespace surfyn
{
namespace classifier
{

using AdPair = std::pair<uint64_t, uint64_t>;

struct AdType
{
   enum Value
   {
      SimilarAd,
      NonSimilarAd,
      AdTypeMax
   };
};


class RealEstateAd
{
public:
   explicit RealEstateAd(int64_t id);
   void setId(int64_t id);
   int64_t getId() const;
   void setDescription(const std::string& id, const std::string& value);
   std::string getDescription(const std::string& id) const;
private:
   int64_t m_id;
   std::unordered_map<std::string, std::string> m_adDescription;
};

}
}
#endif /* INC_REALESTATEAD_H_ */
