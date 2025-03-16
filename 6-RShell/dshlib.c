#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
 int exec_local_cmd_loop() {
    char *cmd_buff;
    int rc = 0;
    command_list_t cmd_list;

    cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
    if (!cmd_buff) {
        perror("malloc failed");
        return ERR_MEMORY;
    }

    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strlen(cmd_buff) == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        rc = build_cmd_list(cmd_buff, &cmd_list);
        if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        } else if (rc != OK) {
            continue;
        }

        if (cmd_list.num == 1) {
            Built_In_Cmds cmd_type = match_command(cmd_list.commands[0].argv[0]);

            if (cmd_type == BI_CMD_EXIT) {
                free(cmd_buff);
                return OK_EXIT;
            } 
            else if (cmd_type == BI_CMD_CD) {
                if (cmd_list.commands[0].argc > 1) {
                    if (chdir(cmd_list.commands[0].argv[1]) != 0) {
                        perror("cd failed");
                    }
                }
                continue;
            } 
            exec_cmd(&cmd_list.commands[0]);
        } else {
            execute_pipeline(&cmd_list);
        }
    }

    free(cmd_buff);
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (!cmd_line || !cmd_buff)
        return ERR_CMD_ARGS_BAD;

    cmd_buff->argc = 0;
    cmd_buff->_cmd_buffer = strdup(cmd_line); //copy the buffer
    if (!cmd_buff->_cmd_buffer)
        return ERR_MEMORY;

    char *cmd = cmd_buff->_cmd_buffer;
    bool insideQuotes = false;
    char *argStart;

    while (*cmd) {
        while (*cmd == SPACE_CHAR && !insideQuotes) { //skip leading spaces b4 quotes
            cmd++;  
        }

        if (*cmd == '"') {  //start of quote
            insideQuotes = true;
            cmd++; 
            argStart = cmd; //start of arg
            cmd_buff->argv[cmd_buff->argc++] = argStart;

            while (*cmd && (*cmd != '"' || insideQuotes)) { //parse quote until closing quote is found
                if (*cmd == '"') {
                    *cmd = '\0';  //null terminate
                    insideQuotes = false;
                    break;
                }
                cmd++;
            }
        } 
        else {  //normal arg (not in quotes)
            argStart = cmd;
            cmd_buff->argv[cmd_buff->argc++] = argStart;

            while (*cmd && *cmd != SPACE_CHAR) { //stop at the next space
                cmd++;
            }
        }

        if (*cmd) { 
            *cmd = '\0'; //null temrinate
            cmd++;
        }

        while (*cmd == SPACE_CHAR && !insideQuotes) {
            cmd++;
        }

        if (cmd_buff->argc >= CMD_ARGV_MAX - 1) break; //overflow check
    }

    cmd_buff->argv[cmd_buff->argc] = NULL; //null terminate
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    int count = 0;
    char *commands[CMD_MAX];
    char *command;

    command = strtok(cmd_line, PIPE_STRING);  //split into tokens by pipes
    memset(clist, 0, sizeof(command_list_t)); 

    while (command != NULL) {
        if (count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        commands[count++] = command; //store command
        command = strtok(NULL, PIPE_STRING); 
    }
    clist->num = count;

    for (int i = 0; i < count; i++) { 
        char *cmd = commands[i];

        while (*cmd == SPACE_CHAR) cmd++; //skip leading spaces

        alloc_cmd_buff(&clist->commands[i]);  //alocate mem for the command buffer
        if (build_cmd_buff(cmd, &clist->commands[i]) != OK) {
            return ERR_MEMORY;
        }
    }

    return OK;
}

int execute_pipeline(command_list_t *cmd_list) {
    int numOfCommands = cmd_list->num;
    int pipes[numOfCommands - 1][2]; // Array of pipes
    pid_t pids[numOfCommands]; // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < numOfCommands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create processes for each command
    for (int i = 0; i < numOfCommands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < numOfCommands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < numOfCommands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(cmd_list->commands[i].argv[0], cmd_list->commands[i].argv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < numOfCommands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < numOfCommands; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return OK;
}


int exec_cmd(cmd_buff_t *cmd) {
    int f_result;
    int c_result;

    f_result = fork();
    if (f_result < 0){ 
        perror("fork error");
        exit(1);
    }
    if (f_result == 0){
        execvp(cmd->argv[0], cmd->argv);
        perror("fork error");
        exit(EXIT_FAILURE);  //exit code for bad fork, but we want to stop here if the fork failed
    } else {
        wait(&c_result);
        if (WIFEXITED(c_result)) {
            int exitStatus = WEXITSTATUS(c_result); //store the exit status
        }
    }

    return OK;
}

Built_In_Cmds match_command(const char *input){
    if (strcmp(input, EXIT_CMD) == 0) return BI_CMD_EXIT; 
    if (strcmp(input, "cd") == 0) return BI_CMD_CD;
    if (strcmp(input, "dragon") == 0) return BI_CMD_DRAGON;
    return BI_NOT_BI;
} 

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX * sizeof(char)); 
    cmd_buff->argc = 0; 
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv)); 
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer) {
        memset(cmd_buff->_cmd_buffer, 0, strlen(cmd_buff->_cmd_buffer));
    }

    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));  
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer) {
        free(cmd_buff->_cmd_buffer); 
        cmd_buff->_cmd_buffer = NULL;  
    }
    return OK;
}
