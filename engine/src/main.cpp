//mandala
#include "python.hpp"
#include "app.hpp"
#include "game.hpp"
#include "octtree.hpp"

int main()
{
    using namespace mandala;
    using namespace boost;
    using namespace boost::python;

    octree_f32_t qt = octree_f32_t(64);
    qt.birth();

    try
    {
        app.run("TestGame");
    }
    catch (std::exception& e)
    {
        std::cout << e.what();

        system("pause");
    }
}