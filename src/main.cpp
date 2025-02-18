#include <iostream>
#include "DNS_TR_C.h"
#include "DNS_app.h"

void DNS_TxRx(DNS_TR_C& dns_client){
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
}

int main() {
    DNS_TR_C dns_client("1.1.1.1");
    //DNS_app x;
    //x.start_tr("111");

    DNS_TxRx(dns_client);

    return 0;
}