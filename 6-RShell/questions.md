1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?
The remote client checks for the EOF character 0x04 at the end of the command's output, which is done by a recv() loop that will continue recieving data until it matches the EOF character. To handle partial reads or ensure complete message transmission we can use a fixed buffer size, loop recv() call, or even checking for when recv() equals 0 as the connection has been closed.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol could use defined delimiters to detect the beginning and end of a command sent over a TCP connection, i.e the null terminator character from commands sent from the client to server and EOF character for server to client. If handled incorrectly, there could be incomplete commands, or concatenated commands, all of which would not have intended behavior.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain information from previous interactions, essentially keeping a history of information that could be used again in the future. Stateless protocols do not maintain information between interactions, thus it has every request as independent of one another. A stateful protocol is more relaible and robust as it has a history, whereas stateless protcols can scale better as each request is independent. 

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP does not ensure packet order or error correction, thus it is useful when efficiency is valued over reliability and this prevalent in many streaming, gaming, and video communication services.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The Sockets API is the interface that is provided by the operating system to enable network communication using syscalls. Functions we utilized from Sockets API in this assignment were socket(), connect(), send(), recv(), bind, listen(), and more.