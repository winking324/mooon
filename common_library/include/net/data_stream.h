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
#ifndef DATA_STREAM_H
#define DATA_STREAM_H
#include "net/net_util.h"
NET_NAMESPACE_BEGIN

/***
  * ���������ж�ȡ������
  */
class CStreamReader
{
public:
    /***
      * �����ͷ��ڴ棬
      * �����������detach����ʲôҲ����
      */
    ~CStreamReader()
    {
        delete []_buffer;
    }

    /***
      * ����һ����������ȡ���������������ռ乹��ʱ����
      * @size: ��Ҫ���������ռ��С
      * @reverse_bytes: �Ƿ�ת�ֽ�
      */
    CStreamReader(uint32_t size, bool reverse_bytes=false)
        :_size(size)
        ,_reverse_bytes(reverse_bytes)
    {
        _buffer = new char[_size];
    }

    /***
      * ����һ����������ȡ���������������ռ���Ҫ�����ߴ���
      * @buffer: �����������Ŀռ䣬������new char[]������
      * @size: �������ռ��С
      * @reverse_bytes: �Ƿ�ת�ֽ�
      */
    CStreamReader(char* buffer, uint32_t size, bool reverse_bytes=false)
        :_buffer(buffer)
        ,_size(size)
        ,_reverse_bytes(reverse_bytes)
    {        
    }

    /***
      * �õ�ָ���������ռ���ʼλ�õ�ָ��
      */
    char* get_buffer()
    {
        return _buffer;
    }

    /***
      * �õ��ܵ��������ռ��С
      */
    uint32_t get_size() const
    {
        return _size;
    }

    /***
      * �õ��Ѿ���ȡ�������ݴ�С
      */
    uint32_t get_offset() const
    {
        return _offset;
    }

    /***
      * ������������������ָ�����ռ���ʼλ�õĵ�ַ��
      * ���ú��������ռ���ڴ���Ҫ�����ߵ���delete[]ȥ�ͷ�
      */
    char* detach()
    {
        char* ptr = _buffer;
        _buffer = NULL;
        return ptr;
    }

    /***
      * ���������ж�ȡ���ݣ�DataTypeֻ��Ϊ�������ͣ�����Ϊָ������
      * @m: ���ڴ洢���������ж�ȡ��������
      * @return: �����ȡ��Խ���򷵻�false�����򷵻�true
      */
    template <typename DataType>
    bool read(DataType& m)
    {
        if (_offset + sizeof(m) > _size) return false;

        DataType n = *((DataType *)(_buffer + _offset));
        _offset += sizeof(m);

        if (!_reverse_bytes)
            m = n;
        else
            CNetUtil::reverse_bytes<DataType>(&n, &m);
        
        return true;
    }

    /***
      * ���������ж�ȡ����
      * @buffer: ���ڴ洢��ȡ��������
      * @size: ��Ҫ��ȡ�����ݴ�С
      * @return: �����ȡ��Խ���򷵻�false�����򷵻�true
      */
    bool read(char* buffer, uint32_t size)
    {
        if (_offset + size > _size) return false;

        memcpy(buffer, _buffer+_offset, size);
        _offset += size;

        return true;
    }
    
private:
    char* _buffer;
    uint32_t _size;
    uint32_t _offset;
    bool _reverse_bytes; /** �Ƿ�ת�ֽ� */
};

/***
  * ��������д������
  */
class CStreamWriter
{
public:
    /***
      * �����ͷ��ڴ棬
      * �����������detach����ʲôҲ����
      */
    ~CStreamWriter()
    {
        delete []_buffer;
    }

    /***
      * ����������д���󣬲��������ռ�
      * @size: ��Ҫ�������������ռ��С
      * @reverse_bytes: �Ƿ�ת�ֽ�
      */
    CStreamWriter(uint32_t size, bool reverse_bytes=false)
        :_size(size)
        ,_reverse_bytes(reverse_bytes)
    {
        _buffer = new char[_size];
    }

    /***
      * ����������д���󣬲������������ռ�
      * @buffer: �����������Ŀռ䣬������new char[]������
      * @size: �������ռ�Ĵ�С
      * @reverse_bytes: �Ƿ�ת�ֽ�
      */
    CStreamWriter(char* buffer, uint32_t size, bool reverse_bytes=false)
        :_buffer(buffer)
        ,_size(size)
        ,_reverse_bytes(reverse_bytes)
    {        
    }

    /***
      * �õ�ָ���������ռ���ʼλ�õĵ�ַ
      */
    char* get_buffer()
    {
        return _buffer;
    }

    /***
      * �õ��ܵ��������ռ��С
      */
    uint32_t get_size() const
    {
        return _size;
    }

    /***
      * �õ��Ѿ�д������ݴ�С
      */
    uint32_t get_offset() const
    {
        return _offset;
    }
    
    /***
      * ������������������ָ�����ռ���ʼλ�õĵ�ַ��
      * ���ú����ռ���ڴ���Ҫ�����ߵ���delete[]ȥ�ͷ�
      */
    char* detach()
    {
        char* ptr = _buffer;
        _buffer = NULL;
        return ptr;
    }

    /***
      * ����������д����
      * @m: ��Ҫд���������е�����
      * @return: ����������ռ䲻��д��m�򷵻�false�����򷵻�true
      */
    template <typename DataType>
    bool write(const DataType& m)
    {
        if (_offset + sizeof(m) > _size) return false;

        DataType n;
        if (!_reverse_bytes)
            n = m;
        else
            CNetUtil::reverse_bytes<DataType>(&m, &n);
        
        memcpy(_buffer, &n, sizeof(m));
        _offset += sizeof(m);

        return true;
    }

    /***
      * ����������д������
      * @buffer: ��Ҫд���������е�����
      * @size: ��Ҫд�Ĵ�С
      */
    bool write(const char* buffer, uint32_t size)
    {
        if (_offset + size > _size) return false;
        
        memcpy(buffer, _buffer+_offset, size);
        _offset += size;

        return true;
    }
    
private:
    char* _buffer;
    uint32_t _size;
    uint32_t _offset;
    bool _reverse_bytes; /** �Ƿ�ת�ֽ� */
};

NET_NAMESPACE_END
#endif // DATA_STREAM_H