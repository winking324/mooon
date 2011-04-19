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
 * Author: eyjian@gmail.com, eyjian@qq.com
 *
 */
#include "kernel_service.h"
#include "message_handler.h"
MOOON_NAMESPACE_BEGIN

CMessageHandler::CMessageHandler(CKernelService* kernel_service)
    :_kernel_service(kernel_service)
{
}

void CMessageHandler::handle(schedule_message_t* schedule_message, mooon_message_t* mooon_message)
{
    if (!is_valid_mooon_message(schedule_message->type))
    {
        SCHEDULER_LOG_ERROR("Invalid message type %u.\n", schedule_message->type);
    }
    else if (_message_handler[schedule_message->type] != NULL)
    {
        (*_message_handler[schedule_message->type])(schedule_message->byte_order, mooon_message);
    }
    else
    {
        SCHEDULER_LOG_ERROR("Handler not found for message type %u.\n", schedule_message->type);
    }
}

void CMessageHandler::init_message_handler()
{
    // 先全部初始化为NULL
    for (int i=0; i<sizeof(_message_handler)/sizeof(mesage_handler_t); ++i)
        _message_handler[i] = NULL;
    
    /// Session
    _message_handler[MOOON_MESSAGE_SESSION_REQUEST] = on_session_request;
    _message_handler[MOOON_MESSAGE_SESSION_RESPONSE] = on_session_response;

    /// Service
    _message_handler[MOOON_MESSAGE_SERVICE_REQUEST] = on_service_request;
    _message_handler[MOOON_MESSAGE_SERVICE_RESPONSE] = on_service_response;
}

ISession* CMessageHandler::get_session(mooon_message_t* mooon_message)
{
    // 找到是哪个Session
    ISession* session = NULL;
    CKernelSession* kernel_session = _kernel_service->find_session(mooon_message->dest_mooon.session_id);
    if (NULL == kernel_session)
    {
        SCHEDULER_LOG_ERROR("");
    }
    else if (kernel_session->get_timestamp() != mooon_message->dest_mooon.timestamp)
    {
        // 时间戳不对
        SCHEDULER_LOG_ERROR("");
    }
    else
    {
        session = kernel_session->get_session();
    }

    return session;
}

void CMessageHandler::on_session_request(bool is_little_endian, mooon_message_t* mooon_message)
{
    ISession* session = get_session(mooon_message);
    if (session != NULL)
    {
        session->on_request(is_little_endian, mooon_message);
    }
}

void CMessageHandler::on_session_response(bool is_little_endian, mooon_message_t* mooon_message)
{
    ISession* session = get_session(mooon_message);
    if (session != NULL)
    {
        session->on_response(is_little_endian, mooon_message);
    }
}

void CMessageHandler::on_service_request(bool is_little_endian, mooon_message_t* mooon_message)
{

}

void CMessageHandler::on_service_response(bool is_little_endian, mooon_message_t* mooon_message)
{

}

MOOON_NAMESPACE_END
