#pragma once

#include <string>
#include <ctime>
#include <vector>

enum class TemplateTokenType;

class FilenameTemplate
{
public:

    void set_template(const std::string& file_template);

    bool is_need_rotate(const std::tm& tm, const std::tm& current_tm) const;

    std::string generate_filename(const std::tm& tm) const;

private:

    struct TemplateToken
    {
        TemplateTokenType type;
        std::string text;

        TemplateToken(TemplateTokenType type, std::string text = "")
            : type(type)
            , text(text)
        { }
    };


    std::vector<TemplateToken> template_tokens;
    bool is_rotate;
    std::string file_template;
};
