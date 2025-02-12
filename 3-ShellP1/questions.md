1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  `fgets()` is a good choice for this application, as it is essentially a line by line processor because reading stops after an EOF or a newline. It captures the full input including spaces, thus making it ideal for command parsing, whilst ensuring input does not exceed buffer size.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  `malloc()` gives more flexibility and ensures we are not using excess memory, as memory will only be allocated when needed. A fixed size array would be stored on the stack (limited space), whereas  `malloc()` stores on the heap, thus preventing issues like stack overflow for large buffers. Since  `malloc()`  dynamically allocates memory, it is better for memory management for cases where input size can vary.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: If we do not trim leading and trailing spaces, it could cause unexpected behavior like executing the wrong command. Entering like " cd " could be interpreted by the system in a numerous amount of ways, like empty command as the first character is a space or an empty argument as there is no directory in the command to change to, however it is interpreted wouldn't properly execute its intended way unless the leading and trailing spaces were removed.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:   
    1. Overwrite Redirection (<, >) - store/save the output of a command to a file and replace all the existing content of that file (cat > file.txt). Challenge: Being careful with file management, as a missing or unreadable file could cause unexpected behavior and possibly overwrite files you did not intend to overwrite. 
    2. Append Redirection(<<, >>) - append the output to the file without compromising the existing data of the file (echo "joanna" >> file.txt). Challenge: Loop-based input reading is required as << requires reading lines until the delimiter, which must be a single word that does not contain spaces or tabs.
    3. Merge Redirection (>&) - redirect the output of a command or a program to a specific file descriptor instead of standard output (command > output.log 2>&1). Challenge: execution order matters, like if it is not handled proprly, it might not merge streams properly.


- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection is used to pass input/output to either a file or stream, whereas piping is used to pass output to another program or utility (through connecting the output of one command, directly into the input of another command). Thus instead of just outputting into a file, piping would allows the usage of the output to become the next input allowing for chaining multiple commands together dynamically.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep these seperate for debugging and error handling purposes, as if an error occurs, a user would be able to idnetify the bug easier due to the seperation of the streams, as opposed to confusing it with regular output.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: If a command fails, then the shell should display the exit status so the user knows what error occured. We could provide a way to merge them by merge redirection (2>&1), so that errors and output would output in same file, if it is necessary. However by default, STDERR and STDOUT should remain separate to prevent errors from mixing with normal output for debugging purposes.