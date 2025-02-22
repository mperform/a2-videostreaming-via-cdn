/******************************************************************************
 * File:    loadBalancer.cpp
 * Author:  Yihang Bi
 * Email:   byihang@umich.edu
 * 
 * Description:
 *   Main function for load balancing server
 *
 * Created: 02-20-2025
 *
 ******************************************************************************/
#include <iostream>
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

#include "balancer.h"

int main(int argc, char* argv[]) {
    // Comment this line to exit debug mode
    spdlog::set_level(spdlog::level::debug);

    cxxopts::Options options("loadBalancer", "A simple load balancing server");
    options.add_options()
        ("p, port", "Port of the load balancer", cxxopts::value<int>())
        ("g, geo", "Run in geo mode", cxxopts::value<bool>())
        ("r, rr", "Run in round-robin mode", cxxopts::value<bool>())
        ("s, server", "Path to file containing server info", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    auto is_geo = result["geo"].as<bool>();
    auto is_rr = result["rr"].as<bool>();
    auto port = result["port"].as<int>();
    auto server_path = result["server"].as<std::string>();

    // false: round-robin; true: geo-distance
    bool balancer_mode = false;

    if (is_geo && is_rr) {
        spdlog::error("Error: Setting both mode simultaneously.");
        exit(1);
    } else if (is_geo) {
        balancer_mode = true;
    } else if (is_rr) {
        balancer_mode = false;
    } else {
        // Should we setup a default mode or exit program?
        spdlog::debug("No mode specified.");
    }

    balancer* b_server = new balancer(balancer_mode);
    b_server->init(server_path);

    spdlog::info("Load balancer started on port {}", port);

    // Should we use infinite loop here?
    while (true) {
        clientInfo cinfo = b_server->recv_req();
        spdlog::info("Received request for client {} with request ID {}", cinfo.client_addr, cinfo.request_id);
        serverInfo sinfo = b_server->assign_server(cinfo);
        b_server->send_res(sinfo);
        spdlog::info("Responded to request ID {} with server {}:{}", cinfo.request_id, sinfo.videoserver_addr, sinfo.videoserver_port);
    }

    delete b_server;
    return 0;
}