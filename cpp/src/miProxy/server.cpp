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
#include <string>
#include <iostream>
#include <map>

constexpr int BUFFER_SIZE = 1024;

//Helper fucntion that converts HTTP requests to all lowercase
std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    for (char& ch : lowerStr) {
        ch = std::tolower(static_cast<unsigned char>(ch));
    }
    return lowerStr;
}
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
    // allow reuse of the port
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


    struct sockaddr_in server_addr;
    make_server_sockaddr(&server_addr, this->port);
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    spdlog::info("miProxy started");

    // implement select() here

    // socket descriptors
    fd_set master_set, read_fds;
    FD_ZERO(&master_set);
    FD_SET(server_socket, &master_set); // add the listening socket to the master set
    int max_fd = server_socket; // track max socket descriptor

    while (true) {
        // copy the master set for select() operation
        read_fds = master_set;

        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);

        if (activity < 0 && errno != EINTR) {
            spdlog::error("Error: select() failed");
            break;
        }

        if (FD_ISSET(server_socket, &read_fds)) {
            sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int client_sock = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
            if (client_sock < 0) {
                spdlog::error("Error: accept() failed");
                continue;
            }

            // Add new client socket to the master set
            FD_SET(client_sock, &master_set);
            client_sockets[client_sock] = client_addr; // Store client info
            if (client_sock > max_fd) {
                max_fd = client_sock; // Update max file descriptor
            }

        }
        // Handle existing client requests
        for (auto it = client_sockets.begin(); it != client_sockets.end(); ) {
            int client_sock = it->first;

            if (FD_ISSET(client_sock, &read_fds)) {
                std::map<std::string, std::string> headers;
                std::string body;
                std::string requestLine = parsing_http_header(client_sock, headers, body);

                if (requestLine.empty()) {
                    spdlog::info("Client {} disconnected", client_sock);
                    close(client_sock);
                    FD_CLR(client_sock, &master_set);
                    it = client_sockets.erase(it);
                    continue;
                }

                spdlog::info("Received HTTP request: {}", requestLine);

                // TODO: Forward request to video server
                std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, Proxy!";
                send(client_sock, response.c_str(), response.size(), 0);
            }

            ++it;
        }

    }
}

std::string proxyServer::parsing_http_header(int clientSocket, std::map<std::string, std::string>& headers, std::string& body) {
    std::string buffer;
    char ch;
    
    // Read bytes until we find the end of headers (\r\n\r\n)
    while (buffer.find("\r\n\r\n") == std::string::npos) {
        ssize_t bytesReceived = recv(clientSocket, &ch, 1, 0);
        if (bytesReceived <= 0) {
           break;
        }
        buffer += ch; // allows buffer to grow dynamically
    }

    // Split headers from the body
    size_t headerEnd = buffer.find("\r\n\r\n");
    std::string headersPart = buffer.substr(0, headerEnd);
    body = buffer.substr(headerEnd + 4); // Remaining part after headers

    std::istringstream headerStream(headersPart);
    std::string line;
    bool firstLine = true;
    std::string requestLine;


    //Grab request line for later use
    if (std::getline(headerStream, line)) {
        std::getline(headerStream, requestLine);
        if (requestLine.back() == '\r') {
            requestLine.pop_back();
        }
    }

    // Parse headers line by line
    while (std::getline(headerStream, line) && !line.empty()) {
        if (line.back() == '\r') {
            line.pop_back(); // Remove trailing \r if present
        }

        size_t separator = line.find(": ");
        if (separator != std::string::npos) {
            std::string key = toLower(line.substr(0, separator));
            std::string value = line.substr(separator + 2);
            headers[key] = value;
        }
    }

    // Determine Content-Length
    int contentLength = headers.count("content-length") ? std::stoi(headers["content-length"]) : 0;

    // Read the body 
    while (body.size() < static_cast<size_t>(contentLength)) {
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead = recv(clientSocket, buffer, std::min(BUFFER_SIZE, contentLength - static_cast<int>(body.size())), 0);
        if (bytesRead <= 0) {
            break; // Connection closed or error
        }
        body.append(buffer, bytesRead);
    }

    return requestLine;
}
