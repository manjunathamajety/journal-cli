#include <config.h>

config::config(){

    std::string config_path;
    //looking for the enviroment paths
    const char* home=std::getenv("HOME");
    const char* xdg_config=std::getenv("XDG_CONFIG_HOME");
    const char* xdg_data=std::getenv("XDG_DATA_HOME");

    //if XDG_CONFIG_HOME is not set, falling back to env HOME/.config
    if(xdg_config == NULL){
        config_path = std::string(home)+"/.config/jrnl/jrnl.txt";
    }
    else{
        config_path=std::string(xdg_config)+"/jrnl/jrnl.txt";
    }

    //checking if the config directory exists
    std::filesystem::path config_dir = std::filesystem::path(config_path).parent_path();
    if(!std::filesystem::exists(config_dir)){
        std::cout<<"Creating config file directory at ", std::string(config_dir);
        std::filesystem::create_directories(config_dir);
    }

    //creating the file with default values, if it doesn't exist
    if(!std::filesystem::exists(config_path)){
        std::cout<<"\nConfig file doesn't exist, initalizing with defaults";
        std::ofstream outfile(config_path);
        
        std::string data_path;
        if(!outfile.is_open()){
            throw std::runtime_error("Failed to open config file:"+config_path);
        }
        if(xdg_data == NULL){
            data_path=std::string(home)+"/.local/share/jrnl/journal.txt";
        }
        else{
            data_path=std::string(xdg_data)+"/jrnl/journal.txt";
        }
        outfile<<"PATH$"+data_path;
    }

    //reading the config file from /.config/
    std::string identifier;
    std::ifstream file(config_path);
    std::getline(file,identifier,'$');
    std::getline(file,PATH);
    //PATH=std::string(home)+PATH;
    
    //checking if the journal target directory exists, creating it not
    std::filesystem::path jrnl_dir = std::filesystem::path(PATH).parent_path();
    if(!std::filesystem::exists(jrnl_dir)){
        std::cout<<"Creating journal file directory at", std::string(jrnl_dir);
        std::filesystem::create_directories(jrnl_dir);
    }

    //creating journal file if it doesn't exist
    if(!std::filesystem::exists(PATH)){
        std::ofstream jrnlfile(PATH);
        if(!jrnlfile.is_open()){
            throw std::runtime_error("Failed to create journal file:"+PATH);
        }
    }
}
