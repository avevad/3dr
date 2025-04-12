#include "3dr/util.hpp"

#include <iostream>
#include <source_location>
#include <string>

namespace tdr {
    void assertion_failed(const std::string &what, const std::source_location where) {
        std::cerr <<
            std::string(where.file_name()) + ":" +
            std::to_string(where.line()) + ":" +
            std::to_string(where.column()) + ": "
            "function ‘" + where.function_name() + "’: "
            "assertion failed: " + what << std::endl;
        std::abort();
    }

    void check_that(bool condition, const std::string &what, const std::source_location where) {
        if (!condition) {
            assertion_failed(what, where);
        }
    }
}
