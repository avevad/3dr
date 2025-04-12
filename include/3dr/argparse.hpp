#pragma once
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

namespace tdr {

    class ArgParser {
    public:
        using KeyArgHandlerSignature = void(const std::string &arg, const std::string *&next, const std::string *end);
        using FreeArgHandlerSignature = void(const std::string &arg);

        ArgParser() = default;

        void register_keys(const std::vector<std::string> &keys, const std::function<KeyArgHandlerSignature> &handler);
        void register_free(const std::function<FreeArgHandlerSignature>& handler);

        void operator()(int argc, char **argv) const;

    private:
        std::unordered_map<std::string, std::function<KeyArgHandlerSignature>> key_handlers;
        std::optional<std::function<FreeArgHandlerSignature>> free_handler;

    };

}
