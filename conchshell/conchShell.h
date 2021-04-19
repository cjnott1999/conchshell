#include <iostream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem::current_path()
using fs;

class conchShell {
private:

public: 
    //Set the current shell greeting
    string greeting = fs::current_path() + "$";
    //Store the working command
    string command;

    //Run the Loop
    void execute();

    //Parse and execute the command
    void parse_and_execute(string c);

};