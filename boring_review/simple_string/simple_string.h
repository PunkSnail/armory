
#ifndef SIMPLE_STRING_HH
#define SIMPLE_STRING_HH

#include <iostream>
#include <stdint.h>

class simple_string
{
public:
    /* constructor and destructor */
    simple_string();
    simple_string(const char *c_str);

    simple_string(const simple_string& str_obj);

    ~simple_string();

    /* ordinary member */
    const char* c_str() const;

    uint32_t length() const;

    /* operator part */
    friend std::ostream& operator <<(std::ostream& os, simple_string& str_obj);
    friend void operator >>(const char *c_str, simple_string& str_obj);

    simple_string& operator =(const simple_string& str_obj);
    simple_string& operator =(const char *c_str);

    char& operator [](uint32_t index);

    simple_string& operator +=(simple_string& str_obj);
    simple_string& operator +=(const char *c_str);

    simple_string operator +(simple_string& str_obj);
    simple_string operator +(const char *c_str);

    bool operator ==(const char *c_str);
    bool operator ==(simple_string& str_obj);

    bool operator !=(const char *c_str);
    bool operator !=(simple_string& str_obj);

private:
    char *m_buf;
    uint32_t m_length;
};


#endif /* SIMPLE_STRING_HH */
