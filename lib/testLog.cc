#include <filesystem>
#include <iostream>
#include <string> 
#include <chrono>
#include <ctime>

#include "log.hpp"

int main(int args, char** argv) 
{ 
    using namespace logging;
    {
        auto now = std::chrono::system_clock::now();    
        Log log{"file.txt", logging::status::NOTICE};
        log.note_to_logfile("Don't work your files", logging::status::NOTICE, now);
    }
    {
        auto now = std::chrono::system_clock::now() + std::chrono::microseconds(50);
        Log log("file.txt", logging::status::WARNING);
        log.note_to_logfile("Don't connect your device", logging::status::CRITICAL, now);
        log.note_to_logfile("Don't connect your device", logging::status::NOTICE, now);
    }
    try 
    {
        Log log("/dev/");
        DEBUG_LOG_INFO("WORK DON'T CORRECT TEST 1");
    } 
    catch (std::logic_error& err) {
        DEBUG_LOG_INFO(std::string("WORK CORRECT!") + err.what());
    }
    try 
    {
        Log log("ds", logging::status::ERROR);        
        DEBUG_LOG_INFO("WORK DON'T CORRECT TEST 2");
    }
    catch (std::logic_error& err) 
    {
        DEBUG_LOG_INFO(std::string("WORK CORRECT") + err.what());
    }
}