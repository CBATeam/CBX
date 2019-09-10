/**
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * 
 * SPD logging.
 */

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <memory>

namespace cbx {
    namespace spd_logging {
        extern std::shared_ptr<spdlog::logger> logger;
    
        void init_logging();
    }
}
