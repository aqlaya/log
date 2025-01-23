#pragma once

#include "unitlog.hpp"

namespace logging {
  class Log { 
    public:  


      /* инициализация программы (конструктор) */
      Log(fs::path &&namefile, status status = status::NOTICE);
  
      /* запись в лог-файл */
      void note_to_logfile(const UnitLog& obj) const;
      void note_to_logfile(std::string&& message, status status, const ch::time_point<sys_clock>& time) const;
  
      /* изменить стасу дефолта */
      inline void change_default_status(status s) noexcept { stat = s; }
 
      /* удаляем неиспользуемые конструкторы */
      Log(const Log&) = delete;
      Log &operator=(const Log&) = delete;
      Log(Log&&) = delete;
      Log &operator=(Log&&) = delete;
  
    private:
      /* переводить из статуса в строку */ 
      static std::string convert_status_to_str(status status);
  
      inline bool 
      check_is_owner_write(const fs::path &p) const {
  	    return fs::perms::none !=
  	      (fs::status(p).permissions() & fs::perms::owner_write);
      }
  
      inline std::string 
      convert_time_point_to_str(const std::time_t& point_t) const {
        char* clock_p = std::ctime(&point_t); 
        return std::string(clock_p, 0, std::strlen(clock_p) - 1);
      }
  
      /* fields */
      mutable std::fstream file;
      logging::status stat;
  };
}
