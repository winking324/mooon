/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: eyjian@qq.com or eyjian@gmail.com
 */
#ifndef NET_IP_ADDRESS_H
#define NET_IP_ADDRESS_H
#include "net/net_config.h"
#include "sys/syscall_exception.h"
NET_NAMESPACE_BEGIN

/** IP地址，兼容IPV4和IPV6 */
class ip_address_t
{
public:    
    /** 构造一个127.0.0.1地址，不抛出异常 */
    ip_address_t();
    
    /** 构造一个IPV4地址，不抛出异常 */
    ip_address_t(uint32_t ipv4);

    /***
      * 构造一个IPV6地址
      * @exception: 如参数为NULL，则抛出CSyscallException异常
      */
    ip_address_t(const uint32_t* ipv6);
    
    /***
      * 构造一个IP地址，可以为IPV4，也可为IPV6
      * @ip: 字符串IP地址，如果为NULL，则构造一个0.0.0.0的IPV4地址
      * @exception: 如果为无效IP地址，则抛出CSyscallException异常
      */
    ip_address_t(const char* ip);

    /** 拷贝构造，不抛出异常 */
    ip_address_t(const ip_address_t& ip);

    /** 判断是否为IPV6地址，不抛出异常 */    
    bool is_ipv6() const;

    /** 转换成字符串，不抛出异常 */
    std::string to_string() const;

    /** 得到地址数据的有效字节数 */
    size_t get_address_data_length() const;

    /** 得到地址数据 */
    const uint32_t* get_address_data() const;    

    /** 判断是否为0.0.0.0地址 */
    bool is_zero_address() const;

    /** 判断是否为广播地址 */
    bool is_broadcast_address() const;

public: // 赋值和比较操作
    ip_address_t& operator =(uint32_t ipv4);
    ip_address_t& operator =(const uint32_t* ipv6);
    ip_address_t& operator =(const char* ip);
    ip_address_t& operator =(const ip_address_t& other);
    bool operator <(const ip_address_t& other) const;
    bool operator >(const ip_address_t& other) const;
    bool operator ==(const ip_address_t& other) const;    
    
private:
    void from_string(const char* ip);
    
private:
    bool _is_ipv6;
    uint32_t _ip_data[4];
};

inline ip_address_t::ip_address_t()
    :_is_ipv6(false)
{
    _ip_data[0] = 0;
}

inline ip_address_t::ip_address_t(uint32_t ipv4)
    :_is_ipv6(false)
{
    _ip_data[0] = ipv4;
}

inline ip_address_t::ip_address_t(const uint32_t* ipv6)
    :_is_ipv6(true)
{
    if (NULL == ipv6) throw sys::CSyscallException(EINVAL, __FILE__, __LINE__);
    memcpy(_ip_data, ipv6, sizeof(_ip_data));
}

inline ip_address_t::ip_address_t(const char* ip)
{
    from_string(ip);
}

inline ip_address_t::ip_address_t(const ip_address_t& ip)
{
    _is_ipv6 = ip.is_ipv6();
    memcpy(_ip_data, ip.get_address_data(), sizeof(_ip_data));
}

inline ip_address_t& ip_address_t::operator =(uint32_t ipv4)
{
    _is_ipv6 = false;
    _ip_data[0] =  ipv4;
    return *this;
}

inline ip_address_t& ip_address_t::operator =(const uint32_t* ipv6)
{
    if (NULL == ipv6) throw sys::CSyscallException(EINVAL, __FILE__, __LINE__);
    memcpy(_ip_data, ipv6, sizeof(_ip_data));
    return *this;
}

inline ip_address_t& ip_address_t::operator =(const char* ip)
{
    from_string(ip);
    return *this;
}

inline ip_address_t& ip_address_t::operator =(const ip_address_t& other)
{
    _is_ipv6 = other.is_ipv6();
    memcpy(_ip_data, other.get_address_data(), sizeof(_ip_data));
    return *this;
}

inline bool ip_address_t::operator <(const ip_address_t& other) const
{
    const uint32_t* ip_data = other.get_address_data();
    
    return _is_ipv6? (-1 == memcmp(_ip_data, ip_data, sizeof(_ip_data))): (_ip_data[0] < ip_data[0]);
}

inline bool ip_address_t::operator >(const ip_address_t& other) const
{
    const uint32_t* ip_data = other.get_address_data();
    return _is_ipv6? (1 == memcmp(_ip_data, ip_data, sizeof(_ip_data))): (_ip_data[0] > ip_data[0]);
}

inline bool ip_address_t::operator ==(const ip_address_t& other) const
{
    const uint32_t* ip_data = other.get_address_data();
    return _is_ipv6? (0 == memcmp(_ip_data, ip_data, sizeof(_ip_data))): (_ip_data[0] == ip_data[0]);
}

inline bool ip_address_t::is_ipv6() const
{
    return _is_ipv6;
}

NET_NAMESPACE_END
#endif // NET_IP_ADDRESS_H
