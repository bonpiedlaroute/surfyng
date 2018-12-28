/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_
#include <string>
#include <unordered_map>


namespace surfyn
{
namespace utils
{

class Config
{

public:
   Config(const std::string& configfilename);
   void loadconfig();
   int getIntValue(const std::string& key) const;
   std::string getStringValue(const std::string& key) const;
   double getDoubleValue(const std::string& key) const;
   long getLongValue(const std::string& key) const;

private:
   std::unordered_map<std::string, std::string> m_configvalues;
   std::string m_configfilename;
};

}
}
#endif /* INC_CONFIG_H_ */
