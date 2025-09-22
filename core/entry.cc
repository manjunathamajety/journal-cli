//This is the implementation file for adding an entry into the journal.
//#include <entry.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <ctime>

inline std::int64_t timestamp(){

    time_t now;
    std::time(&now);
    return now;   

}

int main(){

    long long now;
    now=timestamp();
    std::ofstream file("input.txt",std::ios::app);
    file<<now<<"::"<<"New Wrold"<<"\n";
    file.close();
   


}
