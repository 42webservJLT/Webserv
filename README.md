# Webserv
Webserv Project of the 42 Core Curriculum


the following functions are allowed:

### 1. **execve**
The `execve` function is used to execute a program. It replaces the current process with a new program.

#### Prototype:
```cpp
int execve(const char *pathname, char *const argv[], char *const envp[]);
```

- `pathname`: The path of the program to be executed.
- `argv`: An array of arguments to pass to the new program (the first element is the program's name).
- `envp`: An array of environment variables for the new program.

#### Example:
```cpp
#include <unistd.h>

int main() {
    char *args[] = {"/bin/ls", "-l", NULL};
    execve("/bin/ls", args, NULL);  // Replaces current process with 'ls'
    return 0;
}
```

---

### 2. **dup**
The `dup` function creates a copy of an existing file descriptor.

#### Prototype:
```cpp
int dup(int oldfd);
```

- `oldfd`: The file descriptor to be duplicated.

#### Example:
```cpp
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    int new_fd = dup(fd);  // Duplicates the file descriptor
    close(fd);
    return 0;
}
```

---

### 3. **dup2**
The `dup2` function is similar to `dup`, but you can specify the new file descriptor.

#### Prototype:
```cpp
int dup2(int oldfd, int newfd);
```

- `oldfd`: The file descriptor to be duplicated.
- `newfd`: The new file descriptor.

#### Example:
```cpp
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    dup2(fd, STDOUT_FILENO);  // Redirects stdout to the file
    close(fd);
    return 0;
}
```

---

### 4. **pipe**
The `pipe` function creates a pair of file descriptors that are connected. Data written to the first descriptor can be read from the second.

#### Prototype:
```cpp
int pipe(int pipefd[2]);
```

- `pipefd[0]`: The reading end.
- `pipefd[1]`: The writing end.

#### Example:
```cpp
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    int pipefd[2];
    pipe(pipefd);
    write(pipefd[1], "Hello", 5);
    char buffer[5];
    read(pipefd[0], buffer, 5);
    printf("%s\n", buffer);  // Prints "Hello"
    return 0;
}
```

---

### 5. **strerror**
The `strerror` function returns a pointer to a string that describes the error code passed as an argument.

#### Prototype:
```cpp
char *strerror(int errnum);
```

- `errnum`: The error code (typically from `errno`).

#### Example:
```cpp
#include <stdio.h>
#include <errno.h>

int main() {
    printf("%s\n", strerror(errno));  // Prints a description of the last error
    return 0;
}
```

---

### 6. **gai_strerror**
This function provides a textual description for error codes returned by the `getaddrinfo` function.

#### Prototype:
```cpp
char *gai_strerror(int errcode);
```

- `errcode`: The error code returned by `getaddrinfo`.

#### Example:
```cpp
#include <stdio.h>
#include <netdb.h>

int main() {
    int error = EAI_NONAME;
    printf("%s\n", gai_strerror(error));  // "Name or service not known"
    return 0;
}
```

---

### 7. **errno**
`errno` is a global variable that holds the error code set by system calls or library functions when an error occurs.

#### Example:
```cpp
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int fd = open("nonexistentfile.txt", O_RDONLY);
    if (fd == -1) {
        printf("Error: %s\n", strerror(errno));  // Prints error message
    }
    return 0;
}
```

---

### 8. **fork**
The `fork` function creates a new process by duplicating the calling process. The new process is called the child process.

#### Prototype:
```cpp
pid_t fork(void);
```

- Returns `0` in the child process, and the child's PID in the parent.

#### Example:
```cpp
#include <unistd.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child Process\n");
    } else {
        printf("Parent Process\n");
    }
    return 0;
}
```

---

### 9. **socketpair**
The `socketpair` function creates a pair of connected sockets.

#### Prototype:
```cpp
int socketpair(int domain, int type, int protocol, int sv[2]);
```

- `domain`: The communication domain (e.g., `AF_UNIX`).
- `type`: The type of socket (e.g., `SOCK_STREAM`).
- `protocol`: The protocol (e.g., `0` for default).
- `sv[2]`: Array of two file descriptors for the socket pair.

#### Example:
```cpp
#include <sys/socket.h>
#include <stdio.h>

int main() {
    int sv[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);  // Creates a pair of connected sockets
    write(sv[0], "Message", 7);
    char buffer[8];
    read(sv[1], buffer, 7);
    printf("%s\n", buffer);  // Prints "Message"
    return 0;
}
```

---

### 10. **htons**
The `htons` function converts a 16-bit number from host byte order to network byte order.

#### Prototype:
```cpp
uint16_t htons(uint16_t hostshort);
```

#### Example:
```cpp
#include <netinet/in.h>
#include <stdio.h>

int main() {
    uint16_t hostshort = 12345;
    uint16_t netshort = htons(hostshort);
    printf("Network byte order: %u\n", netshort);
    return 0;
}
```

---

### 11. **htonl**
The `htonl` function converts a 32-bit number from host byte order to network byte order.

#### Prototype:
```cpp
uint32_t htonl(uint32_t hostlong);
```

#### Example:
```cpp
#include <netinet/in.h>
#include <stdio.h>

int main() {
    uint32_t hostlong = 123456789;
    uint32_t netlong = htonl(hostlong);
    printf("Network byte order: %u\n", netlong);
    return 0;
}
```

---

### 12. **ntohs**
The `ntohs` function converts a 16-bit number from network byte order to host byte order.

#### Prototype:
```cpp
uint16_t ntohs(uint16_t netshort);
```

#### Example:
```cpp
#include <netinet/in.h>
#include <stdio.h>

int main() {
    uint16_t netshort = 12345;
    uint16_t hostshort = ntohs(netshort);
    printf("Host byte order: %u\n", hostshort);
    return 0;
}
```

---

### 13. **ntohl**
The `ntohl` function converts a 32-bit number from network byte order to host byte order.

#### Prototype:
```cpp
uint32_t ntohl(uint32_t netlong);
```

#### Example:
```cpp
#include <netinet/in.h>
#include <stdio.h>

int main() {
    uint32_t netlong = 123456789;
    uint32_t hostlong = ntohl(netlong);
    printf("Host byte order: %u\n", hostlong);
    return 0;
}
```

---

Certainly! Let’s continue from where we left off. I’ll pick up from the `select` function and keep going with the remaining functions.

---

### 14. **select (continued)**
#### Example (continued):
```cpp
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);  // Add STDIN to the read set
    struct timeval timeout = {5, 0};  // Timeout of 5 seconds
    int result = select(1, &readfds, NULL, NULL, &timeout);

    if (result > 0) {
        printf("Data is ready to read\n");
    } else if (result == 0) {
        printf("Timeout occurred\n");
    } else {
        perror("select error");
    }
    return 0;
}
```

---

### 15. **poll**
The `poll` function monitors multiple file descriptors, like `select`, but provides more flexibility, such as monitoring events more efficiently.

#### Prototype:
```cpp
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

- `fds`: Array of `pollfd` structures, each representing a file descriptor and the events to be monitored.
- `nfds`: The number of file descriptors.
- `timeout`: Timeout in milliseconds.

#### Example:
```cpp
#include <poll.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct pollfd pfd = { STDIN_FILENO, POLLIN, 0 };
    int ret = poll(&pfd, 1, 5000);  // Timeout of 5000 ms (5 seconds)
    if (ret > 0 && (pfd.revents & POLLIN)) {
        printf("Data is available to read\n");
    } else {
        printf("Timeout or no data\n");
    }
    return 0;
}
```

---

### 16. **epoll**
`epoll` is a more scalable I/O event notification mechanism than `select` or `poll` for handling large numbers of file descriptors.

#### Functions:
- `epoll_create`: Creates an epoll instance.
- `epoll_ctl`: Controls the behavior of the epoll instance.
- `epoll_wait`: Waits for events on the epoll instance.

#### Example:
```cpp
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_EVENTS 10

