/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/
#include "Config.h"
#include <fstream>
#include <stdlib.h>
#include <sstream>

namespace surfyn
{
namespace utils
{

Config::Config(const std::string& configfilename): m_configfilename(configfilename)
{

}

void Config::loadconfig()
{
   std::ifstream file;
   file.open(m_configfilename.c_str());
   std::string line;

   if( file.is_open() )
   {
      while( std::getline(file, line) )
      {
         if( line[0] != '#' )
         {
            std::istringstream current_line(line);
            std::string key;
            if( std::getline(current_line, key, '='))
            {
               std::string value;
               if( std::getline(current_line, value ) )
               {
                     m_configvalues[key] = value;
               }
            }
         }
      }
   }
   file.close();
}

int Config::getIntValue(const std::string& key) const
{
   return atoi(getStringValue(key).c_str());
}

std::string Config::getStringValue(const std::string& key) const
{
   std::unordered_map<std::string, std::string>::const_iterator iter;
   iter = m_configvalues.find(key);

   if( iter != m_configvalues.end() )
   {
      return iter->second;
   }
   else
   {
      return "";
   }
}

double Config::getDoubleValue(const std::string& key) const
{
   return atof(getStringValue(key).c_str());
}

long Config::getLongValue(const std::string& key) const
{
   return atol(getStringValue(key).c_str());
}

}
}
