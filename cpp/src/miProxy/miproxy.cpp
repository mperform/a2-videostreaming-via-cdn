/******************************************************************************
 * File:    miproxy.cpp
 * Author:  Yihang Bi, Thomas He, John Rogan 
 * Email:   byihang@umich.edu, mperform@umich.edu, jrogan@umich.edu
 * 
 * Description:
 *   
 *
 * Created: 02-06-2025
 *
 ******************************************************************************/

#include "server.h"

#include <iostream>
#include <sys/socket.h>
#include <spdlog/spdlog.h>
#include <cxxopts.hpp>

int main(int argc, char* argv[]) {
    // Socket setup
    // -l | --listen-port: The TCP port your proxy should listen on for accepting connections from your browser.
    // -h | --hostname: Argument specifying the IP address of the video server from which the proxy should request video chunks.
    // -p | --port: Argument specifying the port of the video server at the IP address described by hostname.
    // -a | --alpha: A float in the range [0, 1]. Uses this as the coefficient in your EWMA throughput estimate.
    cxxopts::Options options("iPerfer", "A tool to measure TCP throughput");
    options.add_options()
        ("l,listen-port", "Port number", cxxopts::value<int>())
        ("h,hostname", "Server hostname", cxxopts::value<std::string>())
        ("p,port", "Port number", cxxopts::value<int>())
        ("a,alpha", "EWMA coefficient", cxxopts::value<double>())
        ;

    auto result = options.parse(argc, argv);

    if (!result.unmatched().empty()) {
        spdlog::error("Error: Unexpected argument(s) provided");
        return 1;
    }   


    // Create and start server and pass in the arguments
    int listen_port = result["listen-port"].as<int>();
    std::string hostname = result["hostname"].as<std::string>();
    int port = result["port"].as<int>();

    proxyServer server(listen_port);
    server.run();

    // // Setup finished
    // spdlog::info("miProxy started");

    // While loop, wait for requests
    while (true) {

    }
    // while (true) {
    //     // select()

    //     // 	if (socket is ready)
    //         // receive request from socket
    //         // deal with single request (e.g forwarding to webserver, forwarding back to client, updating throughput, etc.)
    //         // move on
    // }

    // Parsing HTTP 1.1 Headers
    // 1. read HTTP header 
    // have recv() in a loop, use a fixed size buffer, and when buffer is full, double its size
    // 2. parse HTTP header

    // 3. read HTTP body, using content-length size buffer

    // Calculating throughput

    // have a running average

    // calculate whenever there is a POST request to /on-fragment-received
        // will include data size, start_time, end_time
        // use this to calculate throughput
        // update the overall throughput using EWMA
    

    //



    
}