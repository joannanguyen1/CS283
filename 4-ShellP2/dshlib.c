#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

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
    cmd_buff_t cmd;

    cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
    alloc_cmd_buff(&cmd);

    while (1){
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL){
            printf("\n");
            break; 
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strlen(cmd_buff) == 0){
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        rc = build_cmd_buff(cmd_buff, &cmd); 
        if (rc == ERR_TOO_MANY_COMMANDS){ //too many commands
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        } else if (rc == OK) { 
            Built_In_Cmds cmd_type = match_command(cmd.argv[0]); //match the command
            if (cmd_type == BI_CMD_EXIT){  //exit
                clear_cmd_buff(&cmd);
                free_cmd_buff(&cmd);
                break;
            }
            else if (cmd_type == BI_CMD_CD){ //cd 
                if (cmd.argc > 1){
                    if (chdir(cmd.argv[1]) != 0){ 
                        perror("cd failed");
                    }
                }
                free_cmd_buff(&cmd);
                continue;
            } else if (cmd_type == BI_CMD_DRAGON){ //dragon
                print_dragon();
                free_cmd_buff(&cmd);
                continue;
            }
        }

        int f_result;
        int c_result;

        f_result = fork();
        if (f_result < 0){ 
            perror("fork error");
            exit(1);
        }
        if (f_result == 0){
            execvp(cmd.argv[0], cmd.argv);
            perror("fork error");
            exit(EXIT_FAILURE);  //exit code for bad fork, but we want to stop here if the fork failed
        } else {
            wait(&c_result);
            if (WIFEXITED(c_result)) {
                int exitStatus = WEXITSTATUS(c_result); //store the exit status
            }
        }

        clear_cmd_buff(&cmd);
        free_cmd_buff(&cmd); //free memory
    }

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


