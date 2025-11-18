#include <jrnlmanager.h>

manager::manager(std::string PATH){
    
    std::ifstream file(PATH);
    //Initializes jrnl-objects in the jrnl_manager vector.
    std::string id,tag,stamp,txt;
    //while loops checks for if the entry can be compleltely scanned
    while (
        std::getline(file,id,';')&&
        std::getline(file,tag,';')&&
        std::getline(file,stamp,';')&&
        std::getline(file,txt)){
                
        long long times=std::stoll(stamp);
        time_t t=static_cast<time_t>(times);
        //pushes the entry to the last of the vector jrnl_manager
        jrnl_manager.emplace_back(std::stoi(id),tag,t,txt);
        
    }
    file.close();
    if(jrnl_manager.empty()){
        id_count=0;
    }
    else{
        id_count=jrnl_manager.back().getid();
    }
}

void manager::addentry(std::string txt,std::string tag){
    jrnl_manager.emplace_back(id_count+1,tag,timestamp(),txt);
}

bool write_entry(int fd, std::string entry){    
    int bytes_written = ::write(fd,entry.data(),entry.size());
    int entry_size = entry.size();
    if(bytes_written != entry.size()){
        if(bytes_written == 0){
            return false;
        }
        else if(bytes_written == -1){
            return false;
        }
        else if(bytes_written == entry_size){
            return true;
        }
        else{
            while(bytes_written != entry_size){
                int partial_write = ::write(fd,entry.data()+bytes_written,entry.size()-bytes_written);
                bytes_written = bytes_written + partial_write;
            } 
        }
    }
    return true;
}

bool write_all(int fd, std::vector<jrnl> jrnl_manager){
    bool status;
    for(int i=0; i<jrnl_manager.size();i++){
        jrnl& entry=jrnl_manager[i]; 
        std::string entry_string=std::to_string(entry.getid())+";"+entry.gettag()+";"+std::to_string(entry.getstamp())+";"+entry.getentry()+"\n";
        status = write_entry(fd, entry_string);
        
        if(status == false){
            int e = errno;
            std::cerr<<"Couldn't write the entry, [id: "<<entry.getid()<<"]"<<std::strerror(e)<<"(errno: "<<e<<")";
            return false;
        }
    }
    return true;

}
void manager::save(std::string PATH){
    
    bool write_ok;
    //Using POSIX functions to implement atomic saves. 
    std::string temp_path=PATH+".tmp";
    int file=open(temp_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(file==-1){
        int e=errno;
        std::cerr<<"Failed to open tmp file "<<std::strerror(e)<<"(errorno: "<<e<<")";
        return;
    }    
    write_ok = write_all(file, jrnl_manager);

    //fsync on the temporary file to write the data onto the disk
    int sy = ::fsync(file);
    if(sy == -1){
        int e=errno;
        std::cerr<<"Failed to fsync the tmp file "<<std::strerror(e)<<"(errorno: "<<e<<")";
        return;
    }
    //closing the file 
    int cl = ::close(file);
    if(cl == -1){
        int e=errno;
        std::cerr<<"Failed to close tmp file "<<std::strerror(e)<<"(errorno: "<<e<<")";
        return;
    }   
    //renaming and effectively replacing journal.txt with the comletely written .tmp file
    int rn = ::rename(temp_path.c_str(),PATH.c_str());
    if(rn ==-1){
        int e=errno;
        std::cerr<<"Failed to rename tmp file "<<std::strerror(e)<<"(errorno: "<<e<<")";
        return;
    }

    //fsync the parent directory for extra verification;
    std::filesystem::path parent_dir=std::filesystem::path(PATH).parent_path();
    int dir=open(parent_dir.c_str(), O_RDONLY | O_DIRECTORY);
    if(dir == -1){
        int e=errno;
        std::cerr<<"Failed to open the parent directory "<<std::strerror(e)<<"(errorno: "<<e<<")";
        return;
    } 
    int dir_sy = ::fsync(dir);
    //directory fsync is not a huge criteria, so failure in doing so will only log the error.
    if(dir_sy==-1){
        int e=errno;
        std::cerr<<"Failed to fsync the parent directory "<<std::strerror(e)<<"(errorno: "<<e<<")";
    }
    int dir_cl = ::close(dir);
    if(dir_cl == -1){
        int e=errno;
        std::cerr<<"Failed to close the parent directory "<<std::strerror(e)<<"(errorno: "<<e<<")";
        return;
    }
}
