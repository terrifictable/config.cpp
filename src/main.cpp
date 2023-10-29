#include <cstdio>
#include <cstdint>
#include <cstdlib>

#include <typeinfo>
#include <string>
#include <vector>

#include <mini_yaml/yaml.hpp>

#include "config/config.h"
#include "common.h"


int x = 2;
double y = 3.14;
std::string str = "default";
int main(unused int argc, unused const char** argv) {
    std::vector<config::value_base*> defaults;
    defaults.push_back(new config::value<int>("int", &x));
    defaults.push_back(new config::value<double>("double", &y));
    defaults.push_back(new config::value<std::string>("str", &str));

    config::Config conf;
    config::result res = conf.init("config.yml", defaults);
    if (res.status != config::status::ok && res.status != config::status::no_config) {
        err("Failed to initialize config, error: %s", res.msg.c_str());
        return 1;
    }

    info("x: %d", x);
    info("y: %f", y);
    info("str: %s", str.c_str());

    if (conf.save().status != config::status::ok) {
        err("Failed to save config, error: %s", res.msg.c_str());
        return 2;
    }

    return 0;
}
