1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  `fork` allows you to make a seperate child process by replicating the parent process virtual memory space, so calling execvp only would replace the current process image, in this case the shell itself, preventing it from being able to handle multiple commands. `fork` can return 3 values that are useful for control flow:
    1. postive value (PID): we are in the parent process allowing the shell to wait for child process to complete before resuming
    2. negative value (child process fails): fork failed
    3. zero (child process sucessfully created): we are in the child process where `execvp` should be called to execute the command

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails, it returns a negative value, showing that the a new child process was unable to be created. I handle this by checking the return value of fork(). If it is negative, I use perror() to notify the user of the error and then exit the shell to prevent further execution.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() finds the command to execute from searching through the directories specified in the PATH environment variable. The PATH variable has a list of directories where executable files are located, so execvp() checks each directory in PATH in order, trying to construct a valid full path to the executable. So, if a match is found, execvp() will execute the command, otherwise an error is returned as execvp() was unable to find the command.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Calling wait() allows for the proper management of child processes, as it tells the parent process to wait for the child process to complete before resuming execution. Thus allowing the parent to collect the child's exit status and ensures that system resources are properly deallocated. If wait() is not called, the child processes may become zombies, leading to resource leaks and possibly system instability.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() gets the exit status of a child process after it terminates. It is important because it allows the parent process to figure out whether the executed command was successful or if theres an error. The parent process would not have any way of knowing the outcome of the command execution without WEXITSTATUS(), thus making it harder to handle errors or take appropriate actions based on the result.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My implementation of build_cmd_buff() ensures quoted arguments are treated as single arguments whilst preserving spaces within quotes, by detecting " (quote) characters, marking the start of a quoted arg, and continuing until the closing " thus maintaining spaces inside the quotes. It is necessary to prevent commands like echo "hello world" from being incorrectly split into multiple arguments.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: The previous parsing logic was based off of pipe seperation, so changes involved shifting from parsing a command list to parsing a single command with arguments, and the addition of quote handling. No challenges.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Linux signals are asynchronous notifications to processes about events like user input/errors. Signals are designed for simple event notification compared to other forms of IPC like pipes that are utilized for data exchange. They transmit a signal number, not data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:
    1. SIGKILL (9): Immediate process termination, cannot be caught or ignored. Use: To stop a program immediately
    2. SIGTERM (15): Requests graceful process termination. Use: To clean up prior to terminating
    3. SIGINT (2): Interrupts a process, pressing Ctrl+C in the terminal. Use: To request to stop a program, or have different actions specified in a user defined signal handler

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, it is immediately suspended and cannot be caught or ignored. SIGSTOP is a special signal that bypasses normal signal handling mechanisms, ensuring that processes can always be stopped by the operating system or users.