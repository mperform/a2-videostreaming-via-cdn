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
    struct sockaddr_in server_addr;
    make_server_sockaddr(&server_addr, this->port);
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    spdlog::info("miProxy started");
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
