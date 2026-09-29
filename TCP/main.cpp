#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

int tcpServer()
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

    std::cout << "TCP等待连接...." << std::endl;

    int connfd = ::accept(sockfd, (struct sockaddr*)&addr2, &addrlen);

    std::cout << "有客户端口连接成功..." << std::endl;

    if (connfd != INVALID_SOCKET)
    {
        bool cycle = true;
        while (1)
        {
            char* message = (char*)malloc(100);
            std::cout << "输入数据: ";
            std::cin >> message;
            int len = strlen(message);
            std::cout << "您输入的数据是:" << message << "  数据长度为:" << strlen(message) << std::endl;

            message[len] = '\r';;
            message[len +1] = '\n';
            message[len + 2] = '\0';
            send(connfd, message, len+2, 0);

            free(message);
        }

    }
}

int tcpClient()
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
    inet_pton(AF_INET, "192.168.4.1", &addr.sin_addr);
    //    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8080);
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(sockaddr_in)) == -1)
    {
        std::cout << "connet error" << std::endl;
        return -1;
    }

    while (1)
    {
        char* message = (char*)malloc(100);
        std::cout << "输入数据: ";
        std::cin >> message;
        int len = strlen(message);
        std::cout << "您输入的数据是:" << message << "  数据长度为:" << strlen(message) << std::endl;

        message[len] = '\r';;
        message[len + 1] = '\n';
        message[len + 2] = '\0';
        send(sockfd, message, len + 2, 0);

        free(message);
    }

}

int main()
{
    tcpClient();
    return 0;
}
