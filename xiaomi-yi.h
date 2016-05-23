#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include "json.hpp"

namespace kongzii {
    namespace xiaomi_yi {
    	inline bool error(const std::string msg) {
		    std::cerr << msg << std::endl;
		    return false;
		}

    	struct XYConnection {
    		XYConnection(const std::string ip, const unsigned int port, unsigned int timeout, const bool active, const unsigned int token):
    			ip(ip), port(port), timeout(timeout), active(active), token(token) {  }

    		const std::string ip;
    		const unsigned int port;
    		const unsigned int timeout;
    		const bool active;
    		const unsigned int token;
    	};

    	class XiaomiYi {
    	public:
    		XiaomiYi() = default;
    		XiaomiYi(const std::string, const unsigned int, const unsigned int);
    		XiaomiYi(const std::string, const unsigned int);
    		XiaomiYi(const std::string);
	
    		bool start();
    		inline void stop(); 

            XYConnection status() const noexcept;
            std::string send(const std::string);

            inline void shoot();
    	private:
    		const std::string ip = "192.168.42.1";
    		const unsigned int port = 7878;
    		const unsigned int timeout = 5;

    		bool active = false;
    		unsigned int token;
    		int sockfd;
    	};

    	inline void XiaomiYi::stop() {
    		close(sockfd);
    		std::clog << "Connection closed." << std::endl;
    	}

        std::string XiaomiYi::send(const std::string comm) {
            struct sockaddr_in serv_addr;
            char buffer[256];

            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) { error("Error while opening socket."); }

            struct hostent *server = gethostbyname(ip.c_str());
            if (server == NULL) { error("Error, no such host."); }

            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;

            bcopy((char *)server->h_addr, 
                (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);

            serv_addr.sin_port = htons(port);
            if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
                error("Error while connecting."); }

            bzero(buffer,256); // Nulls the buffer
            strcpy(buffer, comm.c_str()); // Command to send to Yi

            int n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) { error("Error while writing to socket."); }

            bzero(buffer, 256);
            for(int i = 0; i < 2; ++i) {
                n = read(sockfd, buffer, 255);
                if (n < 0) { error("Error while reading from socket."); }
            }

            return std::string(buffer); // Convert C-Style string to std::string and return
        }

    	bool XiaomiYi::start() {
		    auto response = send("{\"msg_id\":257, \"token\":0}");
			auto json = nlohmann::json::parse(response);

		    token = json["param"];
		    active = true;
		    std::clog << "Connection esthabilised with token: " << token << std::endl;
		    return active;
    	}

    	XYConnection XiaomiYi::status() const noexcept {
    		return XYConnection(ip, port, timeout, active, token);
    	}

        inline void XiaomiYi::shoot() {
            const std::string comm = "{\"msg_id\": 769, \"token\": " + std::to_string(token) + "}"; 
            auto response = send(comm);
        }

    	XiaomiYi::XiaomiYi(const std::string ip, const unsigned int port, const unsigned int timeout):
			ip(ip), port(port), timeout(timeout) {  }

		XiaomiYi::XiaomiYi(const std::string ip, const unsigned int port):
			ip(ip), port(port) {  }

		XiaomiYi::XiaomiYi(const std::string ip):
			ip(ip) {  }
    }
}