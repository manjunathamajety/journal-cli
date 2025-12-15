#include <jrnlmanager.h>
#include <time.h>

//passing jrnl_manager by reference 'const &'
static bool write_all(int fd, std::vector<jrnl>& jrnl_manager){
    for(int i=0; i<jrnl_manager.size();i++){
        jrnl& entry=jrnl_manager[i]; 
        std::string entry_string=std::to_string(entry.getid())+";"+entry.gettag()+";"+std::to_string(entry.getstamp())+";"+entry.getentry()+"\n";
        
        ssize_t bytes_written = 0; 
        std::size_t entry_size = entry_string.size();

        while(bytes_written < entry_size){
            ssize_t n = ::write(fd,entry_string.data()+bytes_written,entry_string.size()-bytes_written);
            if(n == -1){
                int e = errno;
                std::cerr<<"jrnl: failed to write the entry "<<entry.getid()<<" "<<std::strerror(e)<<"(errno:"<<e<<")\n";
                return false;
            }   
            if(n == 0){
                int e = errno;
                std::cerr<<"jrnl: bytes written 0, there's something wrong with the file system"<<entry.getid()<<" "<<std::strerror(e)<<"(errno:"<<e<<")\n";
                return false;
            }
            else{
                bytes_written += static_cast<std::size_t>(n);
            }


        }
    }
    return true;

}

void Manager::backup(std::string backup_path){
    bool write_ok;
    //Using POSIX functions to implement atomic saves. 
    int file = open(backup_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(file==-1){
        int e=errno;
        std::cerr<<"Failed to open backup file "<<std::strerror(e)<<"(errorno: "<<e<<")";
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
    
    //fsync the parent directory for extra verification;
    std::filesystem::path parent_dir=std::filesystem::path(backup_path).parent_path();
    int dir = open(parent_dir.c_str(), O_RDONLY | O_DIRECTORY);
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
