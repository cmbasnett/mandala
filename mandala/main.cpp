//mandala
#include "python_mgr.hpp"

using namespace mandala;

int main()
{
    py.initialize();

    //load app.py
    py.exec_file("app.py");

    py.finalize();
}