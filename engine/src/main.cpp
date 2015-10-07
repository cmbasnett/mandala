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

    try
    {
        app_.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
        system("pause");
    }
    //TODO: needing to handle this type of exception is a tad annoying, search for a better way
    catch (boost::python::error_already_set&)
    {
        std::cout << mandala::python::handle_pyerr();
        system("pause");
    }
}