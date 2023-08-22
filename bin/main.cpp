
#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t value1;
    uint2022_t value2;
    char test[]="405272312330606683982498447530407677486444946329741974138101544027695953739965";
    //char test["405272312330606683982498447530407677486444946329742187714201002871583477858493
    char test2[]="3626777458843887524118528";
    int q=10;
    value1 = from_string(test);
    value2 = from_string(test2);
    //int value2 = 10;
    //std::cout << (value1-value2) << std::endl;
    std::cout << value1*value2 << std::endl;
    return 0;
}
