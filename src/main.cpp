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


int main(int argc,char *argv[]) {
    DNS_TR_C dns_client("1.1.1.1");
    DNS_app x("127.0.0.1");
    //std::cout<<argv[1];
    //x.start_tr(argv[1]);
    DNS_TxRx(dns_client);

    return 0;
}