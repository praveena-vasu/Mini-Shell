/*        Name              :Praveena Vasu
          Date              :27/06/2026
          Project Name      :Minishell
          Description       :The Mini Shell is a Linux command-line interpreter developed using the C programming language and
                            Linux system calls. The project simulates the basic functionality of a Unix/Linux shell by accepting
                            user commands, identifying their type, and executing them accordingly.The shell supports both built-in 
                            commands and external commands. Built-in commands such as cd, pwd, exit, jobs, fg, bg, and various echo
                            operations are handled internally by the shell. External commands are executed by creating child processes
                            using the fork() system call and replacing the child process image using execvp().

                            The project also implements signal handling for process management. Signals such as SIGINT (Ctrl+C), 
                            SIGTSTP (Ctrl+Z), and SIGCHLD are handled to provide behavior similar to a Linux shell. Stopped 
                            processes are maintained using a linked list, enabling job control operations through the jobs, fg, 
                            and bg commands.
                            In addition, the shell supports multiple command execution using pipes (|). Pipe handling is 
                            implemented using the pipe(), dup2(), and fork() system calls, allowing the output of one command to
                            be used as the input of another command.The shell also provides prompt customization through the PS1
                            variable, allowing users to modify the command prompt dynamically. */


#include"minishell.h"
#include<stdio.h>

char prompt_str[25]="minishell$:";
char input_str[50];
int main()
{
    system("clear");//to clear the console
    scan_input(prompt_str,input_str);
    
}
