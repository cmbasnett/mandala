#include <queue>

#include <boost\function.hpp>

namespace naga
{
    enum class dispatch_protocol
    {
        normal,
        discretionary
    };

    struct dispatch
    {
        void queue();

    private:
        std::queue<boost::function<void()>> _queue;
    };

    extern struct dispatch dispatch;
}