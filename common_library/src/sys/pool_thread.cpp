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
 * Author: jian yi, eyjian@qq.com
 */
#include "sys/pool_thread.h"
SYS_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// CPoolThread::CPoolThreadHelper

CPoolThread::CPoolThreadHelper::CPoolThreadHelper(CPoolThread* pool_thread)
    :_pool_thread(pool_thread)
	,_sync_flag(sf_init)
{
}

void CPoolThread::CPoolThreadHelper::run()
{
	// wait���ں����߳�ͬ��
	CLockHelper<CLock> lock_helper(_lock);
	if (sf_init == _sync_flag)
	{
		_sync_flag = sf_waiting;
		_event.wait(_lock);
	}
	
    while (!_stop)
    {
        _pool_thread->run();
    }
}

void CPoolThread::CPoolThreadHelper::wakeup()
{
	CLockHelper<CLock> lock_helper(_lock);		
	if (sf_waiting == _sync_flag)
		_event.signal();	

	// ����signal����wait
	_sync_flag = sf_wakeuped;
}

bool CPoolThread::CPoolThreadHelper::before_start()
{
    return _pool_thread->before_start();
}

//////////////////////////////////////////////////////////////////////////
// CPoolThread

CPoolThread::CPoolThread()
	:_index(UINT16_MAX)
{
    _pool_thread_helper = new CPoolThreadHelper(this);
}

CPoolThread::~CPoolThread()
{
    _pool_thread_helper->dec_refcount();
}

void CPoolThread::wakeup()
{
	_pool_thread_helper->wakeup();
}

bool CPoolThread::start()
{
    return _pool_thread_helper->start();
}

void CPoolThread::stop()
{
    _pool_thread_helper->stop();    
}

void CPoolThread::set_stack_size(size_t stack_size)
{
    _pool_thread_helper->set_stack_size(stack_size);
}

size_t CPoolThread::get_stack_size() const
{
    return _pool_thread_helper->get_stack_size();
}

uint32_t CPoolThread::get_thread_id() const
{
    return _pool_thread_helper->get_thread_id();
}

SYS_NAMESPACE_END