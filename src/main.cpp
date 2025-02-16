#include <iostream>
#include "DNS_TR_C.h"
#include "DNS_app.h"

int main() {
    DNS_TR_C dns_client;
    DNS_app x;
    x.start_tr("111");
    
    /*
    for(;;){
        std::string domain_name;
        std::cin>>domain_name;
        dns_client.setDomain(domain_name);

        try {

            dns_client.send_query();
            dns_client.resive_query();
           
            // Разбираем ответ
            std::string ip = dns_client.parse_dns_response(dns_client.getResponse());
            std::cout << "Resolved IP address for " << dns_client.getDomain()<< ": " << ip << std::endl;
    
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    */

    return 0;
}