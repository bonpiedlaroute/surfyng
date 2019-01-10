/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/

#pragma warning(suppress : 4996)
#define _CRT_SECURE_NO_WARNINGS

#include "Logger.h"
#include <time.h>
#include <sstream>

#define MAX_DATE_TIME_FORMAT   64



namespace surfyn
{
namespace utils
{


Logger* Logger::m_instance = nullptr;
std::mutex  Logger::m_logger_mutex;

void Logger::writelog()
{
   std::stringstream logfilenamestreamer;
   char filename[MAX_DATE_TIME_FORMAT];
   setDateTimeFormat(filename);

   logfilenamestreamer << filename << ".log";

   m_logfile.open(logfilenamestreamer.str().c_str(), std::ios_base::out );
   while(true)
   {
       std::unique_lock<std::mutex> lck(m_logger_mutex_queue);
       m_logger_cv.wait(lck);

       while(!m_log_messages.empty())
       {
         std::string log;

         log = m_log_messages.front();
         m_log_messages.pop();

         char date_time_format[MAX_DATE_TIME_FORMAT];

         setDateTimeFormat(date_time_format);

         std::stringstream logstreamer;
         logstreamer << date_time_format << " " << log << "\n";

         m_logfile << logstreamer.str() << std::flush;
       }


   }
}

Logger::Logger()
{

}

Logger* Logger::getInstance()
{
   if( m_instance == nullptr )
   {
      std::lock_guard<std::mutex> lock(Logger::m_logger_mutex);

      if( m_instance == nullptr )
      {
            m_instance = new Logger();
            m_instance->m_log_fut = std::async(std::launch::async, &Logger::writelog, m_instance);
      }

   }

   return m_instance;
}

void Logger::info(const std::string& msg)
{
   push_log("[INFO] " + msg);

}
void Logger::warn(const std::string& msg)
{
   push_log("[WARN] " + msg);
}

void Logger::error(const std::string& msg)
{
   push_log("[ERROR] " + msg);
}

Logger::~Logger()
{

}

void Logger::push_log(const std::string& msg)
{
   std::lock_guard<std::mutex> lock(m_logger_mutex_queue);
   m_log_messages.emplace(msg);
   m_logger_cv.notify_one();
}

void Logger::setDateTimeFormat(char * datetimeformat) const
{
   time_t now = time(NULL);
   struct tm* localinfo = localtime(&now);

   strftime(datetimeformat,MAX_DATE_TIME_FORMAT, "%Y-%m-%d-%H-%M-%S", localinfo);
}

}
}
