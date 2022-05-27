#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int main() {
    SOCKET clientSock;
    int port = 8000;
    WSADATA wsaData;
    int wsaErr;
    WORD wVerReq = MAKEWORD(2, 2);
    wsaErr = WSAStartup(wVerReq, &wsaData);
    if (wsaErr != 0) {
        cout << "Sinsock dll not found" << endl;
        return 0;
    }
    else {
        cout << "Winsock dll found" << endl;
        cout << "Status: " << wsaData.szSystemStatus << endl;
    }


    clientSock = INVALID_SOCKET;
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock == INVALID_SOCKET) {
        cout << "socket() failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket() is ok" << endl;
    }


    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);
    if (connect(clientSock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "connect() failed" << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "connect() is ok" << endl;
    }
    system("pause");
    WSACleanup();
    return 0;
}

