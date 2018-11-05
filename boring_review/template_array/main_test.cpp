#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "template_array.hpp"

using namespace std;

#define TEST_ARRAY_LEN 10

void print_int_array(template_array<int>& arr)
{
    for (uint32_t i = 0; i < arr.length(); i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void test_1()
{
    template_array<int> arr(TEST_ARRAY_LEN);

    for (uint32_t i = 0; i < TEST_ARRAY_LEN; i++)
    {
        arr.push_back(rand() % 100);
    }
    print_int_array(arr);
}

class person_t
{
public:
    person_t() {  }

    person_t(string name,int age)
    {
        this->m_name = name;
        this->m_age = age;
    }
public:
    string m_name;
    int m_age;
};

void print_person_array(template_array<person_t>& arr)
{
    for (uint32_t i = 0; i < arr.length(); i++)
    {
        cout << "NAME " << arr[i].m_name << "  AGE " << arr[i].m_age << endl;
    }
}

void test_2()
{
    template_array<person_t> array_1(TEST_ARRAY_LEN);

    char name[4] = { 0 };

    for (int i = 0; i < TEST_ARRAY_LEN; i ++)
    {
        name[0] = (char)(97 + rand() % 26);
        name[1] = (char)(97 + rand() % 26);
        name[2] = (char)(97 + rand() % 26);
        
        array_1.push_back(person_t(name, 15 + rand() % 85));
    }
    template_array<person_t> array_2(TEST_ARRAY_LEN);

    array_2 = array_1;

    print_person_array(array_2);

    cout << "------------------\n";

    array_2.pop_back();
    
    person_t person("xxx", 15 + rand() % 85);
    
    array_2.push_back(person);

    array_2.pop_pos(2);
    array_2.pop_pos(2);

    print_person_array(array_2);
}

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    test_2();

    return 0;
}
