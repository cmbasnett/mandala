//mandala
#include "python.hpp"
#include "app.hpp"
#include "game.hpp"
#include "ntree.hpp"

int main()
{
    using namespace mandala;
    using namespace boost;
    using namespace boost::python;

    ntree<8> a;

    std::cout << reinterpret_cast<int>(&a);

    try
    {
        app.run("TestGame");
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }
}