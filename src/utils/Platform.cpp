#include "Platform.h"

#ifdef _WIN32
bool init_sockets(){
    WSADATA wsa{};
    return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
}

void cleanup_sockets(){ 
    WSACleanup(); 
}

void close_socket(socket_t s){ 
    closesocket(s); 
}
#else
bool init_sockets(){ 
    return true; 
}

void cleanup_sockets(){}

void close_socket(socket_t s){ 
    close(s); 
}
#endif
