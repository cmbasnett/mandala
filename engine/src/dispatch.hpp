#include <queue>

#include <boost\function.hpp>

namespace naga
{
    enum class DispatchProtocol
    {
        NORMAL,
        DISCRETIONARY
    };

    struct Dispatch
    {
        void queue();

    private:
        std::queue<boost::function<void()>> _queue;
    };

	extern struct Dispatch dispatch;
}