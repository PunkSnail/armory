#include <iostream>

#include "simple_string.h"

using namespace std;

/* test + += << [] */
void test01()
{
    simple_string str1("aaa");
    simple_string str2("bbb");

    str1 += str2;

    cout << "str1 += str2:\t" << str1 << endl;
    str1 += "xxx";
    cout << "str1 += \"xxx\":\t" << str1 << endl;
    
    str1 = "aaa";
    simple_string str3 = str1 + str2;
    cout << "str3:\t" << str3 << endl;
    
    simple_string str4 = str1 + "---";
    cout << "str4:\t" << str4 << endl;
    
    str4 = str1;

    for (uint32_t i = 0; i < str4.length(); i++)
    {
        cout << str4[i];
    }
    cout << endl;
}

/* test == != */
void test02()
{
    simple_string str1 = "aaa";
    simple_string str2("bbb");
    
    if (str1 != str2)
        cout << str1 << " != " << str2 << endl;

    if (str1 != "ccc")
        cout << str1 << " != " << "ccc" << endl;

    str1 = str2;
    if (str1 == str2)
        cout << str1 << " == " << str2 << endl;
    
    if (str1 == "bbb")
        cout << str1 << " == " << "bbb" << endl;
}

int main(int argc, char *argv[])
{
    cout << "now test simple string" << endl;

    test02();
    
    return 0;
}
