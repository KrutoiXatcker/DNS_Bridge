#ifndef DNS_app_
#define DNS_app_

#include <unordered_map>
#include <arpa/inet.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "DNS_TR_C.h"


class DNS_app{

public:
    
    DNS_app(std::string server_ip);

    ~DNS_app();

    void setServer_IP(const std::string& server_ip);

    void start_tr(const std::string filename);

private:
    // IP - адресс сервера
    std::string Server_IP = "127.0.0.1";

    //  Имя базы данных
    std::string db_name;

    // База данных доменов и IP-адресов
    std::unordered_map<std::string, std::string> hostnames;

    // Загрузка БД
    void bayt_cripted(const std::string& filename);

};


#endif