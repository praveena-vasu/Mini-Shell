#include "minishell.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* external_commands[154]; //Array to store all external commands

pid_t pid=0;
extern char prompt_str[25];
extern char input_str[50];
char current_cmd[25];//Stores current command name
int status;

list* head=NULL;

void signal_handler(int signum)
{
     /* Ctrl + C pressed */
    if(signum==SIGINT)
    {
     /* If no child process is running, display the shell prompt again */
        if(pid==0)
        {
       printf("\n%s",prompt_str);
       fflush(stdout);
            
        } 

    }
     /* Ctrl + Z pressed */
    else if(signum==SIGTSTP)
    {
        /* If shell is waiting for input, simply print the prompt again */
        if(pid==0)
        {
        printf("\n%s",prompt_str);
        fflush(stdout);
        }
       else
         {
                    //when cntrl+z pressed store the stopped processes inside the linked list.
                     //insert_first=>logic(char* prompt_str,char* input_str)
                     //( store pid,command,NULL)

             /* Child process is stopped.Store it in the linked list for background/job control */
            printf("[+1]\n stopped %d\n",pid);
            insert_first(input_str,pid);
         }
    }
        /* Child process termination signal */
    else if(signum==SIGCHLD)
    {
        //How to clear thr source to avoid zombie process
        while(waitpid(-1,&status,WNOHANG)>0);
        {

        }
    }
}

/* Function to read and execute commands */
void scan_input(char* prompt_str,char* input_str)
{
   
    extract_external_commands(external_commands);
     /* Register signal handlers */
    signal(SIGINT,signal_handler); 
    signal(SIGTSTP,signal_handler);
    signal(SIGCHLD,signal_handler);

    while(1)
    {
    
      printf("%s", prompt_str);
      scanf(" %[^\n]",input_str);
      getchar();
    
    //step1:check input_str contains ps1=" or not(STRNCMP)
   if(strncmp(input_str, "PS1=", 4) == 0) //step2 :if it is PS1=
    {
        if(input_str[4]!=' ') //check if input_str[4] is space or not 
        {
            strcpy(prompt_str,input_str+4);
            //continue;

        }
        else//if space is present
        {
        printf("Error!,Space after =");
        }
    }
   
    //get cmd
 else
 {
    char* cmd=get_command(input_str);
    int ret=check_command_type(cmd);

 
    if(ret==BUILTIN)
    {
       execute_internal_commands(input_str);
        //logic for internal commands.
    }
    else if(ret==EXTERNAL)
    {
     //pid=0;//to avoid print the prompt two times.if the child process not created that times the pid will be 0;(without command pid will be 0)
      //trcpy(current_cmd,input_str);
     int status;
       /* Create child process */
      pid=fork();//with command that means it will create child preocess.so non zero value  will be return
    if(pid>0)
    {

        waitpid(pid,&status,WUNTRACED);//to know which child has stopped.
        
    }
    
    else if(pid==0)
    {
         /* Restore default signal handling */
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        execute_external_commands(input_str);
    }
    /* Reset pid after command execution */
      pid=0;

        
    } 
     else if(ret==NO_COMMAND)
     {
        printf("Command not found\n");
     }
}   
}
}





//convert input_str 1d to 2darray[][].
//check pipe is present or not
      //if not
    /*  execvp(arr[0],arr);  //arr means 2d array name.
      int fd[2];
      if(pipe(fd)==0)
      {
        //if pipe is present(n pipes assignment  logic).
        //logic for external command
     }
    }
    else
    {
        //print error msg
    }
 }*/

    