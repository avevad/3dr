#include "3dr/argparse.hpp"

#include <iostream>

#include "3dr/util.hpp"

namespace tdr {

    void ArgParser::register_keys(
        const std::vector<std::string> &keys, const std::function<KeyArgHandlerSignature> &handler
    ) {
        for (const auto &key : keys) {
            TDR_ASSERT(!key_handlers.contains(key));
            key_handlers[key] = handler;
        }
    }

    void ArgParser::register_free(const std::function<FreeArgHandlerSignature> &handler) {
        TDR_ASSERT(!free_handler);
        free_handler = handler;
    }

    void ArgParser::operator()(int argc, char **argv) const {
        std::vector<std::string> args(argc);
        for (std::size_t pos = 0; pos < static_cast<std::size_t>(argc); pos++) {
            args[pos] = argv[pos];
        }

        const auto *next = args.data() + 1, *end = args.data() + argc;
        while (next != end) {
            auto cur = *next++;
            if (cur.starts_with('-')) {
                handler_key_argument(argv, cur, next, end);
            } else {
                handle_free_argument(argv, cur);
            }
        }
    }

    void ArgParser::handle_free_argument(char **argv, const std::string &arg) const {
        if (free_handler) {
            free_handler(arg);
        } else {
            std::cerr << argv[0] << ": free arguments are not accepted" << std::endl;
            std::exit(1);
        }
    }

    void ArgParser::handler_key_argument(
        char **argv, const std::string &arg, const std::string *&next, const std::string *end
    ) const {
        auto iter = key_handlers.find(arg);
        if (iter != key_handlers.end()) {
            auto handler = iter->second;
            handler(arg, next, end);
        } else {
            std::cerr << argv[0] << ": " << arg << ": unknown key argument" << std::endl;
            std::exit(1);
        }
    }


}
