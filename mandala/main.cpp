//mandala
#include "python_mgr.hpp"

using namespace mandala;

int main()
{
    py.initialize();

    py.exec_file("app.py");

    py.finalize();
}