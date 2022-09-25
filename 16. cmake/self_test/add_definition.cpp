#include <iostream>

using namespace std;
int main()
{
    double tmp =0;
    #ifdef SELF_TEST
    tmp = 100;
    #endif
    cout<< tmp<<endl;
    
}