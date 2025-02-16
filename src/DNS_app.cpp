#include "DNS_app.h"

DNS_app::DNS_app(){
    bayt_cripted("domane _p.txt");
}

DNS_app::~DNS_app(){

}

// Загрузка базы данных доменов из файла
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

void DNS_app::start_tr(std::string filename){
    std::ifstream file(filename, std::ios::binary);
    DNS_TR_C send;
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
        //send.resive_query();
       
    }

    send.setDomain(hostnames["257"]);
    send.send_query();

    // Закрываем файл
    file.close();
    
    return;
}