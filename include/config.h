#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <unordered_map>

#ifndef CONFIG_H
#define CONFIG_H

struct ColorConfig{
    std::string id_color;
    std::string tag_color;
    std::string timestamp_color;
    std::string text_color;
};


class config{
    
    private:
        std::string PATH;
        std::string BACKUP_PATH;
        void read_config_file(std::string config_path);
    public:
        config();
        std::string getpath(){return PATH;}
        std::string getbackup(){return BACKUP_PATH;} 
};

#endif 
