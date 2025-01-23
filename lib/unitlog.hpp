#pragma once

#include <chrono>
#include <ctime>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>
#include <cstring>


#include <unordered_map>

namespace logging {

  namespace fs = std::filesystem;
  namespace ch = std::chrono;

  using sys_clock = ch::system_clock; 
  // для дебага 
  inline void DEBUG_LOG_INFO(const std::string &str) {
  #ifdef DEBUG
    std::cerr << "[DEBUG] " << " " << str << std::endl;
  #endif
  }
  // статус логов 
  enum class status { NOTICE, WARNING, ERROR, CRITICAL };
  
  // хеш-таблица для упрощение получения статуса 
  std::unordered_map<std::string, logging::status> table_status = {
    {"NOTICE", logging::status::NOTICE}, 
    {"WARNING", logging::status::WARNING}, 
    {"ERROR", logging::status::ERROR}, 
    {"CRITICAL", logging::status::CRITICAL}
  };
  // структура единицы логирования
  struct UnitLog {
      std::string message;
      std::time_t time;     
      logging::status status;

      UnitLog() = default;

      inline UnitLog(std::string&& message, std::time_t time, std::string status = "NOTICE") try
        : message(std::move(message))
        , time(time)
        , status(table_status.at(status)) // прилетает ошибка при несуществующем ключе
      {
      } catch (...) {
        DEBUG_LOG_INFO("Error in constructor unit_log");
        throw;
      }
      inline UnitLog(std::string&& message, ch::system_clock::time_point time, logging::status status) 
        : message(std::move(message))
        , time(ch::system_clock::to_time_t(time))
        , status(status) 
      {
      }
      inline UnitLog(std::string&& message, ch::system_clock::time_point time, std::string status = "NOTICE")
        : UnitLog(std::move(message), sys_clock::to_time_t(time), status)
      {
      }
      inline UnitLog(UnitLog&& obj)
        : message(std::move(obj.message)) 
        , time(time)
        , status(obj.status)
      {
      }
      inline UnitLog& operator=(UnitLog&& obj)
      {
        message = std::move(obj.message);
        time = obj.time;
        status  = obj.status;
        return *this;
      } 

      // Удаляем copy-constructor, так как копировать неоптимально 
      UnitLog(const UnitLog& ) = delete;
      UnitLog& operator=(const UnitLog&) = delete; 
  };
}