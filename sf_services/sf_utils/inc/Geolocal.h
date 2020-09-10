/*
 * Created on Sun Sep 06 2020
 *
 * Copyright (c) 2020 Surfyn
 */

#ifndef INC_GEOLOCAL
#define INC_GEOLOCAL

#include <string>
#include <unordered_map>

namespace surfyn
{
namespace utils
{
   struct CityCode
   {
      std::string inseeCode{};
      std::string postalCode{};
   };
   class GeoLocal {
   private:
      std::unordered_map<std::string, CityCode> citiesInfos;

   private:
      void init (const std::string& filename);
   public:
      GeoLocal() = delete;
      GeoLocal(const std::string& csvfile);
      std::string getInseeCode(const std::string& city) const;
      std::string getPostalCode(const std::string& city) const;
      std::string getInseeCodeFromPostalCode(const std::string& postalcode) const;
   };    
}

}


#endif /* INC_GEOLOCAL */
