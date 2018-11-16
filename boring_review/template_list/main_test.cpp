#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "template_list.hpp"

using namespace std;

#define TEST_LIST_LEN 5

class person_t
{
public:
    person_t() {  }

    person_t(string name,int age)
    {
        this->m_name = name;
        this->m_age = age;
    }
    person_t& operator = (person_t& obj)
    {
        this->m_name = obj.m_name;
        this->m_age = obj.m_age;

        return *this;
    }
    friend void print_person(person_t& person, void *p_param);

private:
    string m_name;
    int m_age;
};

void print_person(person_t& person, void *p_param)
{
    if (NULL != p_param)
    {
        (*(int*)p_param)++;
        printf("%2d NAME %s AGE %d\n", 
               *(int*)p_param, person.m_name.c_str(), person.m_age);
    }
    else {
        printf("NAME %s AGE %d\n", person.m_name.c_str(), person.m_age);
    }
}

void test(int argc, char *argv[])
{
    char name[64] = { 0 };
    template_list<person_t> list1;

    for (int i = 0; i < TEST_LIST_LEN; i ++)
    {
        name[0] = (char)(97 + rand() % 26);
        name[1] = (char)(97 + rand() % 26);
        name[2] = (char)(97 + rand() % 26);
        name[3] = (char)0;
        
        list1.push_back(person_t(name, 15 + rand() % 85));
    }
    int index = 0;
    cout << "list1:\n";
    list1.loop_through(print_person, &index);

    cout << "- - - - - - - - - - - - - - - - - - \n";
    template_list<person_t> list2;

    for (int i = 0; i < TEST_LIST_LEN * 2; i ++)
    {
        name[0] = (char)(97 + rand() % 26);
        name[1] = (char)(97 + rand() % 26);
        name[2] = (char)(97 + rand() % 26);
        name[3] = (char)(97 + rand() % 26);
        name[4] = (char)0;
        
        list2.push_back(person_t(name, 15 + rand() % 85));
    }
    index = 0;
    cout << "list2:\n";
    list2.loop_through(print_person, &index);

    cout << "- - - - - - - - - - - - - - - - - - \n";
    list2 = list1;

    cout << "copy list1 to list2:\n";
    index = 0;
    list2.loop_through(print_person, &index);

    // template_list<person_t> list3 = list2;
}

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    test(argc, argv);

    return 0;
}
