#include <config.h>

const char* gethome(){
    const char* home = std::getenv("HOME");
    return home;
}

void create_parent_dir(std::string path){
    std::filesystem::path parent_dir = std::filesystem::path(path).parent_path();
    if(!std::filesystem::exists(path)){
        std::cout<<"Creating parent directory at: "<<path<<std::endl;
        std::filesystem::create_directories(path);
    }
}
void config::read_config_file(std::string config_path){
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
        if (eq == std::string::npos){
            //do nothing
            continue;
        }
        std::string key = line.substr(0,eq);
        std::string value = line.substr(eq+1);
        config_parse[key] = value;
    }
    
    //assinging the values in class, from the unordered map
    struct ColorConfig colors; 
    for(const auto& [key,value] : config_parse){
        if(key == "PATH"){
            this->PATH = value;
        }
        else if(key == "BACKUP_PATH"){
            this->BACKUP_PATH = value;
        }
        else if(key == "id_color"){
            colors.id_color = value;
        }
        else if(key == "tag_color"){
            colors.tag_color = value;
        }
        else if(key == "timestamp_color"){
            colors.timestamp_color = value;
        }
        else if(key == "Text_color"){
            colors.text_color = value;
        }
    }
}


void default_config_file(std::string config_path, const char* xdg_data){
    //creating the file with default values, since it doesn't exist
        std::cout<<"Your journal couldn't find where to store your secrets. Due to the lack of choice, storing it in a default location -_-"
            <<std::endl;
        std::ofstream outfile(config_path);
        
        std::string data_path;
        if(!outfile.is_open()){
            throw std::runtime_error("And.... yeah, your journals config file coudn't be opened at "+std::string(config_path));
        }
        if(xdg_data == NULL){
            data_path=std::string(gethome())+"/.local/share/jrnl/journal.txt";
        }
        else{
            data_path=std::string(xdg_data)+"/jrnl/journal.txt";
        }
        outfile<<"#Default path for storing journal.txt file"<<std::endl;
        outfile<<"PATH$"+data_path<<std::endl;

        std::string backup_path;
        if(xdg_data == NULL){
            backup_path = std::string(gethome())+"/.local/share/jrnl/backup";
        }
        else{
            backup_path = std::string(xdg_data)+"/jrnl/backup";
        }
        outfile<<"#Default directory path for storing backups"<<std::endl;
        outfile<<"BACKUP_PATH$" + backup_path<<std::endl;
}

config::config(){

    std::string config_path;
    //looking for the enviroment paths
    const char* home = gethome();
    const char* xdg_config=std::getenv("XDG_CONFIG_HOME");
    const char* xdg_data=std::getenv("XDG_DATA_HOME");

    if(xdg_config == nullptr && home == nullptr){
        //bailing out if there's no home
        throw std::runtime_error("Mate, your system is so weird, it's HOME-less");
    }
    else if( xdg_config != nullptr){
        //if xdg_config is set, using it
        config_path = std::string(xdg_config)+"/jrnl/jrnl.txt";
    }
    else {    
        //using home if xdg_config isn't set up
        config_path = std::string(home)+"/.config/jrnl/jrnl.txt";
    }

    //checking if the config directory exists
    create_parent_dir(config_path);

    if(!std::filesystem::exists(config_path)){
        default_config_file(config_path, xdg_data);
    }
    //reading and assinging class variables from config  file
    read_config_file(config_path);
    //checking if the journal target directory exists, creating it not
    create_parent_dir(PATH);

    //creating journal file if it doesn't exist
    if(!std::filesystem::exists(PATH)){
        std::ofstream jrnlfile(PATH);
        if(!jrnlfile.is_open()){
            throw std::runtime_error("The vault for your secrets couldn't be created. Maybe today, the Universe doesn't want you journalling"+PATH);
        }
    }
}
