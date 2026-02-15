#pragma once

#ifdef _WIN32
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

#ifdef _WIN32
using socket_t = SOCKET;
#else
using socket_t = int;
#endif

// Socket initialization and cleanup
bool init_sockets();
void cleanup_sockets();
void close_socket(socket_t s);