int main() {
    int epollfd = epoll_create(1);  // Create an epoll instance
    struct epoll_event ev, events[MAX_EVENTS];

    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);  // Add stdin to epoll

    int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);  // Wait indefinitely
    if (nfds > 0) {
        printf("Data ready to read\n");
    }

    close(epollfd);
    return 0;
}
```

---

### 17. **kqueue**
`kqueue` is an event notification interface used in BSD-based systems (like macOS) that scales better than `select` or `poll`.

#### Functions:
- `kqueue`: Creates an event queue.
- `kevent`: Registers events or waits for events.

#### Example:
```cpp
#include <sys/types.h>
#include <sys/event.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int kq = kqueue();
    struct kevent change;
    EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

    kevent(kq, &change, 1, NULL, 0, NULL);  // Add event to kqueue

    struct kevent event;
    kevent(kq, NULL, 0, &event, 1, NULL);  // Wait for event

    printf("Data ready to read\n");
    close(kq);
    return 0;
}
```

---

### 18. **socket**
The `socket` function creates a socket for communication.

#### Prototype:
```cpp
int socket(int domain, int type, int protocol);
```

- `domain`: Communication domain (e.g., `AF_INET` for IPv4).
- `type`: Socket type (e.g., `SOCK_STREAM` for TCP).
- `protocol`: Protocol (typically 0 for default).

#### Example:
```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return -1;
    }
    printf("Socket created successfully\n");
    return 0;
}
```

---

### 19. **accept**
The `accept` function accepts a new connection on a socket.

#### Prototype:
```cpp
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

