/*
g++ main.cpp -o main --std=c++20 -I/opt/homebrew/Cellar/nlohmann-json/3.11.3/include -I/opt/homebrew/Cellar/boost/1.87.0/include/boost

{
    "command": "POST",
    "from": "client_0",
    "created_at": 1739468665,
    "level": "DEBUG",
    "content": "order created"
}

{
    "command": "GET",
    "created_from": 1739468665,
    "created_to": 1739478665,
    "level": ["DEBUG", "INFO"],
    "from": "client_0",
    "content": "created"
}

*/

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "services_func.cpp"
#include "Log_item.cpp"
#include "Command_item.cpp"
#include "command_get.cpp"
#include "command_post.cpp"

#define PORT 1234
#define BUFFER_SIZE 1024

//чтобы определить, какую команду запросили
unsigned get_command(json& result_parse) {
    std::string post = "POST";
    std::string get = "GET";
    if (result_parse.contains("command")) {
        std::string command = result_parse["command"].get<std::string>(); 
        std::cout << "command: " << command << std::endl;

        if (command == post && check_post_request(result_parse)) {
            Log_item log_item = creat_log_item(result_parse);
            save_log(std::move(log_item));
            return 0;
        } else if (command == get) {
            Command_item command = creat_get_command(result_parse);
            find_log(command);
            return 0;
        }
    }

    return -1;
}

void handle_client(int client_socket) {
    while(true) {
        char buffer[BUFFER_SIZE];
        try {
            ssize_t bytes_read_to_connect = read(client_socket, buffer, sizeof(buffer));
            if (bytes_read_to_connect == -1) {
                // std::cerr << "0 - Ошибка при чтении данных от клиента" << std::endl;
                close(client_socket);
                return;
            }
            json result_parse = parse_json(buffer);
            unsigned result = get_command(result_parse); 
            
            if (result == -1) {
                close(client_socket);
                return;
            }

            const char response_OK[] = "0";
            ssize_t bytes_sent = write(client_socket, response_OK, strlen(response_OK));
            if (bytes_sent == -1) {
                std::cerr << "1 - Ошибка при отправке клиенту 0" << std::endl; 
                close(client_socket);
                return; 
            }
        } catch (const json::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Ошибка при обработке запроса" << std::endl;
        }
        close(client_socket); 
    }
}

void test() {
    using namespace boost::gregorian;

    // Создание объектов дат
    // date date1(2023, Oct, 15);
    // date date2(2023, Nov, 1);
    date date1(from_string("2025-02-11"));
    date date2(2023, Nov, 1);

    // Сравнение дат
    if (date1 < date2) {
        std::cout << "Первая дата раньше второй." << std::endl;
    } else if (date1 > date2) {
        std::cout << "Вторая дата раньше первой." << std::endl;
    } else {
        std::cout << "Даты равны." << std::endl;
    }
}

int start() {
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket == -1) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return -1;
    }

    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(PORT);
    SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr*)(&SockAddr), sizeof(SockAddr)) == -1) {
        std::cerr << "Ошибка при привязке сокета" << std::endl;
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, 5) == -1) {
        std::cerr << "Ошибка при прослушивании сокета" << std::endl;
        close(server_socket);
        return -1;
    }

    std::cout << "Сервер запущен и ожидает соединений на порту " << PORT << std::endl;

    std::vector<std::thread> threads;
    threads.reserve(20);
    while (true) {
        int client_socket = accept(server_socket, 0, 0);
        if (client_socket == -1) {
            std::cerr << "Ошибка при принятии соединения" << std::endl;
            close(server_socket);
            continue;;
        }

        threads.emplace_back(handle_client, client_socket);
    }

    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    
    close(server_socket);
}

int main() {

    // test();
    start();
        
    return 0;
}