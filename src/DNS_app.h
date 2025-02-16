#ifndef DNS_app_
#define DNS_app_


#include <iostream>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include "DNS_TR_C.h"

class DNS_app{

public:
    
    DNS_app();

    ~DNS_app();

    void start_tr(std::string filename);

private:

    // База данных доменов и IP-адресов
    std::unordered_map<std::string, std::string> hostnames;

    // Загрузка БД
    void bayt_cripted(const std::string& filename);

};


#endif