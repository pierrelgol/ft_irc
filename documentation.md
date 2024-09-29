
Here’s a detailed breakdown of each function you're allowed to use, along with how they’re typically used and what pitfalls to avoid in C++98:

### **1. `socket()`**
- **What it does**: Creates an endpoint for communication and returns a file descriptor for the socket.
- **Usage**: 
  ```cpp
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ```
  - `AF_INET` specifies IPv4.
  - `SOCK_STREAM` specifies a TCP connection.
  - The third argument (protocol) is usually 0.
- **Pitfalls**: Check for return value `-1`, which indicates an error. Socket creation can fail due to resource limitations or incorrect parameters.

### **2. `close()`**
- **What it does**: Closes a file descriptor (including a socket).
- **Usage**: 
  ```cpp
  close(sockfd);
  ```
- **Pitfalls**: After closing, don’t reuse the socket without reinitializing it. Also, handle `close()` carefully to avoid leaving connections open unintentionally.

### **3. `setsockopt()`**
- **What it does**: Configures socket options (like setting a timeout or reusing addresses).
- **Usage**: 
  ```cpp
  int opt = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  ```
- **Pitfalls**: Ensure you provide the correct option type for the level you’re configuring (e.g., `SOL_SOCKET` vs `IPPROTO_TCP`). Misuse can lead to errors or unexpected behavior.

### **4. `getsockname()`**
- **What it does**: Retrieves the local address bound to a socket.
- **Usage**: 
  ```cpp
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);
  getsockname(sockfd, (struct sockaddr *)&addr, &addr_len);
  ```
- **Pitfalls**: Ensure you pass the correct structure and allocate enough space. Check return values for errors.

### **5. `getprotobyname()`**
- **What it does**: Retrieves information about a protocol by name (like "tcp" or "udp").
- **Usage**: 
  ```cpp
  struct protoent *proto = getprotobyname("tcp");
  ```
- **Pitfalls**: This function is not thread-safe, and errors can arise if the protocol is not found.

### **6. `gethostbyname()`**
- **What it does**: Resolves a hostname to an IP address.
- **Usage**:
  ```cpp
  struct hostent *host = gethostbyname("example.com");
  ```
- **Pitfalls**: This function is obsolete and not thread-safe. Prefer using `getaddrinfo()` for newer systems.

### **7. `getaddrinfo()`**
- **What it does**: Resolves a host and service into an address structure, supporting both IPv4 and IPv6.
- **Usage**:
  ```cpp
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;  // or AF_UNSPEC for IPv4/IPv6
  getaddrinfo("example.com", "80", &hints, &res);
  ```
- **Pitfalls**: Ensure you free the memory with `freeaddrinfo()` after usage. Incorrect hint settings can lead to resolution failures.

### **8. `freeaddrinfo()`**
- **What it does**: Frees memory allocated by `getaddrinfo()`.
- **Usage**:
  ```cpp
  freeaddrinfo(res);
  ```
- **Pitfalls**: Forgetting to free the address info can cause memory leaks.

### **9. `bind()`**
- **What it does**: Assigns a local address (IP and port) to a socket.
- **Usage**:
  ```cpp
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  ```
- **Pitfalls**: Binding to a port already in use will fail. You may want to use `setsockopt()` with `SO_REUSEADDR` to allow address reuse.

### **10. `connect()`**
- **What it does**: Establishes a connection to a remote server.
- **Usage**:
  ```cpp
  connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  ```
- **Pitfalls**: Non-blocking connections may require special handling if the connection is not immediately established.

### **11. `listen()`**
- **What it does**: Marks a socket as passive, meaning it will accept incoming connections.
- **Usage**:
  ```cpp
  listen(sockfd, 10);  // 10 is the backlog, max queued connections
  ```
- **Pitfalls**: Forgetting to call `listen()` on a server socket will prevent connections from being accepted.

### **12. `accept()`**
- **What it does**: Accepts an incoming connection and returns a new socket descriptor for the connection.
- **Usage**:
  ```cpp
  int client_sock = accept(sockfd, NULL, NULL);
  ```
