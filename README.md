# Os_project Deadline: 11/12/2022 23:59

## Goal of this programming project is to implement analternative Linux shell in C

# commands 

1. ## exec 

accepts a program to execute with its parameters and executes it bycreating a child process.
The program to execute will be a string (eventually with blank spaces inbetween if command-line parameters are needed).
After termination of the child process, the shell willoutput the PID of the finished process in a new line.
  

– Example:exec ps - uax will execute the command ps-uax.
– Hint: use the strtok function to tokenize (parse) the input string.


2. ## Modifier & 

if this modifier is used at the end of an exec command,imcshwill execute the commandin the background and continue immediately (without waiting for the command to finish). As soon asthe process ends, it will output the PID of the finished process in a new line.


3. ## global usage

his internal command will show a string with details on the version ofimcshbeingexecuted. Example output:IMCSH Version 1.1 created by<author(s)>

4. ## Modifier

f this modifier is used at the end of a command,imcshwill redirect the output as text tothe file given after the symbol. If the file does not exist, the shell will automatically create it. If thefile exists, the shell will try to append the new data to the end of the file. In this case, the only visibleoutput in the console will be the PID of the process after termination.


–Example 1: exec ls -l > directory_output.txt → Executes the command ls -l and writes the output to the filedirectory_output.txt. <br>
–Example 2: globalusage > usage.txt → Executes the command globalusage and writes the output to the fileusage.txt. <br>
-Hint:Use the dup2 system call (unistd.h) to redirect standard output (stdout) to a file de-scriptor <br>

## quit

quits the shell. If there are any running processes, the following question will be prompted to the user <br>: The following processes are running, are you sure you want to quit?<br> [Y/n]. A list of all currently running processes will follow. If the user enters Y, the shell will quit and all running processes will be terminated.
