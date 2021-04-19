#include <iostream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>

using namespace std;

class conchShell {
private:

public: 

    //Set the current shell greeting
    string greeting = "$ ";
    //Store the working command
    string command;

    //Run the Loop
    void execute();

    //Parse and execute the command
    void parse_and_execute(string c);

};