/*
   (c) copyright 2017
   All rights reserved

   author(s): Noel Tchidjo
*/
#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include <string>
#include <queue>
#include <mutex>
#include <fstream>
#include <future>
#include <condition_variable>

namespace surfyn
{

namespace utils
{

/** @brief  this class creates a thread which will have the job to log messages.
 *          call to function info(), warn(), error(), will push the msg to a queue.
 *          msg will then be handle by the thread and put in log file.
 */
class Logger
{
public:
   static Logger* getInstance();

   void info(const std::string& msg);
   void warn(const std::string& msg);
   void error(const std::string& msg);

private:
   Logger();
   ~Logger();
   void writelog();
   void push_log(const std::string& msg);
   void setDateTimeFormat(char * datetimeformat) const;

private:
   static Logger* m_instance;
   static std::mutex  m_logger_mutex;
   std::queue<std::string> m_log_messages;
   std::mutex m_logger_mutex_queue;
   std::condition_variable m_logger_cv;
   std::future<void> m_log_fut;
   std::fstream      m_logfile;
};

}
}

#endif /* INC_LOGGER_H_ */
