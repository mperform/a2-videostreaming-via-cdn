/******************************************************************************
 * File:    balancer.h
 * Author:  Yihang Bi
 * Email:   byihang@umich.edu
 * 
 * Description:
 *   Header file for balancer class
 *
 * Created: 02-15-2025
 *
 ******************************************************************************/

#pragma once
#include <cstdint>
#include <netinet/in.h>
#include <vector>
#include <string>

#include "LoadBalancerProtocol.h"

struct serverInfo {
    in_addr_t videoserver_addr;
    uint16_t videoserver_port;
};

struct clientInfo {
    in_addr_t client_addr;
    uint16_t request_id;
};

class balancer {
public:
    // Constructor. Initialize mode and other variables.
    balancer(bool mode_in);
    // Destructor.
    ~balancer() {}

    // Initialize server list or map
    // Start listening on port
    void init(std::string file_path);

    // The function will assign the server based on the client information
    // It will determine the proper strategy itself(roundrobin/geodistance)
    serverInfo assign_server(clientInfo cinfo);

    // Receiving next request
    clientInfo recv_req();
    
    // Sending the assignment
    void send_res(serverInfo sinfo);
    
private:

    // Get the server information with round-robin strategy
    serverInfo assign_server_round_robin();

    // Get the server information with geographic distance strategy
    serverInfo assign_server_geo_dist(clientInfo cinfo);

    // Initialize map by reading the input file
    // Return 0 if succeed.
    uint8_t init_map(std::string file_path);

    // Initialize serverList by reading the input file
    // Return 0 if succeed.
    uint8_t init_list(std::string file_path);

    uint32_t roundRobinPointer;
    // false: round-robin; true: geo-distance
    bool mode;
    int sockfd;

    std::vector<serverInfo> serverList;

};