#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
/*
WORD == unsigned short(16 bits/2 bytes)
DWORD == unsigned long

int WSAStartup( WORD wVersionRequested, LPWSADATA IpWSAData ); Returns a pointer to the WSADATA struct in the IpWSAData param

typedef struct WSAData
{
    WORD wVersion;
    WORD wHighVersion;
    char szDescription[WSADESCRIPTION_LEN + 1];
    char szSystemStatus[WSASYS_STATUS_LEN + 1];
    unsigned short iMaxSockets;
    unsigned short iMaxUdpDg;
    char FAR *IpVendorInfo;
};

SOCKET WSAAPI socket( int af, int type, int protocol ); //af: AF_INET for UDP or TCP, type SOCK_DGRAM/STREAM, protocol IPPROTO_TCP
int bind(SOCKET s, const struct sockaddr* name, int socklen);

struct sockaddr_in
{
    short sin_family; //Address family Must be AF_INET
    unsigned short sin_port; //IP port
    struct in_addr sin_addr; //IP address
    char sin_zero[8]; //Padding

    //htons functions return the value in TCP/IP network byte order
};

int listen(SOCKET s, int backlog); backlog = max num of connections allowed (OS dependant)

SOCKET accept(SOCKET s, struct sockaddr* addr, int* addrlen);

*/

int main()
{
    std::cout << "=== Set up the DLL ===\n\n";
    //WSA initialization
    WSADATA wsaData; //Data, pass reference of this to startup func
    int wsaerr; //Error variable to return if success ( == 0) or error ( != 0)
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0)
    {
        std::cout << "The winsock dll not found" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "The winsock dll found!\n";
        std::cout << "The status: " << wsaData.szSystemStatus << '\n';
    }

    std::cout << '\n';
    std::cout << "=== Creating the socket ===\n\n";

    //Create socket
    SOCKET serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "socket() is OK!\n";
    }

    std::cout << '\n';
    std::cout << "=== Binding IP, port num ===\n\n";

    //Port num
    int port = 8080;
    const char* IP = "127.0.0.1";

    //Bind socket to Port num, IP
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(port);

    /*int*/inet_pton(AF_INET, IP, &service.sin_addr.s_addr);

    if (/*int*/bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cout << "bind() failed: " << WSAGetLastError() << '\n';
        /*int*/closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "bind() is OK!\n";
    }

    std::cout << '\n';
    std::cout << "=== Initislizing listening ===\n\n";

    //Listen socket
    int maxNumConnect = 3;
    if(/*int*/listen(serverSocket, maxNumConnect) == SOCKET_ERROR)
    {
        std::cout << "listen() failed: Error listening on socket " << WSAGetLastError() << '\n';
    }
    else
    {
        std::cout << "listen() is OK! Waiting for connections...\n";
    }

    std::cout << '\n';
    std::cout << "=== Accepting connection ===\n\n";

    //Accept connection
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, nullptr, nullptr);
    if (acceptSocket == INVALID_SOCKET)
    {
        std::cout << "accept() failed: " << WSAGetLastError() << '\n';
        WSACleanup();
        return -1;
    }

    //Close socket
    WSACleanup();
    closesocket(serverSocket);
}
