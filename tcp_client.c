// Socket connection: Exactly same as that of server’s socket creation

// The connect() system call connects the socket referred to by the file
// descriptor sockfd to the address specified by addr. Server’s address
// and port is specified in addr.
int connect(
    int sockfd,
    const struct sockaddr *addr,
    socklen_t addrlen);