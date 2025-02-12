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
    void parsing_http_header();

    double calculateThroughput();

    void getManifestRequest();

    void getVidSegmentRequest();

    void postACKRecieved();   
    // static int make_server_sockaddr(struct sockaddr_in *addr, int port);
};