#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SZ 50
#define SPACE ' '
#define TAB '\t'
#define PERIOD '.'
#define NULL_TERMINATOR '\0'

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *, int, int);
int word_print(char *, int, int);

int setup_buff(char *buff, char *user_str, int len) {
    // fill the entire buffer with .
    memset(buff, PERIOD, len);

    bool inSpace = false;  // tracks whether we are currently inserting spaces
    int inputLen = 0;      // intialize len to 0
    char *out = buff;      // ptr for writing to buff
    char *end = buff + len; // ptr to the end of the buffer

    while (*user_str == SPACE || *user_str == TAB) { // skips spaces and tabs
        user_str++;
    }

    while (*user_str != NULL_TERMINATOR) {  // copy user_str into buff
        if (*user_str == SPACE || *user_str == TAB) {
            if (inSpace) { //it is whitespace
                user_str++; // if already inserted a space skip whitespace
                continue;
            } else { // put only one space
                if (out < end) {
                    *out = SPACE;  // write a single space
                    out++;
                    inputLen++;
                } else {
                    return -1; // no space left in buff
                }
                inSpace = true; // in a collective amount of spaces
            }
        } else { // not whitespace
            if (out < end) {
                *out = *user_str; // copy the char
                out++;
                inputLen++;
            } else {
                return -1; // no space in left in buff
            }
            inSpace = false; // no longer in space
        }
        user_str++;
    }

    char *lastChar = out - 1; // if last char was a space make it into a period
    if (*lastChar == SPACE) {
        *lastChar = PERIOD;  // turn the space to a .
        inputLen--;
    }
    return inputLen;
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int wordCount = 0; // intialize 0 words
    bool wordStart = false; // the start of a word is intially false (i.e many spaces in the beginning)

    if (str_len > len){ // sanity check
        printf("Error: Provided input string is too long");
        exit(3);
    }
    if (str_len == 0){ // empty buff
        return 0; 
    }

    for (int i = 0; i < str_len; i++){
        if (*buff == SPACE){ // if it is a space, it is not the beginning of a word
            wordStart = false;
        } else {
            if (wordStart == false){ // if it is not a space, but we are starting a new word
                wordCount++;
                wordStart = true;
            } 
        } // if it is a character and not a new word then continue
        buff++; 
    }
    return wordCount; 
}

void reverse_string(char *buff, int bufferSize, int len){
    char *start = buff; // pointer to the start of the string
    char *end = buff + len - 1; // pointer to the end of the string
    char temp; // temp var for swapping

    if (len > bufferSize){ // sanity check
        printf("Error: Provided input string is too long");
        exit(3);
    }
    if (len == 0){ // empty buff
        return;
    }

    while (end > start){ // iterate until the end char of the string is at the start of the string
        temp = *start; // stores the starting char
        *start = *end; 
        *end = temp; // swap last char with temp (of the starting char)
        start++;
        end--;
    }
}

int word_print(char *buff, int bufferSize, int len){
    int wordCount = 0; // intial wc = 0
    int wordLen = 0; // intial len per word is 0
    bool wordStart = false; // sees if we are currently in a word
    char *end = buff + len; // points to the position right after the last char

    if (len > bufferSize){ // sanity check
        printf("Error: Provided input string is too long");
        exit(3);
    }
    if (len == 0){ //empty buff
        return 0;
    }

    while (buff < end){ // loop thought every char
        if (wordStart == false){
            if (*buff == SPACE){ // if it is a space skip it and move on
                buff++;
                continue;
            } else { // this is the first char of a word
                wordCount++; 
                wordStart = true; //are now in a word
                wordLen = 0; //reset the count
                printf("%d. %c", wordCount, *buff); 
                wordLen++; 
            }
        } else {
            if (*buff == SPACE){ // are no longer in a word
                wordStart = false;
                printf(" (%d)\n", wordLen); // print the len after the word has ended
                wordLen = 0; // reset the len
            } else {
                printf("%c", *buff); //still a char
                wordLen++;
            }
        }
        buff++;
        if (buff == end && wordStart == true) { // at the end ptr but still a word, print its len
            printf(" (%d)\n", wordLen);
        }
    }

    return wordCount;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //This is safe because accessing argv[1] without checking can cause unexpected outputs as there is an insufficient amount of args
    //So by checking that argc >= 2 before utilzing argv[1], ensures that argv[1] does indeed exist and prevents the above from occuring

    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //This if statement checks to see if there are enough arguments to properly run the program and recieve its expected output
    //If there are not enough args, then the proper input format is provided
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    buff = (char*)malloc(BUFFER_SZ); //allocation space for buff with BUFFER_SZ (50)
    if (buff == NULL){
        printf("Error: Memory allocation failed for buffer.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        case 'r':
            reverse_string(buff, BUFFER_SZ, user_str_len); 
            printf("Reversed string: ");
            char *ptr = buff; // pointer to the start of the buffer
            for (int i = 0; i < user_str_len; i++) {
                putchar(*ptr);
                ptr++; 
            } // only prints the string, not the periods in the buffer
            printf("\n");
            break;
        case 'w':
            int numOfWords = 0;
            printf("Word Print\n----------\n");
            numOfWords = word_print(buff, BUFFER_SZ, user_str_len);
            printf("\nNumber of words returned: %d\n", numOfWords);
            break;
        case 'x':
            if (argc != 5) {
                printf("Error: -x requires <string> <find> <replace>\n");
                free(buff); //free buff
                exit(1);
            }
            printf("Not Implemented!\n");
            exit(3); // error with provided service because this service doesn't exist

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff); //freeing buff
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
// Passing the len helps with triple checking (sanity checks) and making sure we dont cause a buffer overflow
// Passing in the pointer can reduce seg faults with memory leaks from trying to access memory we do not own
// Also this helps with resuability as what if we wanted a different buffer size, we are able to reuse this code with minor tweaks
// This can help with the manipulations of the strings, so we are effiecently able to determine what portion is the actual string and where the periods begin (if they do i.e less than 50 chars)