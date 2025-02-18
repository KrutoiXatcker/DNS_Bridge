#ifndef DNS_TR_C_H
#define DNS_TR_C_H

#include <unordered_map>
#include <arpa/inet.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


class DNS_TR_C {

private:
    // Структура DNS-заголовка
    struct DNSHeader {
        uint16_t id;
        uint16_t flags;
        uint16_t qdcount;
        uint16_t ancount;
        uint16_t nscount;
        uint16_t arcount;
    };

    int sock;                       // Сокет для UDP-соединения
    sockaddr_in server_addr;        // Адрес DNS-сервера
    std::vector<uint8_t> response;  // Буфер для ответа
    std::string domain;             // Доменное имя для запроса
    std::string server_ip;          // IP DNS-сервера
    std::vector<uint8_t> query;     // DNS-запрос


public:

    //  Конструктор принимает адрес DNS сервера
    DNS_TR_C(std::string server_IP);

    // Деструктор
    ~DNS_TR_C();

    // Отправка DNS-запроса
    void send_query();

    // Получение DNS-ответа
    void resive_query();

    // Геттер для доменного имени
    std::string getDomain() const;

    // Геттер для ответа (response)
    std::vector<uint8_t> getResponse() const;

    // Сеттер для доменного имени 
    void setDomain(std::string move);

    // Формирование DNS-запроса
    std::vector<uint8_t> build_dns_query(const std::string& domain);

    // Разбор DNS-ответа
    std::string parse_dns_response(const std::vector<uint8_t>& response);
};

#endif // DNS_TR_C_H