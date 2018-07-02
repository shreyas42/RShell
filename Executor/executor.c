#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include "executor.h"
#include "../Parser/command.h"

extern struct command *command_table;

int execute(){
    // very basic execute
    // temp variable in case of redirection;
    int tempstdin = dup(0);
    int tempstdout = dup(1);
	struct sigaction interruptaction;
	interruptaction.sa_handler = SIG_DFL;
	interruptaction.sa_flags = SA_SIGINFO;
    int input_fd;
    if (strcmp(command_table->infile, "stdin")){
        // Case to open the file if infile is set 
        input_fd = open(command_table->infile, O_RDONLY);
    }
    else{
        // Else duplicating the stdin fd
        input_fd = dup(tempstdin);
    }

    int ret;
    int output_fd;
    int flag;
    for(int i = 0;i < command_table->num_ava_simple_command;i++){
        // redirect input
        dup2(input_fd, 0);
        close(input_fd);
        // setup output
        if(i == command_table->num_ava_simple_command-1){
            if(command_table->mode == 1){
                flag = O_APPEND | O_RDWR | O_CREAT;
            }
            else{
                flag = O_TRUNC | O_RDWR | O_CREAT;
            }
            if(strcmp(command_table->outfile, "stdout")){
                output_fd = open(command_table->outfile, flag);
            }
            else{
                output_fd = dup(tempstdout);
            }
        }
        else{
            int pipefd[2];
            pipe(pipefd);
            output_fd = pipefd[1];
            input_fd = pipefd[0];
        }
        // Implement Piping all redirect output
        // Redirect Output
        dup2(output_fd, 1);
        close(output_fd);

        // Create a child proces
        // Exec
        if(!strcmp(command_table->simplecmds[i]->cmd_name, "cd")){
            // If command is cd
            chdir(command_table->simplecmds[i]->args[1]);   
        }
        else{
            ret = fork();
            if(ret == 0){
				if(sigaction(SIGINT, &interruptaction , NULL) < 0)
					perror("Sigaction");
                execvp(command_table->simplecmds[i]->args[0], command_table->simplecmds[i]->args);
                perror(command_table->simplecmds[i]->args[0]);
                exit(1);
            }
        }
    }
    // print_table();
    // Restore the output and input once command is run
    dup2(tempstdin, 0);
    dup2(tempstdout, 1);
    close(tempstdin);
    close(tempstdout);
    // Check for backgorund
    int status;
    if(!command_table->background){
        waitpid(ret,&status, 0);
        // printf("%d\n", status);
    }
    return ret;
}
