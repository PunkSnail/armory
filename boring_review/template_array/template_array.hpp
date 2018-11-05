
#ifndef TEMPLATE_ARRAY_HHPP
#define TEMPLATE_ARRAY_HHPP

#include <stdint.h>

#define DEFAULT_CAPACITY 20

template <class T>
class template_array
{
public:
    template_array(uint32_t capacity);

    template_array(const template_array& array_obj);
    
    ~template_array();

    uint32_t length();

    template_array& operator =(const template_array& array_obj);

    T& operator[](uint32_t index);

    void push_back(T value);

    void pop_back();

    void pop_pos(uint32_t index);

private:

    T *m_array;
    uint32_t m_capacity;
    uint32_t m_length;
};
    
template <class T>
template_array<T>::template_array(uint32_t capacity)
{
    if (0 == capacity) {
        capacity = DEFAULT_CAPACITY;
    }
    this->m_capacity = capacity;

    this->m_array = new T[this->m_capacity];

    this->m_length = 0;
}

template <class T>
template_array<T>::template_array(const template_array& array_obj)
{
    this->m_capacity = array_obj.m_capacity;
    this->m_length = array_obj.m_length;

    this->m_array = new T[this->m_capacity];

    for (uint32_t i = 0; i < this->m_length; i++)
    {
        this->m_array[i] = array_obj.m_array[i];  // rely on operator =
    }
}
    
template <class T>
template_array<T>::~template_array()
{
    if (NULL != this->m_array)
    {
        delete [] this->m_array;
        this->m_array = NULL;
    }
    this->m_capacity = 0;
    this->m_length = 0;
}

template <class T>
uint32_t template_array<T>::length()
{
    return this->m_length;
}

template <class T>
template_array<T>& template_array<T>::operator =(const template_array& array_obj)
{
    this->m_capacity = array_obj.m_capacity;

    this->m_length = array_obj.m_length;
    
    if (NULL != this->m_array)
    {
        delete [] this->m_array;
        this->m_array = NULL;
    }
    this->m_array = new T[this->m_capacity];

    for (uint32_t i = 0; i < this->m_length; i++)
    {
        this->m_array[i] = array_obj.m_array[i];  // rely on operator =
    }
    return *this;
}

template <class T>
T& template_array<T>::operator[](uint32_t index)
{
    if (index > this->m_length)
    {
        index = this->m_length;
    }
    return this->m_array[index];
}

template <class T>
void template_array<T>::push_back(T value)
{
    if (this->m_length == this->m_capacity) {
        return;
    }
    this->m_array[this->m_length] = value;

    this->m_length++;
}

template <class T>
void template_array<T>::pop_back()
{
    if (this->m_length > 0)
    {
        this->m_length--;
    }
}

template <class T>
void template_array<T>::pop_pos(uint32_t index)
{
    if (index < this->m_length)
    {
        this->m_length--;
    }
    for (uint32_t i = index; i < this->m_length; i++)
    {
        this->m_array[i] = this->m_array[i + 1];
    }
}

#endif /* TEMPLATE_ARRAY_HHPP */
