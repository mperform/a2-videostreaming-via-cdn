/******************************************************************************
 * File:    server.cpp
 * Author:  Yihang Bi
 * Email:   byihang@umich.edu
 * 
 * Description:
 *   <Short summary of what this file is for, what it implements, etc.>
 *
 * Created: 02-06-2025
 *
 ******************************************************************************/

#include "server.h"
#include "spdlog/spdlog.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
// Proxy server needs port number 
proxyServer::proxyServer(int port) {
    this->port = port;
}
proxyServer::~proxyServer() {
    spdlog::info("Shutting down Proxy Server...");
    close(server_socket);  // Ensure socket is closed properly
}
/**
 * Helper function to create a server socket address
 * 
 * @param addr: pointer to a sockaddr_in struct
 * @param port: port number
 * @return 0 if successful
 */
int make_server_sockaddr( struct sockaddr_in *addr, int port) {
    // specify socket family
    addr->sin_family = AF_INET;
    // specify socket address(hostname)
    addr->sin_addr.s_addr = INADDR_ANY;
    // set port value
    addr->sin_port = htons(port);
    return 0;
}
/**
 * Creates a socket
 * Binds the socket to the server address
 * 
 */
void proxyServer::run() {
    // Create a socket
    this->server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket < 0) {
        spdlog::error("Error: Failed to create socket");
        return;
    }
    struct sockaddr_in server_addr;
    make_server_sockaddr(&server_addr, this->port);
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    spdlog::info("miProxy started");
}

void proxyServer::parsing_http_header() {

}
