#include <iostream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>
#include <stdio.h>

using namespace std;

class conchShell {
private:

public: 
    //Set the current shell greeting
    string greeting = get_current_dir_name() + "$";
    //Store the working command
    string command;

    //Run the Loop
    void execute();

    //Parse and execute the command
    void parse_and_execute(string c);

};