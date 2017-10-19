#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]){
    srand(time(NULL));

    while(true){

        std::string name = "/Senario/Meteorology/Temperature/MyArea";
        

        std::cout << "\nSending request to meteorology for temperatur of MyArea" << std::endl;
        //std::cout << name << std::endl;

        std::string command = "ndnpeek -pf ndn:" + name;
        system(command.c_str());
        sleep(7);
    }

    return 0;
}
