/*builtin commands*/
#include "minishell.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval","jobs",
						"bg","fg","set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
	


extern int status;
extern list* head;
extern int pid;
extern char* external_commands[154];

	//step1 : open a file and read the contents till \n
	//step2 : take a length of particular command
	//step3 : allocate the column memory based on the length
	//step4 : store the command in  external_commands[][] array


 /* Function to read all external commands from external_cmds.txt and store them in external_commands array */
void extract_external_commands(char **external_commands)
{
    char ch;                                                           
    char buffer[100];
    int i = 0;
    int count = 0;

    int fd = open("external_cmds.txt", O_RDONLY);

    if(fd == -1)
    {
        perror("open");
        return;
    }

    /* Read file character by character */
    while(read(fd, &ch, 1) > 0)
    {
        // Ignore carriage return character 
        if(ch=='\r')
          continue;
        if(ch == '\n')
        {
            buffer[i] = '\0';
             // Allocate memory and store command
            external_commands[count] = malloc((strlen(buffer) + 1)*sizeof(char));
            strcpy(external_commands[count], buffer);

            count++;
            i = 0;
        }
        else
        {
            buffer[i++] = ch;
        }
    }

    /* Handle last command if file doesn't end with '\n' */
    if(i > 0)
    {
        buffer[i] = '\0';

        external_commands[count] = malloc((strlen(buffer) + 1)*sizeof(char));
        strcpy(external_commands[count], buffer);

        count++;
    }

    external_commands[count] = NULL;

    close(fd);
}


 /* Extract first word from user input
        Example:
               Input  : "ls -l"
               Output : "ls"            */

char* get_command(char* input_str)
{
//fetch the first word-> input_str=>ls -l =>ls and store it in cmd array
   static char cmd[50];
 
    int i = 0;

    while(input_str[i]!='\0' && input_str[i] != ' ')
    {
        cmd[i] = input_str[i];
        i++;
    }

    cmd[i] = '\0';

    return cmd;
}


int check_command_type(char *cmd)
{
    int i;
//compare cmd[] with builtins[][] array   (comapre index by index)
	//if it is matching
    for(i = 0; builtins[i] != NULL; i++)
    {
        if(strcmp(cmd, builtins[i]) == 0)
            return BUILTIN;
    }
//compare cmd[] with external_commands[][] array
    //if it is matching 
    for(i = 0; external_commands[i] != NULL; i++)
    {
        if(strcmp(cmd, external_commands[i]) == 0)
            return EXTERNAL;
    }

    return NO_COMMAND;
}



void execute_internal_commands(char *input_str)
{
    /* exit */
   if(strcmp(input_str,"exit")==0) 
    {
        for(int i=0; external_commands[i]!=NULL; i++)
            free(external_commands[i]);
    exit(0);
     }

    /* pwd */
    else if ((strcmp(input_str, "pwd") == 0))
    {
        // char cwd[50];
        //getcwd(cwd,50);//store old working directory
        char cwd[100];

        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s$\n", cwd);
        }
    }
    
    //if it is true     /*cd*/
   /* else if((strncmp(input_str,"cd",2))==0)
    {
        chdir(input_str + 3);//system call to change the directory

    }*/else if(strncmp(input_str,"cd",2)==0)
{
    char *path = input_str + 2;

    while(*path == ' ')
        path++;

    if(*path == '\0')
        chdir(getenv("HOME"));
    else
        chdir(path);
}
       /* Display shell process ID */    
    else if(strcmp(input_str,"echo $$")==0) //check the input_str contains echo $$ or not 
          {
            printf("%d\n", getpid());//to get exist status code.
          }
             
   else if(strcmp(input_str,"echo $?")==0)  // Display exit status of previous command(if 0--->successfully executed).
           {
        if(WIFEXITED(status))
        {      
            printf("%d\n",WEXITSTATUS(status));
        }
          }


    /* Display shell environment variable */
    else if(strcmp(input_str,"echo $SHELL")==0)
      {                                       //check the input_str  contains echo$shell or not
                                              // if it is true 
        printf("%s\n", getenv("SHELL"));          //to get  
      }

    /* Display all stopped jobs */
    else if(strcmp(input_str,"jobs")==0)//check input_str contains jobs or not
       {           //if it is
          print_list();
       }

       /* Bring stopped process to foreground */
   else if(strcmp(input_str,"fg")==0) //check the input_str contains fg or not
    {
        if(head==NULL)
        {
            printf("No stopped process\n");
        }
     //if it is
          else
              {
            int fg_pid = head->pid;
             printf("%s\n", head->cmd);
             kill(fg_pid, SIGCONT);
             waitpid(fg_pid, &status, WUNTRACED);
             delete_first(); //after completing the particular task delete the node also.
                             //because if we command jobs after the process completion it wont show.
		      }     
      }
             
     /* Resume stopped process in background */ 
   else if(strcmp(input_str,"bg")==0) //check the input_str contains bg or not
        { 
            //if it is  
         if(head==NULL)
            printf("No stopped process\n");
        else
        {   
            printf("%s\n", head->cmd);
         kill(head->pid,SIGCONT);//continue the process.
         delete_first();
         signal(SIGCHLD, signal_handler);
         
        }
       }
       else
	    {
		printf("Invalid Command\n");
	    }
}

