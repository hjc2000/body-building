#include <body-building/body-building-mode/SpringMode.h>
#include <cstddef>
#include <iostream>
#include <new>
#include <stdexcept>

using namespace std;

void operator delete(void *ptr) noexcept
{
    std::cout << "delete" << ptr << std::endl;
}

void operator delete[](void *ptr) noexcept
{
    std::cout << "delete" << ptr << std::endl;
}

void operator delete(void *ptr, std::nothrow_t const &) noexcept
{
    std::cout << "delete" << ptr << std::endl;
}

void operator delete[](void *ptr, std::nothrow_t const &) noexcept
{
    std::cout << "delete" << ptr << std::endl;
}

void operator delete(void *ptr, size_t) noexcept
{
    std::cout << "delete" << ptr << std::endl;
}

void operator delete[](void *ptr, size_t) noexcept
{
    std::cout << "delete" << ptr << std::endl;
}

class Test
{
public:
    Test()
    {
        throw std::runtime_error{"throw"};
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
