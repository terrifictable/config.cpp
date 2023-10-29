#include "config.h"

namespace config {
    #define IF(t)               if (vb_ptr->type == typeid(t)) 
    #define CAST_TYPE(t)        config::value<t>* v_ptr = (config::value<t>*)vb_ptr
    
    
    result Config::init(std::string config_file, std::vector<config::value_base*> d) {
        this->values = d;

        this->file = fs::absolute(config_file).string();
        if (!fs::exists(file)) {
            dbg("No Config file, creating one using default configs");
            
            std::ofstream cf(this->file);
            if (cf.is_open()) {
                for (config::value_base* vb_ptr : this->values) {                
                    #define WRITE(t)    IF(t) { \
                                            CAST_TYPE(t); \
                                            cf << v_ptr->alias << ": " << *v_ptr->ptr << "\n"; \
                                        }
            
                    WRITE(int); 
                    WRITE(short); 
                    WRITE(double);
                    WRITE(float); 
                    WRITE(long); 
                    WRITE(long long); 

                    WRITE(bool);
                    
                    WRITE(char); 
                    WRITE(char*); 
                    WRITE(std::string); 

                    WRITE(void*); 


                    /**    ADD OTHER TYPES HERE   **/


                }
                
                cf << std::endl;
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
            #define ASSIGN_DEFAULT(t)   IF(t) { \
                                            CAST_TYPE(t); \
                                            ASSIGN_TYPE(t); \
                                        }
            ASSIGN_DEFAULT(int); 
            ASSIGN_DEFAULT(short); 
            ASSIGN_DEFAULT(double);
            ASSIGN_DEFAULT(float); 
            ASSIGN_DEFAULT(long); 
            ASSIGN_DEFAULT(long long); 

            ASSIGN_DEFAULT(bool);
            
            ASSIGN_DEFAULT(char); 
            ASSIGN_DEFAULT(char*);
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
        for (config::value_base* vb_ptr : this->values) {                
            #define ASSIGN_TO_STR()     this->node[v_ptr->alias] = std::to_string(*v_ptr->ptr) 
            #define ASSIGN_DEFAULT(t)   IF(t) { \
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
            IF(char*) { CAST_TYPE(char*); 
                this->node[v_ptr->alias] = *v_ptr->ptr; 
            } 
            IF(std::string) { CAST_TYPE(std::string); 
                this->node[v_ptr->alias] = *v_ptr->ptr; 
            }
            
            IF(void*) {
                CAST_TYPE(void*);
                this->node[v_ptr->alias] = "0x" + std::to_string((long long)*v_ptr->ptr);
            } 



            /**    ADD OTHER TYPES HERE   **/


            #undef ASSIGN_TO_STR
            #undef ASSIGN_DEFAULT
        }


        std::string str;
        yaml::Serialize(this->node, str);

        std::ofstream cf(this->file);
        cf << str << std::endl;
        cf.close();

        return RESULT_OK;
    }
    
    #undef IF
    #undef CAST_TYPE
};
