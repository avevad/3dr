#pragma once
#include <source_location>
#include <string>

namespace tdr {

    [[noreturn]] void assertion_failed(
        const std::string &what, std::source_location where = std::source_location::current()
    );

    void check_that(
        bool condition, const std::string &what, std::source_location where = std::source_location::current()
    );

#define TDR_DEBUG if (TDR_DEBUG_BUILD)
#define TDR_ASSERT(WHAT) do { TDR_DEBUG check_that(WHAT, #WHAT); } while (0)
}
