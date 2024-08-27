#include <body-building/body-building-mode/SpringMode.h>
#include <iostream>
#include <stdexcept>

using namespace std;

class Test
{
public:
    Test()
    {
        std::cout << this << std::endl;
        throw std::runtime_error{"构造函数中抛出异常"};
    }

    ~Test()
    {
        std::cout << "~Test" << std::endl;
    }
};

int main()
{
    try
    {
        Test *p = nullptr;
        try
        {
            p = new Test{};
        }
        catch (std::exception const &e)
        {
            std::cout << e.what() << std::endl;
        }

        std::cout << p << std::endl;
        return 0;
    }
    catch (std::exception const &e)
    {
        std::cout << e.what() << std::endl;
        throw;
    }
}