- `sockfd`: The socket file descriptor.
- `addr`: A pointer to the address structure where the address of the connecting client will be stored.
- `addrlen`: On entry, it is the size of the address structure; on exit, it is the actual size.

#### Example:
```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = { AF_INET, htons(8080), INADDR_ANY };
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    int client_fd = accept(server_fd, NULL, NULL);  // Accept incoming connections
    printf("Connection accepted\n");

    close(client_fd);
    close(server_fd);
    return 0;
}
```

---

### 20. **listen**
The `listen` function listens for incoming connections on a socket.

#### Prototype:
```cpp
int listen(int sockfd, int backlog);
```

- `sockfd`: The socket file descriptor.
- `backlog`: The maximum number of pending connections that can be queued.

#### Example:
```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = { AF_INET, htons(8080), INADDR_ANY };
    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    listen(sockfd, 5);  // Listen for connections
    printf("Server listening on port 8080\n");

    close(sockfd);
    return 0;
}
```

---

### 21. **send**
The `send` function sends data over a socket.

#### Prototype:
```cpp
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

- `sockfd`: The socket file descriptor.
- `buf`: The data buffer.
- `len`: The length of the data.
- `flags`: Flags for the operation.

#### Example:
```cpp
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    send(sockfd, "Hello", 5, 0);  // Sends "Hello" over the socket
    close(sockfd);
    return 0;
}
```

---

### 22. **recv**
The `recv` function receives data from a socket.

#### Prototype:
```cpp
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

- `sockfd`: The socket file descriptor.
- `buf`: The buffer to store the received data.
- `len`: The size of the buffer.
- `flags`: Flags for the operation.

#### Example:
```cpp
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    char buffer[100];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    recv(sockfd, buffer, 100, 0);  // Receives data into buffer
    printf("Received: %s\n", buffer);
    close(sockfd);
    return 0;
}
```

---

### 23. **chdir**
The `chdir` function changes the current working directory.

#### Prototype:
```cpp
int chdir(const char *path);
```

#### Example:
```cpp
#include <unistd.h>
#include <stdio.h>

int main() {
    chdir("/home/user");
    printf("Current directory changed\n");
    return 0;
}
```

---

### 24. **bind**
The `bind` function associates a socket with an address.

#### Prototype:
```cpp
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

#### Example:
```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = { AF_INET, htons(8080), INADDR_ANY };
    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    printf("Socket bound to address\n");
    return 0;
}
```

---



### 25. **connect**
The `connect` function connects a socket to a remote address.

#### Prototype:
```cpp
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

#### Example:
```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = { AF_INET, htons(8080), INADDR_ANY };
    connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));  // Connect to server
    printf("Connected to server\n");
    return 0;
}
```

---

### 26. **getaddrinfo**
The `getaddrinfo` function retrieves address information for a hostname.

#### Prototype:
```cpp
int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
```

#### Example:
```cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

int main() {
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    struct addrinfo *result;
    getaddrinfo("example.com", "http", &hints, &result);
    printf("Address info retrieved\n");
    freeaddrinfo(result);
    return 0;
}
```

---

### 27. **freeaddrinfo**
The `freeaddrinfo` function frees the memory allocated by `getaddrinfo`.

#### Example:
```cpp
freeaddrinfo(result);  // Frees the memory allocated for addrinfo structure
```

---

### 28. **setsockopt**
The `setsockopt` function sets options for a socket.

#### Prototype:
```cpp
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```

#### Example:
```cpp
#include <sys/socket.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    printf("Socket option set\n");
    return 0;
}
```

---

This is a comprehensive overview of the system calls and functions.