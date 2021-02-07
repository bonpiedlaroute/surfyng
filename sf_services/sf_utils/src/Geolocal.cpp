#include "Geolocal.h"
#include "boost/algorithm/string/split.hpp"
#include <string>
#include <fstream>

namespace surfyn 
{
namespace utils
{
   void GeoLocal::init(const std::string& csvfile)
   {
      std::fstream input;
      input.open(csvfile);

      if( input.is_open())
      {
         std::string line;
         while(std::getline(input, line))
         {
            std::vector<std::string> results;
            boost::split(results, line, [](char c ){ return c == ';';});

            if( results.size() > 0 && results[0] == "Code INSEE")
               continue;

            if(results.size() == 3 )
            citiesInfos.insert(std::make_pair(results[2], CityCode{results[0], results[1]}));
         }
      }
      input.close();
   }
   GeoLocal::GeoLocal(const std::string& csvfile)
   {
      init(csvfile);
   }
   std::string GeoLocal::getInseeCode(const std::string& city) const
   {
      auto it = citiesInfos.find(city);
      if( it != citiesInfos.end())
      {
         return it->second.inseeCode;
      }
      else {
      return "";
      }
   }

   std::string GeoLocal::getPostalCode(const std::string& city) const
   {
      auto it = citiesInfos.find(city);
      if( it != citiesInfos.end())
      {
         return it->second.postalCode;
      }
      else {
      return "";
      }
   }

   std::string GeoLocal::getInseeCodeFromPostalCode(const std::string& postalcode) const
   {
      auto iter = std::find_if(citiesInfos.begin(), citiesInfos.end(), [&postalcode](const std::pair<std::string, CityCode>& elt)
      {
         return elt.second.postalCode == postalcode ;
      }
      );

      return iter != citiesInfos.end() ? iter->second.inseeCode : "";
   }
}
}
