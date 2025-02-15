/******************************************************************************
 * File:    server.h
 * Author:  Yihang Bi
 * Email:   byihang@umich.edu
 * 
 * Description:
 *   <Short summary of what this file is for, what it implements, etc.>
 *
 * Created: 02-06-2025
 *
 ******************************************************************************/

#include <string>
#include <iostream>
#include <map>
#pragma once

class proxyServer {
public:
    // Constructor needs port number
    proxyServer(int port);

    // Destructor
    ~proxyServer();
    
    
    void run();
private:
    int port;
    int server_socket;
    std::string parsing_http_header(int clientSocket, std::map<std::string, std::string>& headers, std::string& body);

    double calculateThroughput();

    void getManifestRequest();

    void getVidSegmentRequest();

    void postACKRecieved();   
    // static int make_server_sockaddr(struct sockaddr_in *addr, int port);
};