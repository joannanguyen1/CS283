#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    int count = 0;
    char *commands[CMD_MAX];
    char *command;

    command = strtok(cmd_line, PIPE_STRING); //seperate by pipes
    memset(clist, 0, sizeof(command_list_t)); //intialize command list with zeros

    while (command != NULL){
        if (count >= CMD_MAX){ //too many commands
            return ERR_TOO_MANY_COMMANDS;
        }
        commands[count++] = command; //store command, and move on 
        command = strtok(NULL, PIPE_STRING);
    }

    clist->num = count; //store the count

    for (int i = 0; i < count; i++){ 
        char *cmd = commands[i];
        while (*cmd == SPACE_CHAR) //trim leading whitespaces
            cmd++;

        char *delimiter = strchr(cmd, SPACE_CHAR); //seperate executables and args
        if (delimiter){
            *delimiter = '\0'; //terminate executable name
            strncpy(clist->commands[i].exe, cmd, EXE_MAX - 1); 
            strncpy(clist->commands[i].args, delimiter + 1, ARG_MAX - 1);
        }
        else {
            strncpy(clist->commands[i].exe, cmd, EXE_MAX - 1);
        }
    }

    return OK;
} 
