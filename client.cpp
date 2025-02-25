#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    int Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(1234);
    SockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(Socket, (struct sockaddr*)(&SockAddr), sizeof(SockAddr)) != 0) {
        perror("connect");
        close(Socket);
        return 1;
    }

    char Buffer[1] = "";

    // {"command": "POST","size": 43,"from": "telnet","created_at": 1734851665,"level": "DEBUG","content": "order created ddsr"}

    char command_post[] = R"(
        { 
            "command": "POST",
            "size": 43,
            "from": "client_0",
            "created_at": 1734851665,
            "level": "DEBUG",
            "content": "order created ddsr" 
        })";

    char command_get[] = R"(
        { 
            "command": "GET",
            "from": "client_1",
            "created_from": 17590898665,
            "level": "DEBUG",
            "content": "order created" 
        })";
    // char content[] = R"({"from":"client_0"})";
    // ssize_t bytes_sent_command = write(Socket, command_post, strlen(command_post));
    ssize_t bytes_sent_command = write(Socket, command_get, strlen(command_get));
    if (bytes_sent_command == -1) {
        std::cerr << "Ошибка при отправке команды на сервер" << std::endl;
        close(Socket);
        return 0; 
    }

    if (read(Socket, Buffer, 2) == -1) {
        std::cout << "Ошибка при получении данных от сервера" << std::endl;  
    } else if (Buffer[0] == '0') 
    {
        // ssize_t bytes_sent = write(Socket, content, strlen(content));
        // if (bytes_sent == -1) {
        //     std::cerr << "Ошибка при отправке данных на сервер" << std::endl;
        //     close(Socket);
        //     return 0;
        // }
    } else {
        std::cout << "recv: " << Buffer << std::endl;
        std::cout << "От сервера не пришло подтверждение" << std::endl; 
    }

    close(Socket);
    return 0;
}