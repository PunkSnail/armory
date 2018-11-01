
#include <string.h>
#include "simple_string.h"

using namespace std;

simple_string::simple_string()
{
    this->m_length = 0;

    this->m_buf = new char[1]; // for operator []
    this->m_buf[0] = '\0';
}

simple_string::simple_string(const char *c_str)
{
    this->m_length = (uint32_t)strlen(c_str);

    this->m_buf = new char[this->m_length + 1]();

    strncpy(this->m_buf, c_str, this->m_length);
}

simple_string::simple_string(const simple_string& str_obj)
{
    this->m_length = str_obj.m_length;

    this->m_buf = new char[this->m_length + 1]();
    
    strncpy(this->m_buf, str_obj.m_buf, this->m_length);
}

simple_string::~simple_string()
{
    if (this->m_buf)
    {
        delete [] this->m_buf;
        this->m_buf = NULL;
    }
    this->m_length  = 0;
}

const char* simple_string::c_str() const
{
    return this->m_buf;
}

uint32_t simple_string::length() const
{
    return this->m_length;
}

ostream& operator <<(ostream& os, simple_string& str_obj)
{
    os << str_obj.m_buf;
    return os;
}

void operator >>(const char *c_str, simple_string& str_obj)
{
    if (str_obj.m_buf)
    {
        delete [] str_obj.m_buf;
        str_obj.m_buf = NULL;
    }
    str_obj.m_length = strlen(c_str);

    str_obj.m_buf = new char[str_obj.m_length + 1]();

    strncpy(str_obj.m_buf, c_str, str_obj.m_length);
}

simple_string& simple_string::operator =(const simple_string& str_obj)
{
    this->m_length = str_obj.m_length;

    if (this->m_buf)
    {
        delete [] this->m_buf;
        this->m_buf = NULL;
    }
    this->m_buf = new char[this->m_length + 1]();
    
    strncpy(this->m_buf, str_obj.m_buf, this->m_length);
    
    return *this;
}

simple_string& simple_string::operator =(const char *c_str)
{
    this->m_length = (uint32_t)strlen(c_str);

    if (this->m_buf)
    {
        delete [] this->m_buf;
        this->m_buf = NULL;
    }
    this->m_buf = new char[this->m_length + 1]();
    
    strncpy(this->m_buf, c_str, this->m_length);

    return *this;
}

char& simple_string::operator [](uint32_t index)
{
    if (index > this->m_length) {
        return this->m_buf[this->m_length];
    }
    return this->m_buf[index];
}

simple_string& simple_string::operator +=(simple_string& str_obj)
{
    if (0 == str_obj.m_length) {
        return *this;
    }
    char *tmp_buf = new char [this->m_length + str_obj.m_length + 1]();

    if (this->m_buf)
    {
        strncpy(tmp_buf, this->m_buf, this->m_length);
        delete [] this->m_buf;
        this->m_buf = NULL;
    }
    this->m_buf = tmp_buf;
    
    strncpy(this->m_buf + this->m_length, str_obj.m_buf, str_obj.m_length);
    
    this->m_length += str_obj.m_length;
    
    return *this;
}

simple_string& simple_string::operator +=(const char *c_str)
{
    if (NULL == c_str) {
        return *this;
    }
    uint32_t c_str_len = strlen(c_str);

    char *tmp_buf = new char [this->m_length + c_str_len + 1]();

    if (this->m_buf)
    {
        strncpy(tmp_buf, this->m_buf, this->m_length);
        delete [] this->m_buf;
        this->m_buf = NULL;
    }
    this->m_buf = tmp_buf;
    
    strncpy(this->m_buf + this->m_length, c_str, c_str_len);
    
    this->m_length += c_str_len;
    
    return *this;
}

simple_string simple_string::operator +(simple_string& str_obj)
{
    if (0 == str_obj.m_length) {
        return *this;
    }
    simple_string result;

    if (result.m_buf)
    {
        delete [] result.m_buf;
        result.m_buf = NULL;
    }
    result.m_length = this->m_length + str_obj.m_length;
    result.m_buf = new char [result.m_length + 1]();

    strncpy(result.m_buf, this->m_buf, this->m_length);
    strncpy(result.m_buf + this->m_length, str_obj.m_buf, str_obj.m_length);

    return result;
}

simple_string simple_string::operator +(const char *c_str)
{
    if (NULL == c_str) {
        return *this;
    }
    uint32_t c_str_len = (uint32_t)strlen(c_str);
    
    simple_string result;

    if (result.m_buf)
    {
        delete [] result.m_buf;
        result.m_buf = NULL;
    }
    result.m_length = this->m_length + c_str_len;
    result.m_buf = new char [result.m_length + 1]();

    strncpy(result.m_buf, this->m_buf, this->m_length);
    strncpy(result.m_buf + this->m_length, c_str, c_str_len);

    return result;
}

bool simple_string::operator ==(const char *c_str)
{
    bool result = false;
    
    uint32_t c_str_len = (uint32_t)strlen(c_str);

    if (c_str_len == this->m_length 
        && strncmp(this->m_buf, c_str, this->m_length) == 0)
    {
        result = true;
    }
    return result;
}

bool simple_string::operator ==(simple_string& str_obj)
{
    bool result = false;
    
    if (str_obj.m_length == this->m_length 
        && strncmp(this->m_buf, str_obj.m_buf, this->m_length) == 0)
    {
        result = true;
    }
    return result;
}

bool simple_string::operator !=(const char *c_str)
{
    bool result = false;

    uint32_t c_str_len = (uint32_t)strlen(c_str);

    if (c_str_len != this->m_length 
        || strncmp(this->m_buf, c_str, this->m_length) != 0)
    {
        result = true;
    }
    return result;
}

bool simple_string::operator !=(simple_string& str_obj)
{
    bool result = false;
    
    if (str_obj.m_length != this->m_length 
        || strncmp(this->m_buf, str_obj.m_buf, this->m_length) != 0)
    {
        result = true;
    }
    return result;
}

