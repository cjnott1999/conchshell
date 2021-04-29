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
    shell.loop();
}
