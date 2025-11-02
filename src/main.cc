#include <iostream>
#include <journal.h>
#include <jrnlmanager.h>
#include <config.h>
int main(int argc,char** argv){
      
    if (argc>3){
        std::cout<<"Error! Too many arguments!";
    }
    
        
    if (argc<3){
        std::cout<<"Error! Too few arguments!";
    }
    
   if (argc==3){
        config c1;
        
        manager m1(c1.getpath());

        std::string arg=argv[1];
    
        if (arg=="-a"){
            std::string str=argv[2];
            
            m1.addentry(str);
            m1.save(c1.getpath());
        }

        if (arg=="-l"){
            std::string str=argv[2];
            
            m1.display(str);
            m1.save(c1.getpath());
        }

    }
    
}
