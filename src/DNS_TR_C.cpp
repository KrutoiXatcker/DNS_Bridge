#include "DNS_TR_C.h"

// Конструктор
DNS_TR_C::DNS_TR_C()
    : domain("google.com"),
      server_ip("192.168.1.7"),
      //server_ip("8.8.8.8"),
      sock(socket(AF_INET, SOCK_DGRAM, 0)) {
    if (sock < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Настройка адреса DNS-сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(53);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    // Формирование DNS-запроса
    query = build_dns_query(domain);
}

// Деструктор
DNS_TR_C::~DNS_TR_C() {
    if (sock >= 0) {
        close(sock);
    }
}

// Формирование DNS-запроса
std::vector<uint8_t> DNS_TR_C::build_dns_query(const std::string& domain) {
    std::vector<uint8_t> packet;

    // Заголовок
    DNSHeader header;
    header.id = htons(0x1234); // Идентификатор запроса
    header.flags = htons(0x0100); // Флаги (стандартный запрос)
    header.qdcount = htons(1); // Один вопрос
    header.ancount = 0;
    header.nscount = 0;
    header.arcount = 0;

    // Добавляем заголовок в пакет
    packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&header), reinterpret_cast<uint8_t*>(&header) + sizeof(header));

    // Вопрос
    size_t start = 0;
    while (true) {
        size_t dot = domain.find('.', start);
        if (dot == std::string::npos) {
            packet.push_back(static_cast<uint8_t>(domain.size() - start));
            packet.insert(packet.end(), domain.begin() + start, domain.end());
            break;
        }
        packet.push_back(static_cast<uint8_t>(dot - start));
        packet.insert(packet.end(), domain.begin() + start, domain.begin() + dot);
        start = dot + 1;
    }
    packet.push_back(0); // Конец имени домена

    // Тип и класс запроса
    uint16_t qtype = htons(1); // Тип A
    uint16_t qclass = htons(1); // Класс IN
    packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&qtype), reinterpret_cast<uint8_t*>(&qtype) + sizeof(qtype));
    packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&qclass), reinterpret_cast<uint8_t*>(&qclass) + sizeof(qclass));

    return packet;
}

// Разбор DNS-ответа
std::string DNS_TR_C::parse_dns_response(const std::vector<uint8_t>& response) {
    // Пропускаем заголовок
    size_t pos = sizeof(DNSHeader);

    // Пропускаем вопрос
    while (pos < response.size() && response[pos] != 0) pos++;
    if (pos >= response.size()) throw std::runtime_error("Invalid DNS response: malformed question section");
    pos += 5; // Пропускаем нулевой байт и QTYPE/QCLASS

    // Разбираем ответ
    while (pos < response.size()) {
        // Пропускаем имя (может быть сжатым)
        if ((response[pos] & 0xC0) == 0xC0) {
            pos += 2; // Сжатое имя
        } else {
            while (pos < response.size() && response[pos] != 0) pos++;
            if (pos >= response.size()) throw std::runtime_error("Invalid DNS response: malformed name");
            pos++;
        }

        // Читаем тип, класс и TTL
        if (pos + 10 >= response.size()) break; // Недостаточно данных для чтения
        uint16_t type = ntohs(*reinterpret_cast<const uint16_t*>(&response[pos]));
        pos += 8; // Пропускаем TYPE, CLASS и TTL

        // Читаем длину данных
        uint16_t data_len = ntohs(*reinterpret_cast<const uint16_t*>(&response[pos]));
        pos += 2;

        // Проверяем тип записи
        if (type == 1 && data_len == 4) { // Тип A (IPv4)
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &response[pos], ip, sizeof(ip));
            return ip;
        } else if (type == 28 && data_len == 16) { // Тип AAAA (IPv6)
            char ip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &response[pos], ip, sizeof(ip));
            return ip;
        } else if (type == 5) { // Тип CNAME
            // Пропускаем CNAME и продолжаем поиск
            pos += data_len;
            continue;
        } else {
            // Пропускаем неподдерживаемые типы записей
            pos += data_len;
            continue;
        }
    }

    throw std::runtime_error("No supported DNS record type found");
}
// Отправка DNS-запроса
void DNS_TR_C::send_query() {
    if (sendto(sock, query.data(), query.size(), 0,
               reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        throw std::runtime_error("Failed to send DNS query");
    }
}

// Получение DNS-ответа
void DNS_TR_C::resive_query() {
    response.resize(512); // Буфер для ответа
    socklen_t addr_len = sizeof(server_addr);
    ssize_t len = recvfrom(sock, response.data(), response.size(), 0,
                           reinterpret_cast<sockaddr*>(&server_addr), &addr_len);
    if (len < 0) {
        throw std::runtime_error("Failed to receive DNS response");
    }
    response.resize(len);
}

// Геттер для доменного имени
std::string DNS_TR_C::getDomain() const {
    return domain;
}

// Геттер для ответа (response)
std::vector<uint8_t> DNS_TR_C::getResponse() const {
    return response;
}

// Сеттер для доменого имени 
void DNS_TR_C::setDomain(std::string domain_buf) {
    domain = domain_buf;
    query = build_dns_query(domain_buf);
}

