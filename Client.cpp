#define _WIN32_WINNT 0x0600
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

int main()
{
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);

    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0)
    {
        std::cout << "WSAStartup failed: winsock dll not found!\n";
        return 0;
    }
    else
    {
        std::cout << "winsock dll found!\n";
        std::cout << "The status: " << wsaData.szSystemStatus << '\n';
    }

    //Create socket
    SOCKET clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "socket() failed: " << WSAGetLastError() << '\n';
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "socket() is OK!\n";
    }

    //Connect
    const char* IP = "127.0.0.1";
    int port = 8080;

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(port);

    inet_pton(AF_INET, IP, &clientService.sin_addr.s_addr);

    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        std::cout << "Client: Failed to connect: " << WSAGetLastError() << '\n';
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "Client: Successfully connected!";
        std::cout << "Client: Can start sending and receiving data...\n";
    }

    return 0;
}

