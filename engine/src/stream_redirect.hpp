#pragma once

//std
#include <iostream>
#include <functional>

namespace naga
{
    struct stream_redirect
    {
        stream_redirect(std::ostream& stream, std::streambuf* rdbuf) :
            stream(stream),
            rdbuf_old(stream.rdbuf())
        {
            stream.set_rdbuf(rdbuf);
        }

        ~stream_redirect()
        {
            stream.set_rdbuf(rdbuf_old);
        }

    private:
        stream_redirect(const stream_redirect&) = delete;
        stream_redirect& operator = (const stream_redirect&) = delete;

        std::ostream& stream;
        std::streambuf* rdbuf_old;
    };

    //http://stackoverflow.com/questions/12826751/c-execute-function-any-time-a-stream-is-written-to
    struct functionbuf : public std::streambuf
    {
        typedef std::function<void(std::string)> function_type;

        functionbuf(const function_type& function)
            : function(function)
        {
            setp(buffer, buffer + sizeof(buffer) - 1);
        }

    private:
        char buffer[1024];
        function_type function;

        virtual int_type overflow(int_type c) override
        {
            if (!traits_type::eq_int_type(c, traits_type::eof()))
            {
                *this->pptr() = traits_type::to_char_type(c);
                pbump(1);
            }

            return sync() ? traits_type::not_eof(c) : traits_type::eof();
        }

        virtual int_type sync() override
        {
            if (pbase() != pptr())
            {
                function(std::string(pbase(), pptr()));

                setp(pbase(), epptr());
            }

            return 0;
        }
    };

    struct ofunctionstream :
        private virtual functionbuf,
        public std::ostream
    {
        ofunctionstream(const function_type& function) :
            functionbuf(function),
            std::ostream(static_cast<std::streambuf*>(this))
        {
            setf(std::ios_base::unitbuf);
        }
    };
}