#pragma once
#ifndef CONFIG_H

#include <filesystem>
#include <fstream>
#include <vector>

#include <mini_yaml/yaml.hpp>
#include "common.h"

namespace config {
    namespace fs = std::filesystem;

    typedef enum {
        ok = 0,
        no_config = 1,
        parse_error = 2,
        file_not_found = 3,
        other = 100,
    } status;

    typedef struct {
        status status;
        std::string msg;
    } result;
    #define RESULT_OK result{.status = ok, .msg = ""}
    #define RESULT(stat, message) result{.status = stat, .msg = message}


    struct value_base {
        std::string alias;
        const std::type_info& type;

        value_base(std::string a, const std::type_info& t) : alias(a), type(t) {}
    };

    template<typename T>
    struct value : value_base {
        T* ptr;

        value(std::string a, T* p) : value_base(a, typeid(T)), ptr(p) {}
    };



    class Config {
    private:
        std::string file;
        std::vector<config::value_base*> values;

        result load();
        result clear();

    public:
        yaml::Node node;


        Config() {}
        ~Config() {}

        result init(std::string file, std::vector<config::value_base*> defaults);
        result reload();
        result save();
    };
};

#endif // !CONFIG_H
