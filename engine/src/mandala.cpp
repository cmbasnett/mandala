//mandala
#include "mandala.hpp"
#include "python.hpp"

namespace mandala
{
    MANDALA_API_ENTRY void init()
    {
        py.exec_file("app.py");
    }
}