- **Pitfalls**: Always check for a return value of `-1`. If non-blocking I/O is enabled, handle `EAGAIN` or `EWOULDBLOCK`.

### **13. `htons()` / `htonl()`**
- **What it does**: Converts host byte order to network byte order (short/long).
- **Usage**:
  ```cpp
  uint16_t port = htons(8080);
  uint32_t ip = htonl(INADDR_LOOPBACK);
  ```
- **Pitfalls**: Ensure you always convert data like port numbers and IP addresses before sending over the network.

### **14. `ntohs()` / `ntohl()`**
- **What it does**: Converts network byte order to host byte order (short/long).
- **Usage**:
  ```cpp
  uint16_t port = ntohs(addr.sin_port);
  uint32_t ip = ntohl(addr.sin_addr.s_addr);
  ```

### **15. `inet_addr()`**
- **What it does**: Converts a string IP address (e.g., "127.0.0.1") to a network byte order address.
- **Usage**:
  ```cpp
  uint32_t addr = inet_addr("127.0.0.1");
  ```
- **Pitfalls**: If the IP is invalid, this returns `INADDR_NONE` which is indistinguishable from a valid address.

### **16. `inet_ntoa()`**
- **What it does**: Converts an IP address in network byte order to a string.
- **Usage**:
  ```cpp
  char *ip_str = inet_ntoa(addr.sin_addr);
  ```
- **Pitfalls**: `inet_ntoa()` is not thread-safe, and the returned string is statically allocated, so be careful with concurrency.

### **17. `send()`**
- **What it does**: Sends data on a connected socket.
- **Usage**:
  ```cpp
  send(sockfd, buffer, sizeof(buffer), 0);
  ```
- **Pitfalls**: Be prepared to handle partial sends, which may require looping until all data is sent.

### **18. `recv()`**
- **What it does**: Receives data from a connected socket.
- **Usage**:
  ```cpp
  int nbytes = recv(sockfd, buffer, sizeof(buffer), 0);
  ```
- **Pitfalls**: Similar to `send()`, `recv()` might not receive all available data in one call. Also, handle return values like `0` (connection closed) and `-1` (error).

### **19. `signal()`**
- **What it does**: Sets up a handler for a specific signal.
- **Usage**:
  ```cpp
  signal(SIGINT, signal_handler);
  ```
- **Pitfalls**: Signal handling can interfere with normal control flow. Be cautious when dealing with system-level interrupts.

### **20. `sigaction()`**
- **What it does**: A more advanced way to handle signals than `signal()`. It allows fine control over signal behavior.
- **Usage**:
  ```cpp
  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sigaction(SIGINT, &sa, NULL);
  ```
- **Pitfalls**: Using `sigaction()` properly can avoid race conditions that `signal()` can cause.

### **21. `lseek()`**
- **What it does**: Moves the file pointer to a new location in a file descriptor.
- **Usage**:
  ```cpp
  off_t offset = lseek(fd, 0, SEEK_SET);
  ```
- **Pitfalls**: Always check the return value for `-1` to ensure the seek operation was successful.

### **22. `fstat()`**
- **What it does**: Retrieves information about a file based on its file descriptor.
- **Usage**:
  ```cpp
  struct stat file_stat;
  fstat(fd, &file_stat);
  ```
- **Pitfalls**: Ensure the file descriptor is valid, and handle errors.

### **23. `fcntl()`**
- **What it does**: Manipulates file descriptor flags, often used for setting non-blocking mode.
- **Usage**:
  ```cpp
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
  ```
- **Pitfalls**: Improper use can lead to unexpected behavior, especially when dealing with non-blocking I/O.

### **24. `poll()` (or equivalent)**
- **What it does**: Monitors multiple file descriptors to see if they’re ready for I/O.
- **Usage**:

```cpp
  struct pollfd fds[2];
  fds[0].fd = sockfd;
  fds[0].events = POLLIN;
  poll(fds, 2, timeout);
```

- **Pitfalls**: Ensure proper event masking and handle timeouts to avoid infinite blocking.
