#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    try
    {
        return 0;
    }
    catch (std::exception const &e)
    {
        std::cout << e.what() << std::endl;
        throw;
    }
}
