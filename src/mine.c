#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#define LIMIT 256 // max number of tokens for a command
#define MAXLINE 1024 

#define TRUE 1


//User can now use cd command to change directory
//User can now use exit command to exit the shell 
//User can now use ls command to list files in current directory = still using system call
//User can now use globalusage to display the message:  IMCSH Version 1.1 created by <user>




int changeDirectory(char* args[]){
	// If we write no path (only 'cd'), then go to the home directory
    printf("hello");
	if (args[1] == NULL) {
        
		chdir(getenv("HOME")); 
        printf("Changed to home directory\n");
		return 1;
	}
	// Else we change the directory to the one specified by the 
	// argument, if possible
	else{
        if (chdir(args[1]) != 0) {
            printf("No such directory\n");
        }
        else{
            printf("Changed to directory %s\n", args[1]);
            return 1;
        }
        
    }



        
}

int globalUsage(){




    char text[] = "IMCSH Version 1.1 created by";

    printf("%s %s\n",text,getenv("LOGNAME"));
    return 1;
}



int commandHandler(char* tokens[]){

    if(strcmp(tokens[0],"cd")==0){
        changeDirectory(tokens);
    }
    else if(strcmp(tokens[0],"exit")==0){
        exit(0);
    }
    else if (strcmp(tokens[0],"ls")==0){
        system("ls");
    
    }
    else if (strcmp(tokens[0],"exec")==0){
        execvp(tokens[1],tokens);
    
    }
    // global usage
    else if (strcmp(tokens[0],"globalusage") == 0){
        globalUsage();
    }






    else if(strcmp(tokens[0],"help")==0){
        printf("cd - change directory");
    }
    else{
        printf("Command not found\n");
    }


}


    

int main() {
	char line[MAXLINE]; // buffer for the user input
	char * tokens[LIMIT]; // array for the different tokens in the command
	int numTokens;
    int no_reprint_prmpt;
	no_reprint_prmpt = 0; 	// to prevent the printing of the shell
	
    pid_t pid;				// after certain methods
	pid = -10; // we initialize pid to an pid that is not possible
	

    
    // We set our extern char** environ to the environment, so that
    // we can treat it later in other methods

    
	


    

	// We set shell=<pathname>/simple-c-shell as an environment variable for
	// the child
    static char* currentDirectory;
	setenv("shell",getcwd(currentDirectory, 1024),1);
	
	// Main loop, where the user input will be read and the prompt
	// will be printed
	while(TRUE){
		// We print the shell prompt if necessary
	
		no_reprint_prmpt = 0;
		
		// We empty the line buffer
		memset ( line, '\0', MAXLINE );

		// We wait for user input
		fgets(line, MAXLINE, stdin);
	
		// If nothing is written, the loop is executed again
		if((tokens[0] = strtok(line," \n\t")) == NULL) continue;
		
		// We read all the tokens of the input and pass it to our
		// commandHandler as the argument
		numTokens = 1;
		while((tokens[numTokens] = strtok(NULL, " \n\t")) != NULL) numTokens++;
        commandHandler(tokens);
        int i;
        for(i = 0; i < numTokens; i++){
            
            printf("token %d: %s\n", i,tokens[i]);
        
        
            
		
	}          

	
}
}