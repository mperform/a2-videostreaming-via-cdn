/******************************************************************************
 * File:    utils.hpp
 * Author:  Yihang Bi
 * Email:   byihang@umich.edu
 * 
 * Description:
 *   Header file for util functions
 *
 * Created: 02-22-2025
 *
 ******************************************************************************/
#pragma once

// Sending a single variable
// The variable can be a user-defined struct
template <typename T>
int send_package(int sockfd, T* pack);