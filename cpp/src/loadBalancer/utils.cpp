#include "utils.hpp"

#include <spdlog/spdlog.h>
#include <sys/socket.h>

template <typename T>
int send_package(int sockfd, T* pack) {
    size_t total_bytes_sent = 0;
    while (total_bytes_sent < sizeof(T)) {
        ssize_t bytes_sent = send(sockfd, 
            pack + total_bytes_sent, 
            sizeof(T) - total_bytes_sent, 0);
        
        if (bytes_sent < 0) {
            spdlog::error("Send error: {}", strerror(errno));
            break;
        }

        total_bytes_sent += bytes_sent;
    }
}