#pragma once
#include <sstream>
#include <plog/Util.h>
#include <plog/Severity.h>

namespace plog
{
    class Record
    {
    public:
        Record(Severity severity, const char* func, size_t line, const void* object)
            : m_severity(severity), m_tid(util::gettid()), m_object(object), m_line(line), m_func(func)
        {
            util::ftime(&m_time);
        }

        Record& operator<<(char data)
        {
            char str[] = { data, 0 };
            *this << str;
            return *this;
        }

        Record& operator<<(const char* data)
        {
            data = data ? data : "(null)";

#ifdef _WIN32
            m_stream << util::toUnicode(data);
#else            
            m_stream << data;
#endif

            return *this;
        }

        Record& operator<<(char* data)
        {
            *this << const_cast<const char*>(data);
            return *this;
        }

        Record& operator<<(const std::string& data)
        {
            *this << data.c_str();
            return *this;
        }

#ifndef __ANDROID__
        Record& operator<<(wchar_t data)
        {
            wchar_t str[] = { data, 0 };
            *this << str;
            return *this;
        }

        Record& operator<<(const wchar_t* data)
        {
            data = data ? data : L"(null)";

#ifdef _WIN32
            m_stream << data;
#else
            *this << util::toString(data);
#endif

            return *this;
        }

        Record& operator<<(wchar_t* data)
        {
            *this << const_cast<const wchar_t*>(data);
            return *this;
        }

        Record& operator<<(const std::wstring& data)
        {
            *this << data.c_str();
            return *this;
        }
#endif

        template<typename T>
        Record& operator<<(const T& data)
        {
            m_stream << data;
            return *this;
        }

        std::string func() const
        {
            return util::processFuncName(m_func);
        }

    public:
        util::Time          m_time;
        const Severity      m_severity;
        const unsigned int  m_tid;
        const void* const   m_object;
        const size_t        m_line;
        util::nstringstream m_stream;

    private:
        const char* const   m_func;
    };
}