
#ifndef TEMPLATE_LIST_HHPP
#define TEMPLATE_LIST_HHPP

#include <stdint.h>
#include <stdio.h>


template <class T>
struct link_node
{
    T val;
    link_node<T> *next;
};


template <class T>
class template_list
{
public:
    template_list();
    template_list(const template_list<T>& obj);
    ~template_list();

    void push_by_pos(uint32_t pos, T val);
    void push_front(T val);
    void push_back(T val);

    void pop_by_pos(uint32_t pos);
    void pop_front();
    void pop_back();

    uint32_t size();

    /* through_cb has two parameters: T val, void *cb_param */
    template <class through_cb>
    void loop_through(through_cb cb, void *cb_param);

    template_list<T>& operator =(template_list<T>& obj);

private:

    link_node<T> m_header;
    uint32_t m_size;
};

template <class T>
template_list<T>::template_list()
{
    this->m_header.next = NULL;
    this->m_size = 0;
}

template <class T>
template_list<T>::template_list(const template_list<T>& obj)
{
    this->m_header.next = NULL;
    this->m_size = obj.m_size;

    link_node<T> *p_dest = &this->m_header;
    link_node<T> *p_src = obj.m_header.next;
    
    for (uint32_t i = 0; i < this->m_size; i++)
    {
        p_dest->next = new link_node<T>;

        p_dest->next->val = p_src->val;
        p_dest->next->next = NULL;
        
        p_dest = p_dest->next;

        p_src = p_src->next;
    }
}

template <class T>
template_list<T>::~template_list() 
{
    link_node<T> *p_node = this->m_header.next;

    link_node<T> *p_del = NULL;

    for (uint32_t i = 0; i < this->m_size; i++)
    {
        p_del = p_node;
        p_node = p_del->next;
        
        delete p_del;
    }
}

template <class T>
void template_list<T>::push_by_pos(uint32_t pos, T val) 
{
    if (pos > this->m_size) {
        return;
    }
    link_node<T> *p_new = new link_node<T>;

    p_new->val = val;
    p_new->next = NULL;

    link_node<T> *p_node = &(this->m_header);

    for (uint32_t i = 0; i < pos; i++) {
        p_node = p_node->next;  // rely on operator =
    }
    p_new->next = p_node->next;
    p_node->next = p_new;

    this->m_size++;
}

template <class T>
void template_list<T>::push_front(T val)
{
    push_by_pos(0, val);
}

template <class T>
void template_list<T>::push_back(T val)
{
    push_by_pos(this->m_size, val);
}

template <class T>
void template_list<T>::pop_by_pos(uint32_t pos)
{
    if (0 == this->m_size || pos > this->m_size) {
        return;
    }
    link_node<T> *p_node = &(this->m_header);

    for (uint32_t i = 0; i < pos; i++) {
        p_node = p_node->next;
    }
    link_node<T> *p_del = p_node->next;
    p_node->next = p_del->next;
    
    delete p_del;
    this->m_size--;
}

template <class T>
void template_list<T>::pop_front()
{
    pop_by_pos(0);
}

template <class T>
void template_list<T>::pop_back()
{
    pop_by_pos(this->m_size);
}

template <class T>
uint32_t template_list<T>::size()
{
    return this->m_size;
}

template <class T>
template <class through_cb>
void template_list<T>::loop_through(through_cb cb, void *cb_param)
{
    link_node<T> *p_node = this->m_header.next;

    while (NULL != p_node)
    {
        cb(p_node->val, cb_param);
        p_node = p_node->next;
    }
}

template <class T>
template_list<T>& template_list<T>::operator =(template_list<T>& obj)
{
    /* release surplus memory */
    if (this->m_size > obj.m_size) 
    {
        uint32_t surplus_count =  this->m_size - obj.m_size;

        link_node<T> *p_del = NULL;
        link_node<T> *p_node = this->m_header.next;

        for (uint32_t i = 0; i < surplus_count; i++)
        {
            p_del = p_node;
            p_node = p_node->next;
            
            delete p_del;
        }
        this->m_header.next = p_node;
    }
    this->m_size = obj.m_size;

    link_node<T> *p_dest = &this->m_header;
    link_node<T> *p_src = obj.m_header.next;
    
    for (uint32_t i = 0; i < this->m_size; i++)
    {
        if (NULL == p_dest->next)
        {
            p_dest->next = new link_node<T>;
            p_dest->next->next = NULL;
        }
        p_dest->next->val = p_src->val;
        
        p_dest = p_dest->next;

        p_src = p_src->next;
    }
    return *this;
}

#endif /* TEMPLATE_LIST_HHPP */
