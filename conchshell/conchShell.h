#include <iostream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
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
    void loop();

    void WaitFor(int pid);
    void execute(string c);

    //Parse and execute the command
    void parse_and_execute(string c);

};

//The execute loop, this will run indefinitely while the shell is running
void conchShell::loop(){
    while(true){
        cout << greeting;
        //Gets the entire line off of the command line
        getline(cin, command);
        parse_and_execute(command);
    }
}


//Parse the tokens as they come in and execute commands as necessary
void conchShell::parse_and_execute(string c){
    char * tokens;
    char delimiters[] = " ";

    //convert the command from a C++ string to a c-style string
    char *command_cstring = &command[0];
    tokens = strtok(command_cstring,delimiters);

    //Print out all of the tokens
    while (tokens)
    {   
        if (!strcmp(tokens, "ls")){
            execute(tokens);
        }
        tokens = strtok(NULL,delimiters);
    }
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* WaitFor(pid)                                                  *
* Waits for foreground processes...based on Rochkind's waitfor  *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void conchShell::WaitFor(int pid){
  int gotpid, status;

  while( (gotpid = wait(&status)) != pid)
    cout << "Process [" << gotpid << "] Finished..." << endl;
}

void conchShell::execute(string c){
    int id = fork();
    if(id == 0){ // we are in the child process
        char *cmd = "ls";
        char *argv[2];
        argv[0] = "ls";
        argv[1] = NULL;
        execvp(cmd, argv);
    }
    else{
        // Here we are in the parent
        WaitFor(id);
    }

    if(c.compare("exit") == 0){
        exit(0);
    }
}

