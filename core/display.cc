#include <display.h>


std::string timeconvert(std::string timestamp){
    
    time_t time=std::stoll(timestamp);
    std::tm* localtime=std::localtime(&time);
    
    std::ostringstream oss;
    oss<<std::put_time(localtime, "%Y-%m-%d %H:%M");
    return oss.str();
}



void displayn(int n){
    
    std::ifstream fread("input.txt");
    for(int i=0;i<n;i++){
        std::string timestamp, text;
        std::getline(fread,timestamp,':');
        std::getline(fread,text);
        std::cout<<timeconvert(timestamp)<<" "<<text<<"\n";

    }
    fread.close();

}


void display(std::string num){
    
    std::string text,timestamp;
    std::ifstream file("input.txt");
    if(num=="-all"){
    
        while(std::getline(file,text)){
            std::getline(file,timestamp,':');
            std::getline(file,text);
            std::cout<<timeconvert(timestamp)<<" "<<text<<"\n";

        }
    }
    else{
        int n=std::stoi(num);
        displayn(n);
    }
    

    file.close();

}


