/******************************************************************************
 * The MIT Licence
 *
 * Copyright (c) 2021 Airbus Operations S.A.S
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/

#ifndef _ED247_INTERNALS_H_
#define _ED247_INTERNALS_H_

// ED247_FRIEND_TEST macro will be defined by tests only when building tests
#ifndef ED247_FRIEND_TEST
#define ED247_FRIEND_TEST(...)
#endif

/************
 * Includes *
 ************/

#include "ed247.h"
#include "ed247_logs.h"

#include <string>
#include <memory>
#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <iterator>
#include <vector>

#include <assert.h>
#include <chrono>

#ifdef __linux__
    #include <byteswap.h>
#elif __QNXNTO__
    #include <gulliver.h>
    #define bswap_16(x) ENDIAN_RET16(x)
    #define bswap_32(x) ENDIAN_RET32(x)
    #define bswap_64(x) ENDIAN_RET64(x)
#endif

#ifdef __unix__
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <netdb.h>
    #include <ifaddrs.h>
    #ifndef INVALID_SOCKET
        #define INVALID_SOCKET (-1)
    #endif
    #ifndef ED247_SOCKET
        #define ED247_SOCKET int
    #endif
#elif _WIN32
    #include <winsock2.h>
    #include <Ws2tcpip.h>
    #include <mswsock.h>
    #include <windows.h>
    #ifndef ED247_SOCKET
        #define ED247_SOCKET SOCKET
    #endif
    #ifndef _MSC_VER
        #define bswap_16(x) __builtin_bswap16(x)
        #define bswap_32(x) __builtin_bswap32(x)
        #define bswap_64(x) __builtin_bswap64(x)
    #else
        #define bswap_16(x) _byteswap_ushort(x)
        #define bswap_32(x) _byteswap_ulong(x)
        #define bswap_64(x) _byteswap_uint64(x)
    #endif
#endif

#ifndef _MSC_VER
    #include <unistd.h>
#endif

#ifndef NDEBUG
    #define DEBUG_ONLY if(true)
#else
    #define DEBUG_ONLY if(false)
#endif

#ifndef _UNUSED
#define _UNUSED(x) (void)x
#endif


#include "cpp_14.h"


/***********
 * Defines *
 ***********/
template<typename T>
struct weak_hash : public std::unary_function<std::weak_ptr<T>, size_t>
{
    size_t operator()(const std::weak_ptr<T> & wp) const
    {
        auto sp = wp.lock();
        return std::hash<decltype(sp)>()(sp);
    }
};

template<typename T>
struct weak_equal_to : public std::unary_function<std::weak_ptr<T>, bool>
{
    bool operator()(const std::weak_ptr<T> & wx, const std::weak_ptr<T> & wy) const
    {
        return wx.lock() == wy.lock();
    }
};

/*********
 * Types *
 *********/

struct ed247_internal_context_t {};

struct ed247_internal_channel_t {};


#endif