void execute_external_commands(char* input_str)
{
     //2D array conversion logic - it should be pointer to pointer(char **) , not pointer to array - (*)[30]
   
    char arr[20][20];
    int i = 0, r = 0, c = 0;

    while(input_str[i] != '\0')
    {
        if(input_str[i] == ' ')
        {
            arr[r][c] = '\0';
            r++;
            c = 0;
        }
        else
        {
            arr[r][c++] = input_str[i];
        }
        i++;
    }

    arr[r][c] = '\0';

    char *argv[21];

    for(int j = 0; j <= r; j++)
    {
        argv[j] = arr[j];
    }
    argv[r + 1] = NULL;

    int flag = 0;

    for(int j = 0; j <= r; j++)
    {
        if(strcmp(arr[j], "|") == 0)
        {
            flag = 1;
            break;
        }
    }

    if(flag == 0)
    {
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    }

    int cmd_pos[10];
    int cmd_count = 0;

    cmd_pos[cmd_count++] = 0;

    for(int j = 0; j <= r; j++)
    {
        if(strcmp(argv[j], "|") == 0)
        {
            argv[j] = NULL;
            cmd_pos[cmd_count++] = j + 1;
        }
    }

    int input_fd = 0;
    int fd[2];

    for(int k = 0; k < cmd_count; k++)
    {
        if(k != cmd_count - 1)
        {
            pipe(fd);
        }

        pid_t pid = fork();

        if(pid == 0)
        {
            if(input_fd != 0)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            if(k != cmd_count - 1)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }

            execvp(argv[cmd_pos[k]], &argv[cmd_pos[k]]);
            perror("execvp");
            exit(1);
        }
        else if(pid > 0)
        {
            if(input_fd != 0)
            {
                close(input_fd);
            }

            if(k != cmd_count - 1)
            {
                close(fd[1]);
                input_fd = fd[0];
            }
        }
        else
        {
            perror("fork");
            return;
        }
    }

    for(int k = 0; k < cmd_count; k++)
    {
        wait(NULL);
    }

    exit(0);
}



int insert_first(char* data,int pid)
{
    //printf("INSERTING PID=%d CMD=%s\n",pid,current_cmd);
    list *new = malloc(sizeof(list));
    if(new==NULL)
    {
        printf("Memory is not allocated\n");
        return 0;
    }
    strcpy(new->cmd,data);
    new->pid = pid;
    new->link = head;
    head = new;

    return 1;
}

int delete_first()
{
    if(head == NULL) 
        return 0;
   else
   {                      //printf("Deleting PID=%d\n", head->pid);
    list *temp = head;
    head = head->link;
    free(temp);
   }

    return 1;
}

void print_list()
{
    if(head==NULL)
    {
        printf("\n");
        return;
    }
    list *temp = head;
    int count = 1;
    while(temp)
    {
        printf("[%d]+ Stopped\t\t%s\n", count, temp->cmd);
        temp = temp->link;
        count++;
    }
}
	/*exit
    
	cd
	pwd

	//check input string contains exit or not
	//if it is exit 
	   //exit(0)
  //check the input_str contains cd or not
  if(strncmp(input_str
  //use strncmp
     if it is
              chdir(input_str+3);
//checl if if input_str contains pwd or not
//if it is
          char buff[50];
		  getcwd(buff,50);if u want use before mini shell prompt
		 //print the directory


	    
}*/