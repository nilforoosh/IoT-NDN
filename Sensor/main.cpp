#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]){
    srand(time(NULL));

    while(true){
        double temp = ((double)rand()/RAND_MAX)*2 + 23;
        std::string name = "/Senario/Weather/Sample/MyArea/Sensors/";
        name.append(argv[1]);
        name.append("/");
        name.append(std::to_string(temp));

        std::cout << "\nSending MyArea temperatur to the gateway with an Interest:" << std::endl;
        std::cout << name << std::endl;

        std::string command = "ndnpeek -pf ndn:" + name;
        system(command.c_str());
        sleep(5);
    }

    return 0;
}