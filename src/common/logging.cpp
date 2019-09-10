/**
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 *
 * SPD logging.
 */

#include "logging.hpp"

using namespace cbx;

void spd_logging::init_logging() {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%H:%M:%S]-{%l}- %v");

    try {
        logger = spdlog::rotating_logger_mt("logfile", "logs/cbx_dll.log", 1024000, 3);
        logger->flush_on(spdlog::level::debug);
    } catch (const spdlog::spdlog_ex&) {
        spdlog::set_level(spdlog::level::off);
        logger = spdlog::stdout_logger_mt("CBX");
    }
}
