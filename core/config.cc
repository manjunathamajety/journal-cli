#include <config.h>

//helper to create parent_directories
static void check_create_parent_dir(const std::string& path){
    std::filesystem::path parent_dir = std::filesystem::path(path).parent_path();
    if(!std::filesystem::exists(parent_dir)){
        std::cout<<"Creating parent directory at: "<<parent_dir<<std::endl;
        std::filesystem::create_directories(parent_dir);
    }
}

//helper to create files
static void check_create_file(const std::string& path){
    std::ofstream out(path,std::ios::app);
    if(!out){
        throw std::runtime_error(std::string("Couldn't create file at ") + path);
    }
}

config::config(){
    //getting the default environments
    const char* home = std::getenv("HOME");
    const char* xdg_config = std::getenv("XDG_CONFIG_HOME");
    const char* xdg_data = std::getenv("XDG_DATA_HOME");

    if(xdg_config == nullptr && home == nullptr){
        //bailing out since there's no damned home
        throw std::runtime_error("Mate, your system is so weird, it's HOME-less");
    }
    else if(xdg_config != nullptr){
        //xdg_config is set, using it
        config_path = std::string(xdg_config)+"/jrnl/jrnl.txt";
    }
    else {
        //using home if xdg_config isn't set
        //and it actually has got home
        config_path = std::string(home)+"/.config/jrnl/jrnl.txt";
    }
    //chekcing for if the directories exist and creating the file if it doesn't exist
    check_create_parent_dir(config_path);

    if(xdg_data == nullptr && home == nullptr){
        //bailing out since there's no damned xdg_data_home and home
        throw std::runtime_error("No way your system has no HOME AND XDG_DATA_HOME set man, I ain't god to work under such work conditions");
    }
    else if(xdg_data != nullptr){
        //using xdg_data for default path location for jrnl file
        PATH = std::string(xdg_data)+"/jrnl/journal.txt";
        BACKUP_PATH = std::string(xdg_data)+"/jrnl/backup";
    }
    else{
        //working with home
        PATH = std::string(home)+"/.local/share/jrnl/journal.txt";
        BACKUP_PATH = std::string(home)+"/.local/share/jrnl/backup";
    }
    check_create_parent_dir(PATH);
    check_create_parent_dir(BACKUP_PATH+"/dummy.txt");
    check_create_file(PATH);
}

//method to check 
void config::initialization(){
    if (!std::filesystem::exists(config_path)){
        std::ofstream outfile(config_path);
        if(!outfile){
            throw std::runtime_error(std::string("Coudn't create file at location ") + config_path);
        }
        else{
            outfile<<"#Default location to store journal.txt"<<"\n";
            outfile<<"PATH$"<<PATH<<"\n";
            outfile<<"#Default location to store backups"<<"\n";
            outfile<<"BACKUP_PATH$"<<BACKUP_PATH<<"\n";
            outfile<<"#Color Configs[256bit ANSI Color Codes]"<<"\n";
            outfile<<"Id$"<<"32"<<"\n";
            outfile<<"Tag$"<<"34"<<"\n";
            outfile<<"Time$"<<"32"<<"\n";
            outfile<<"Text$"<<"37"<<"\n";
            outfile.close();
        }
    }
}


const ColorTemplate& config::parseconfig(){
    
    //reading the config file
    std::string line;
    std::ifstream file(config_path); 
    std::unordered_map<std::string, std::string> config_parse;
    while(std::getline(file,line)){
        if(line.empty() || line[0] == '#'){
            //do nothing
            continue;
        }
        
        auto eq = line.find('$');
        //when the search has failed
        if (eq == std::string::npos){
            //do nothing
            continue;
        }
        std::string key = line.substr(0,eq);
        std::string value = line.substr(eq+1);
        config_parse[key] = value;
    }
    
    //assinging the values in class, from the unordered map
    for(const auto& [key,value] : config_parse){
        if(key == "PATH"){
            PATH = value;
        }
        else if(key == "BACKUP_PATH"){
            BACKUP_PATH = value;
        }
        else if(key == "Id"){
            colors.id_color = value;
        }
        else if(key == "Tag"){
            colors.tag_color = value;
        }
        else if(key == "Time"){
            colors.timestamp_color = value;
        }
        else if(key == "Text"){
            colors.text_color = value;
        }
    }

    return colors;
}
