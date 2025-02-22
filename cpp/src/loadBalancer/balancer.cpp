/******************************************************************************
 * File:    balancer.h
 * Author:  Yihang Bi
 * Email:   byihang@umich.edu
 * 
 * Description:
 *   Function definitions for balancer class
 *
 * Created: 02-15-2025
 *
 ******************************************************************************/

#include "balancer.h"
#include <cassert>
#include <sys/socket.h>
#include <fstream>
#include <spdlog/spdlog.h>
#include <arpa/inet.h>

using namespace std;

balancer::balancer(bool mode_in): mode(mode_in) {
    roundRobinPointer = 0;
}

serverInfo balancer::assign_server_round_robin() {
    return serverList.at((roundRobinPointer++) % serverList.size());
}

// TODO: Implement the function
// We need an efficient data structure to store the map
serverInfo balancer::assign_server_geo_dist(clientInfo cinfo) {
    return serverInfo();
}

uint8_t balancer::init_list(std::string file_path) {
    assert(mode == false);
    ifstream ifs;
    ifs.open(file_path);

    if (!ifs.is_open()) {
        spdlog::error("ERROR: Cannot open the file.");
        exit(1);
    }

    std::string jnk;
    std::string ip_str;
    uint16_t port;
    int num_server = 0;
    ifs >> jnk >> num_server;

    for (uint32_t i=0; i<num_server; i++) {
        ifs >> ip_str >> port;
        in_addr_t ip = inet_addr(ip_str.c_str());
        if (ip == INADDR_NONE) {
            spdlog::error("ERROR: Reading invalid IP address.");
            exit(1);
        }
        serverList.push_back(serverInfo{ip, port});
    }
}

// TODO: Implement the function
uint8_t balancer::init_map(std::string file_path) {
    return 0;
}

// TODO: Implement the function
clientInfo balancer::recv_req() {

}

// TODO: Implement the function
void balancer::send_res(serverInfo sinfo) {

}

// TODO: initialize the socket and start listening on port
void balancer::init(std::string file_path) {
    if (!mode)
        init_list(file_path);
    else 
        init_map(file_path);
    return;
}

serverInfo balancer::assign_server(clientInfo cinfo) {
    if (!mode) 
        return assign_server_round_robin();
    else
        return assign_server_geo_dist(cinfo);
    
}