#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

int main()
{

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup 失败，错误码: " << result << std::endl;
        return -1;
    }
    std::cout << "✅ Winsock 初始化成功" << std::endl;

    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   
   // unsigned long ul = 1;
   // int ret = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);


    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    //    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8088);

    if (::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        // 获取详细错误
        int err = WSAGetLastError();
        std::cout << "bind失败，错误码: " << err << std::endl;
        if (err == WSAEADDRINUSE) {
            std::cout << "端口8088已被占用！" << std::endl;
        }
        return false;
    }

    if (::listen(sockfd, 10) < 0) {
        return false;
    }

    struct sockaddr_in addr2 = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int connfd = ::accept(sockfd, (struct sockaddr*)&addr2, &addrlen);

    if (connfd != INVALID_SOCKET)
    {

        while (1)
        {
            Sleep(5000);
            send(connfd, (char*)"111", 3, 0);
        }

    }

}
