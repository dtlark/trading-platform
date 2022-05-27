#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char* argv[]) {
    SOCKET serverSock, acceptSock;
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


    serverSock = INVALID_SOCKET;
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == INVALID_SOCKET) {
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
    if (bind(serverSock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "bind() failed" << WSAGetLastError() << endl;
        closesocket(serverSock);
        WSACleanup();
        return 0;
    }
    else {
        cout << "bind() is ok" << endl;
    }


    if (listen(serverSock, 1) == SOCKET_ERROR) cout << "listen() failed: " << WSAGetLastError() << endl;
    else cout << "listen() is ok" << endl;


    acceptSock = accept(serverSock, NULL, NULL);
    if (acceptSock == INVALID_SOCKET) {
        cout << "accept() failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }
    cout << "Accepted connection" << endl;
    system("pause");
    WSACleanup();


    return 0;
}

