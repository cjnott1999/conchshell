#include <iostream>
#include <fstream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <filesystem>
#include <vector>

#include "lexer.h"
#include "tokens.h"


using namespace std;

class conchShell {
private:

public: 

    //Set the current shell greeting
    string greeting = "$ ";
    //Store the working command
    string command;

    //History conditions
    vector<string> history;

    //Redirect conditions
    int redirect = 0;
    char *redirectFileName;

    //Run the Loop
    void loop();

    //Shell builtins
    void conch_cd(string directory);

    //Functions to actually execute the system command
    void WaitFor(int pid);
    void execute(char *cmd, char *argv[]);

    //Parse and execute the command
    void parse_and_execute(string c);
    
};


void conchShell::conch_cd(string directory){
    if (directory == "~"){
        chdir(getenv("HOME"));
    }
    else {
        chdir(directory.c_str());
    }
  
    
}

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
    //convert the command from a C++ string to a c-style string
    
    char *command_cstring = &command[0];

    //Do some lexing
    Token *tokens= lex(command_cstring);
    

    //Now let us do some parsing 


    vector<string>  inputTokens;

    int tracker = 0;
    int fileOutput = 0;

    //if the first token is exit, we gotta exit
    if (tokens->type == TOKEN_EXIT){
        exit(0);
    }

    if (tokens->type == TOKEN_CD){
        string directory = "~";
        if (tokens->next != NULL){
            tokens = tokens->next;
            directory = tokens->identifier;

        }

        conch_cd(directory);
        return;
    }

    //Check for bang
    if (tokens->type == TOKEN_BANG){
        string historic_command = history[history.size()- 2];
        char *historic_command_cstring = &historic_command[0];

        tokens = lex(historic_command_cstring);
            
    }
    
    if(tokens->type == TOKEN_HISTORY){
        for(int i = 0; i < history.size(); i++){
            cout << history[i] << endl;
        }
    }
    
    while(tokens!=NULL)
    {
        //Check is file redirect is present
        if (tokens->type == TOKEN_FILE_OUTPUT){
            fileOutput = tracker;
            redirect = 1;
        }
        inputTokens.push_back(tokens->identifier);
        tokens=tokens->next;
        tracker++;
    }

    int num_tokens;
    if (fileOutput == 0){
        num_tokens = inputTokens.size();
    }
    else{
        num_tokens = fileOutput;
        redirectFileName = &inputTokens[fileOutput + 1][0];
    }
    
    char *cmd = &inputTokens[0][0];
    char *argv[num_tokens];

    for(int i = 0; i < num_tokens; i++) {
            argv[i] = &inputTokens[i][0];
    }


    argv[num_tokens] = NULL;

    //Add the command to history
    history.push_back(command);
    execute(cmd,argv);

}

void conchShell::WaitFor(int pid){
  int gotpid, status;
  while( (gotpid = wait(&status)) != pid);
}

void conchShell::execute(char *cmd, char *argv[]){
    //Pipe up
    int pipefd[2];
    pipe(pipefd);

    //Fork stuff up
    int id = fork();
    
    if(id == 0){ 
        if (redirect){
            // we are in the child process
            close(pipefd[0]); 
            //Dup the pipe to stdout
            dup2(pipefd[1], 1); 
        }
        //Run the command through a system call
        execvp(cmd, argv);
    }
    else{

        if (redirect){
            //Ofstram will redirect output to a file
            ofstream out(redirectFileName);
            //Close the write end of the pipe
            close(pipefd[1]);
            //Open the read end of the pipe as a file
            FILE *readP = fdopen(pipefd[0], "r");
            char c[1000];
            
            while (fgets(c,999,readP) != NULL){
                out << c;
            }

            //Clean up and close
            close(pipefd[0]);
            out.close();
        }

        //Reset the file redirect
        redirect = 0;
        WaitFor(id);
    }

}

