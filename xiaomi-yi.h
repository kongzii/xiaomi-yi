/*
    Copyright [2016] <Peter Jung> 
    Released under GPL License via Github
    https://github.com/kongzii/
*/

#ifndef KONGZII_XIAOMI_YI_H_
#define KONGZII_XIAOMI_YI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include "json.hpp"

namespace kongzii {
namespace xiaomi_yi {
inline bool error(const std::string msg) {
    std::cerr << msg << std::endl;
    return false;
}

struct XYConnection {
    XYConnection(const std::string ip, const unsigned int port,
        unsigned int timeout, const bool active,
        const std::string token):
        ip(ip), port(port), timeout(timeout), active(active), token(token) {  }

    const std::string ip;
    const unsigned int port;
    const unsigned int timeout;
    const bool active;
    const std::string token;
};

class XiaomiYi {
 public:
    XiaomiYi(const std::string, const unsigned int, const unsigned int);
    XiaomiYi(const std::string kIP_, const unsigned int kPort_):
        XiaomiYi(kIP_, kPort_, 5) {}
    explicit XiaomiYi(const std::string kIP_): XiaomiYi(kIP_, 7878) {}
    XiaomiYi(): XiaomiYi("192.168.42.1") {}

    XiaomiYi(XiaomiYi &&other) = delete;
    XiaomiYi &operator=(XiaomiYi &&other) = delete;

    bool start();
    inline void stop();

    XYConnection status() const noexcept;
    std::string send(const std::string);

    inline bool shoot();
    inline bool record();

 private:
    const std::string kIP_;
    const unsigned int kPort_;
    const unsigned int kTimeOut_;

    bool active_ = false;
    bool recording_ = false;
    std::string token_;
    int sockfd_;
};

inline void XiaomiYi::stop() {
    close(sockfd_);
    std::clog << "Connection closed." << std::endl;
}

std::string XiaomiYi::send(const std::string comm) {
    struct sockaddr_in serv_addr;
    char buffer[256];

    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) { error("Error while opening socket."); }

    struct hostent *server = gethostbyname(kIP_.c_str());
    if (server == NULL) { error("Error, no such host."); }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);

    serv_addr.sin_port = htons(kPort_);
    if (connect(sockfd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        { error("Error while connecting."); }

    bzero(buffer, 256);  // Nulls the buffer
    strcpy(buffer, comm.c_str());  // Command to send to Yi

    int n = write(sockfd_, buffer, strlen(buffer));
    if (n < 0) { error("Error while writing to socket."); }

    bzero(buffer, 256);
    for (int i = 0; (active_ == false) ? (i < 2) : (i < 1); ++i) {  // Read two times for getting token
        n = read(sockfd_, buffer, 255);
        if (n < 0) { error("Error while reading from socket."); }
    }

    return std::string(buffer);  // Convert C-Style string to std::string
}

bool XiaomiYi::start() {
    auto response = send("{\"msg_id\":257, \"token\":0}");
    auto json = nlohmann::json::parse(response);

    int get_token = json["param"];
    token_ = std::to_string(get_token);
    active_ = true;
    std::clog << "Connection esthabilised with token: " << token_ << std::endl;
    return active_;
}

XYConnection XiaomiYi::status() const noexcept {
    return XYConnection(kIP_, kPort_, kTimeOut_, active_, token_);
}

inline bool XiaomiYi::shoot() {
    const std::string comm = "{\"msg_id\": 769, \"token\": " + token_ + "}";
    auto response = send(comm);
    std::clog << "Shoot." << std::endl;

    return true;
}

inline bool XiaomiYi::record() {
    std::string r = (recording_ == false) ? "513" : "514";
    const std::string comm = "{\"msg_id\": " + r + ", \"token\": " + token_ + "}";
    auto response = send(comm);

    if (recording_ == false) {
        std::clog << "Start recording." << std::endl;
        recording_ = true;
    } else {
        std::clog << "Stop recording." << std::endl;
        recording_ = false;
    }

    return true;
}

XiaomiYi::XiaomiYi(const std::string ip, const unsigned int port,
    const unsigned int timeout):
    kIP_(ip), kPort_(port), kTimeOut_(timeout) {  }

}  // namespace xiaomi_yi
}  // namespace kongzii

#endif  // KONGZII_XIAOMI_YI_H_
