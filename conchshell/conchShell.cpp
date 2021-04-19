#include "conchShell.h"
#include <iostream>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <stdio.h>


using namespace std;

int main(int argc, char* argv[]){
    //Inititate a shell object
    conchShell shell;

    //Begin execution
    shell.execute();
}

void conchShell::execute(){
    while(true){
        cout << greeting;

        //Gets the entire line off of the command line
        getline(cin, command);

        parse_and_execute(command);
    }
}

void conchShell::parse_and_execute(string c){
    char * tokens;
    char delimiters[] = " ";

    //conver the command from a C++ string to a c-style string
    char *command_cstring = &command[0];
    tokens = strtok(command_cstring,delimiters);

    //Print out all of the tokens
    while (tokens)
    {
        cout << tokens << endl;
        tokens = strtok(NULL,delimiters);
    }
}