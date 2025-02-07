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
    // Constructors, Creating a proxy server listening on specific ports
    proxyServer();

    // Destructor
    ~proxyServer();
    
    
    void run();
private:
    void parsing_http_header();

    double calculateThroughput();

    void getManifestRequest();

    void getVidSegmentRequest();

    void postACKRecieved();   
    
    

};