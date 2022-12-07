#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define LIMIT 256 // max number of tokens for a command
#define MAXLINE 1024
#define TRUE 1

int changeDirectory(char *args[])
{
    // If we write no path (only 'cd'), then go to the home directory
    if (args[1] == NULL)
    {
        chdir(getenv("HOME"));
        printf("Changed to home directory\n");
        return 1;
    }
    // Else we change the directory to the one specified by the
    // argument, if possible
    else
    {
        if (chdir(args[1]) != 0)
        {
            printf("No such directory\n");
            return -1;
        }
        else
        {
            printf("Changed to directory %s\n", args[1]);
            return 1;
        }
    }
}

int quit()
{
    // if any poccesses are running, then
    // print all the running processes
    // ask the user if he wants to quit
    // if yes, then quit
    // if no, then continue
    printf("Following processes are running:\n");
    // execvp("ps");
    printf("Do you want to quit? (y/n)\n");
    char answer;
    scanf("%c", &answer);
    if (answer == 'y')
    {
        printf("Quitting\n");
        // kill all the processes
        
        exit(0);
    }
    else
    {
        printf("Continuing\n");
        return 1;
    }
}


int globalUsage()
{
    char text[] = "IMCSH Version 1.1 created by Bertold Vinze, David Bobek and Dinu Scripnic";
    printf("%s\n", text);
    return 1;
}


int executeFunction(char *tokens[])
{
    // fork a child process
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        //
        if (execvp(tokens[1], tokens + 1) < 0)
        {
            printf("Error executing command\n");
            return -1;
        }
    }
    else if (pid < 0)
    {
        // error forking
        printf("Error forking\n");
        return -1;
    }
    else
    {
        // parent process
        while (wait(&status) != pid);
        printf("Child process with id %d terminated\n", pid);
    }
    return 1;
}


int export(char *tokens[])
{
    // find the filename in tokens
    // open the file
    // write the output of the command to the file
    // close the file
    // return 1
    FILE *pFile = NULL;
    int idx = 0;
    // count the number of tokens
    while (tokens[idx] != NULL){
        idx++;
    }
    // open the file
    pFile = fopen(tokens[idx - 1], "a");
    // check if the file was opened
    if (pFile == NULL)
        {
            printf("Error opening file\n");
            return -1;
        }
    // if the command is globalusage, then write the text to the file
    if (strcmp(tokens[0], "globalusage") == 0)
    {
        fprintf(pFile, "IMCSH Version 1.1 created by Bertold Vinze, David Bobek and Dinu Scripnic\n");
        fclose(pFile);
        return 1;
    }
    else if (strcmp(tokens[0], "exec") == 0)
    {
        // create a new array with the tokens before >
        char *newTokens[idx];
        int i = 0;
        // copy the tokens to the new array
        while (strcmp(tokens[i], ">") != 0)
        {
            newTokens[i] = tokens[i];
            i++;
        }
        newTokens[i] = NULL;  
        // fork a child process
        pid_t pid;
        int status;
        pid = fork();
        // print the output of the command to the file
        if (pid == 0)
        {
            // redirect the output to the file
            dup2(fileno(pFile), 1);
            if (execvp(newTokens[1], newTokens+1) < 0)
            {
                printf("Error executing command\n");
                return -1;
            }
        }
        else if (pid < 0)
        {
            // error forking
            printf("Error forking\n");
            return -1;
        }
        else
        {
            // parent process
            while (wait(&status) != pid);
            printf("Child process with id %d terminated\n", pid);
        }

    }
}


int runBackground(char *tokens[]){
    printf("Running in background\n");
    return 1;
}


int commandHandler(char *tokens[])
{
    int idx = 0;
    // get the number of tokens
    while (tokens[idx] != NULL)
    {
        idx++;
    }
    // check if the command have > in it
    // The error is in this for loop
    for (int i = 0; i < idx; i++)
    {
        if (strcmp(tokens[i], ">") == 0)
        {
            export(tokens);
            return 1;
        }
        if (strcmp(tokens[i], "&") == 0)
        {
            runBackground(tokens);
            return 1;
        }
    }
    // check if the command is cd
    if (strcmp(tokens[0], "cd") == 0)
    {
        changeDirectory(tokens);
    }
    // check if the command is quit
    else if (strcmp(tokens[0], "quit") == 0)
    {
        quit();
    }
    // check if the command is exec
    else if (strcmp(tokens[0], "exec") == 0)
    {
        executeFunction(tokens);
    }
    // check if the command is globalusage
    else if (strcmp(tokens[0], "globalusage") == 0)
    {
        globalUsage();
    }
    // if none of the above, then the command is not found
    else
    {
        printf("Command not found\n");
    }
}

int main()
{
    char line[MAXLINE];  // buffer for the user input
    char *tokens[LIMIT]; // array for the different tokens in the command
    int numTokens;
    int no_reprint_prmpt;
    no_reprint_prmpt = 0; // to prevent the printing of the shell

    pid_t pid; // after certain methods
    pid = -10; // we initialize pid to an pid that is not possible

    // We set our extern char** environ to the environment, so that
    // we can treat it later in other methods

    // We set shell=<pathname>/simple-c-shell as an environment variable for
    // the child
    static char *currentDirectory;
    setenv("shell", getcwd(currentDirectory, 1024), 1); // we set the shell variable to the current directory

    // Main loop, where the user input will be read and the prompt
    // will be printed

    while (TRUE)
    {
        // We print the shell prompt if necessary
        fputs("IMCSH> ", stdout);
        no_reprint_prmpt = 0;

        // We empty the line buffer
        memset(line, '\0', MAXLINE);

        // We wait for user input
        fgets(line, MAXLINE, stdin);

        // If nothing is written, the loop is executed again
        if ((tokens[0] = strtok(line, " \n\t")) == NULL)
            continue;

        // We read all the tokens of the input and pass it to our
        // commandHandler as the argument
        numTokens = 1;
        while ((tokens[numTokens] = strtok(NULL, " \n\t")) != NULL)
            numTokens++;
        commandHandler(tokens);
        //     int i;
        //     for(i = 0; i < numTokens; i++){

        //         printf("token %d: %s\n", i,tokens[i]);

        // }
    }
}