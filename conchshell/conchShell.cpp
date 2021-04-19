#include "conchShell.h"
#include <iostream>
#include <string>
#include <sys/wait.h>


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
    cout << c << endl;
}