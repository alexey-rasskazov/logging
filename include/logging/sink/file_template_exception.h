#pragma once

#include <string>

namespace logging {

class FileTemplateException : public std::exception
{
public:
    template<typename T>
    FileTemplateException(T &&msg)
        : desc{"logging::FileTemplateException: "}
    {
        desc.append(std::forward<T>(msg));
    }

    virtual const char* what() const noexcept override;

private:
    std::string desc;
};

}
