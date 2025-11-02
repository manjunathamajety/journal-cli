#include <config.h>

config::config(){
    //acquiring home env in different systems    
    std::string home=std::getenv("HOME");
    std::string config_path= home+"/.config/jrnl/jrnl.txt";
    //reading the config file from /.config/
    
    std::string identifier;
    std::ifstream file(config_path);
    std::getline(file,identifier,'$');
    std::getline(file,PATH);
    PATH=home+PATH;
    file.close();
}
