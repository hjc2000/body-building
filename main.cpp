#include <body-building/body-building-mode/SpringMode.h>
#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    try
    {
        Test_SpringMode();
        return 0;
    }
    catch (std::exception const &e)
    {
        std::cout << e.what() << std::endl;
        throw;
    }
}
