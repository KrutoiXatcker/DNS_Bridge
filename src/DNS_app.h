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
    
    DNS_app();

    ~DNS_app();

    void start_tr(std::string filename);

private:

    std::string Server_IP  = "127.0.0.1";

    // База данных доменов и IP-адресов
    std::unordered_map<std::string, std::string> hostnames;

    // Загрузка БД
    void bayt_cripted(const std::string& filename);

};


#endif