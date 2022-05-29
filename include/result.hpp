#pragma once

#include "option.hpp"
#include <string>
#include <variant>

namespace csr
{
    class bad_result_access : public std::exception
    {
    };

    template <typename T, typename E>
    class Result
    {
    private:
        std::variant<T, E> res;

    private:
        Result(T &&val);
        Result(E &&err);

    public:
        Result(Result &&other);
        Result &operator=(Result &&other);
        ~Result() = default;
        Result() = delete;
        Result(const Result &_) = delete;
        Result &operator=(const Result &_) = delete;

        bool is_ok() const;
        bool is_err() const;

        const T &expect(const std::string &message) const;
        T &expect(const std::string &message);
        const T &unwrap() const;
        T &unwrap();

        const E &expect_err(const std::string &message) const;
        E &expect_err(const std::string &message);
        const E &unwrap_err() const;
        E &unwrap_err();

        T unwrap_or(T &&default_value);

        inline static Result<T, E> Ok(T &&value);
        inline static Result<T, E> Err(E &&err);

        Option<T> ok();
    };

    template <typename T, typename E>
    Result<T, E>::Result(T &&val) : res(std::move(val))
    {
    }

    template <typename T, typename E>
    Result<T, E>::Result(E &&err) : res(std::move(err))
    {
    }

    template <typename T, typename E>
    Result<T, E>::Result(Result &&other) : res(std::move(other.res))
    {
    }

    template <typename T, typename E>
    Result<T, E> &Result<T, E>::operator=(Result &&other)
    {
        res = std::move(other.res);
        return *this;
    }

    template <typename T, typename E>
    bool Result<T, E>::is_ok() const
    {
        return res.index() == 0;
    }

    template <typename T, typename E>
    bool Result<T, E>::is_err() const
    {
        return res.index() == 1;
    }

    template <typename T, typename E>
    const T &Result<T, E>::expect(const std::string &message) const
    {
        if (!is_ok())
        {
            eprintln("%s", message.c_str());
            throw bad_result_access();
        }
        return std::get<0>(res);
    }

    template <typename T, typename E>
    T &Result<T, E>::expect(const std::string &message)
    {
        if (!is_ok())
        {
            eprintln("%s", message.c_str());
            throw bad_result_access();
        }
        return std::get<0>(res);
    }

    template <typename T, typename E>
    const T &Result<T, E>::unwrap() const
    {
        if (!is_ok())
        {
            throw bad_result_access();
        }
        return std::get<0>(res);
    }

    template <typename T, typename E>
    T &Result<T, E>::unwrap()
    {
        if (!is_ok())
        {
            throw bad_result_access();
        }
        return std::get<0>(res);
    }

    template <typename T, typename E>
    const E &Result<T, E>::expect_err(const std::string &message) const
    {
        if (!is_err())
        {
            eprintln("%s", message.c_str());
            throw bad_result_access();
        }
        return std::get<1>(res);
    }

    template <typename T, typename E>
    E &Result<T, E>::expect_err(const std::string &message)
    {
        if (!is_err())
        {
            eprintln("%s", message.c_str());
            throw bad_result_access();
        }
        return std::get<1>(res);
    }

    template <typename T, typename E>
    const E &Result<T, E>::unwrap_err() const
    {
        if (!is_err())
        {
            throw bad_result_access();
        }
        return std::get<1>(res);
    }

    template <typename T, typename E>
    E &Result<T, E>::unwrap_err()
    {
        if (!is_err())
        {
            throw bad_result_access();
        }
        return std::get<1>(res);
    }

    template <typename T, typename E>
    T Result<T, E>::unwrap_or(T &&default_value)
    {
        return is_ok() ? unwrap() : default_value;
    }

    template <typename T, typename E>
    inline Result<T, E> Result<T, E>::Ok(T &&value)
    {
        return Result<T, E>(std::move(value));
    }

    template <typename T, typename E>
    inline Result<T, E> Result<T, E>::Err(E &&err)
    {
        return Result<T, E>(std::move(err));
    }

    template <typename T, typename E>
    Option<T> Result<T, E>::ok()
    {
        if (is_ok())
        {
            return Option<T>::Some(std::move(std::get<0>(res)));
        }
        else
        {
            return Option<T>::None();
        }
    }
}
