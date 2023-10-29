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

    // TODO: Make this (at least somewhat) work... this is very much retarded and doesnt work with std::vector because the people who made std::vector themselves were very very special and retarded
    struct value_base {
        std::string alias;
        const std::type_info& type;

        value_base(std::string a, const std::type_info& t) : alias(a), type(t) {}
    };
    template<typename T>
    struct value : value_base {
        // T default_val;
        T* ptr;

        value(std::string a, /*T def,*/ T* p) : value_base(a, typeid(T)), ptr(p) {}
    };



    class Config {
    private:
        std::string file;
        yaml::Node node;
        std::vector<config::value_base*> defaults;

        result load();
        result clear();

    public:
        Config() {}
        ~Config() {}

        result init(std::string file, std::vector<config::value_base*> defaults);
        result reload();
        result save();
    };
};

#endif // !CONFIG_H
