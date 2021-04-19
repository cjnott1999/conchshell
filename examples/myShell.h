#include <iostream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>

using namespace std;

class myShell {
private:

public: 
    // History
    //string history[];

    string greeting = "Hi there > ";
    string command;

    // The run loop
    void execute();

    // Helper Functions
    void parse_and_execute(string c);

    void WaitFor(int pid);
};