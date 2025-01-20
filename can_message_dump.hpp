/**
 * @file can_message_dump.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 20.01.2025
 */

#pragma once

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <csignal>
#include <unordered_map>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>

#include "codes/codes.hpp"

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 4096

#include "can_message_dump.hpp"
#include "magic_enum.hpp"
#include "emio.hpp"
#include "color.hpp"

const char* DEFAULT_CAN_INTERFACE = "can0";

struct __attribute__((packed)) __attribute__((__may_alias__)) Header {
    Codes::Instance     Instance_enumeration : 4 = Codes::Instance::Undefined;
    Codes::Module       Module_type : 8    = Codes::Module::Undefined;
    uint8_t             Reserved : 4       = 0x0;
    Codes::Message_type Message_type : 12  = Codes::Message_type::Undefined;
    bool                Emergency_flag : 1 = false;
};

const std::string line_format = "{:^10} {:^8} {:^16} {:^12} {:^12} {:^6} {:^12}";
