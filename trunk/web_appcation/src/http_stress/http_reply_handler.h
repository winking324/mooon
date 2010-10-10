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
#ifndef HTTP_REPLY_HANDLER_H
#define HTTP_REPLY_HANDLER_H
#include "dispatcher/dispatcher.h"
MY_NAMESPACE_BEGIN

class CHttpReplyHandler: public IReplyHandler
{
public:
    CHttpReplyHandler(my::IHttpParser* http_parser);

private:  
    /** �õ��洢Ӧ����Ϣ��buffer */
    virtual char* get_buffer();

    /** �õ��洢Ӧ����Ϣ��buffer��С */
    virtual uint32_t get_buffer_length() const;    

    virtual void sender_closed(void* object, int32_t node_id, const net::ip_address_t& peer_ip, uint16_t peer_port);
    
    /** ����Ӧ����Ϣ */
    virtual bool handle_reply(void* object, int32_t node_id, const net::ip_address_t& peer_ip, uint16_t peer_port, uint32_t data_size);

private:
    void reset();
    
private:
    uint32_t _offset;
    char _buffer[IO_BUFFER_MAX];
    my::IHttpParser* _http_parser;
};

class CHttpReplyHandlerFactory: public IReplyHandlerFactory
{
public:
    CHttpReplyHandlerFactory(my::IHttpParser* http_parser);
    
private:
    virtual IReplyHandler* create_reply_handler();
    virtual void destroy_reply_handler(IReplyHandler* reply_handler);

private:
    my::IHttpParser* _http_parser;
};

MY_NAMESPACE_END
#endif // HTTP_REPLY_HANDLER_H