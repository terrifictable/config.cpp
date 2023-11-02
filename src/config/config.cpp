#include "config.h"

namespace config {
    #define $(t)                (vb_ptr->type == typeid(t)) 
    #define CAST_TYPE(t)        config::value<t>* v_ptr = (config::value<t>*)vb_ptr
    

    void save_to_node(yaml::Node& node, std::vector<config::value_base*> values) {
        for (config::value_base* vb_ptr : values) {                
            #define ASSIGN_TO_STR()     node[v_ptr->alias] = std::to_string(*v_ptr->ptr) 
            #define ASSIGN_DEFAULT(t)   if $(t) { \
                                            CAST_TYPE(t); \
                                            ASSIGN_TO_STR(); \
                                        }

            ASSIGN_DEFAULT(int); 
            ASSIGN_DEFAULT(short); 
            ASSIGN_DEFAULT(double);
            ASSIGN_DEFAULT(float); 
            ASSIGN_DEFAULT(long); 
            ASSIGN_DEFAULT(long long); 

            ASSIGN_DEFAULT(bool);
            
            ASSIGN_DEFAULT(char); 
            if ($(char*) || $(std::string)) { CAST_TYPE(char*); 
                node[v_ptr->alias] = *v_ptr->ptr; 
            } 
            
            if ($(void*)) { CAST_TYPE(void*);
                std::stringstream sst;
                sst << std::hex << *v_ptr->ptr;
                node[v_ptr->alias] = sst.str();
            } 



            /**    ADD OTHER TYPES HERE   **/


            #undef ASSIGN_TO_STR
            #undef ASSIGN_DEFAULT
        }
    }

    
    result Config::init(std::string config_file, std::vector<config::value_base*> d) {
        this->values = d;

        this->file = fs::absolute(config_file).string();
        if (!fs::exists(file)) {
            dbg("No Config file, creating one using default configs");
            
            std::ofstream cf(this->file);
            if (cf.is_open()) {
                yaml::Node n;
                save_to_node(n, this->values);

                std::string str;
                yaml::Serialize(n, str);
                cf << str << std::endl;
                cf.close();
            } else {
                err("std::ofstream failed to open file");
            }

            return RESULT(no_config, "Creating default config");
        }

        return load();
    }

    result Config::load() {
        try {
            yaml::Parse(this->node, this->file.c_str()); // files = char*; string = std::string;
        } catch (std::exception& e) {
            return RESULT(parse_error, e.what());
        }

        for (config::value_base* vb_ptr : this->values) {                
            #define ASSIGN_TYPE(t)      *v_ptr->ptr = this->node[v_ptr->alias].As<t>()
            #define ASSIGN_DEFAULT(t)   if $(t) { CAST_TYPE(t); ASSIGN_TYPE(t); }

            ASSIGN_DEFAULT(int); 
            ASSIGN_DEFAULT(short); 
            ASSIGN_DEFAULT(double);
            ASSIGN_DEFAULT(float); 
            ASSIGN_DEFAULT(long); 
            ASSIGN_DEFAULT(long long); 

            ASSIGN_DEFAULT(bool);
            
            ASSIGN_DEFAULT(char); 
            if ($(char*)) { CAST_TYPE(char*);
                *v_ptr->ptr = const_cast<char*>(this->node[v_ptr->alias].As<std::string>().c_str());
            }
            ASSIGN_DEFAULT(std::string);

            ASSIGN_DEFAULT(void*);


            /**   ADD OTHER TYPES HERE   **/



            #undef ASSIGN_TO_STR
            #undef ASSIGN_DEFAULT
        }

        return RESULT_OK;
    }

    result Config::clear() {
        this->node = yaml::Node();
        this->node.Clear();

        return RESULT_OK;
    }

    result Config::reload() {
        result res = this->clear();
        if (res.status != status::ok)
            return res;
        return this->load();
    }

    result Config::save() {
        save_to_node(this->node, this->values);

        std::string str;
        yaml::Serialize(this->node, str);

        std::ofstream cf(this->file);
        cf << str << std::endl;
        cf.close();

        return RESULT_OK;
    }
    
    #undef $
    #undef CAST_TYPE
};
