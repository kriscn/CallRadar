#pragma once

#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging
#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging
//std::shared_ptr<spdlog::logger> log4c;
extern std::shared_ptr<spdlog::logger> log4c;  // ÉùÃ÷ log4c

inline void initLogger()
{
    log4c = spdlog::basic_logger_mt("file_logger", "logs/basic-log.txt");
    log4c->set_level(spdlog::level::debug);
    log4c->flush_on(spdlog::level::trace);
}