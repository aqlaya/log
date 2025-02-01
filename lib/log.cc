#include "log.hpp"

namespace logging { 

logging::Log::Log(fs::path &&namefile, logging::status status ) { 
	if (namefile.has_filename() && fs::exists(namefile) &&
	check_is_owner_write(namefile)) {
	    this->stat = status;
        this->file = std::fstream(std::move(namefile), std::ios::app);
    } else if (!fs::exists(namefile)) {
	    throw std::logic_error("File not exists!");
    } else if (!check_is_owner_write(namefile)) {
	    throw std::logic_error("Not permissions write for owner");
    } else {
	    throw std::logic_error("Not filename path");
    }


void logging::Log::note_to_logfile(const logging::UnitLog& obj ) const {
    if (obj.status < stat) return;
    file.exceptions(std::fstream::failbit);
    try {
	    file << convert_time_point_to_str(obj.time) << " ["
	        << convert_status_to_str(obj.status) << "] " << obj.message << '\n';
        file.flush();
    } catch (std::ios_base::failure &exception) {
        DEBUG_LOG_INFO(exception.what());
	    throw;
    }
    
}

void logging::Log::note_to_logfile(std::string&& mess, 
                                    logging::status stat, 
                                        const sys_clock::time_point& time) const 
{
    logging::UnitLog unitlog(std::move(mess), time, stat);
    note_to_logfile(unitlog);
}

std::string logging::Log::convert_status_to_str(logging::status status) {
    switch (status) {
        case logging::status::NOTICE:
	        return std::string("NOTICE");
        case logging::status::WARNING:
	        return std::string("WARNING");
        case logging::status::ERROR:
	        return std::string("ERROR");
        case logging::status::CRITICAL:
	        return std::string("CRITICAL");
    }
    return std::string("WARNING");
}
}
