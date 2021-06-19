#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <optional>

enum class TemplateTokenType;

struct TemplateFileParams
{
    int year    = 0;
    int month   = 0;
    int mday    = 0;
	int yday    = 0;
    int week    = 0;
    int hour    = 0;

    bool operator < (const TemplateFileParams &rhs) const;
};

class FilenameTemplate
{
public:

    FilenameTemplate(const std::string& file_template);

    bool rotatable() const { return is_rotatable; }

    bool is_need_rotate(const std::tm& tm, const std::tm& current_tm) const;

    std::string generate_filename(const std::tm& tm) const;

    std::optional<TemplateFileParams> parse_filename(const std::string &filename);

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

    bool is_rotatable;
    size_t name_start_token;
    size_t name_pos;
    std::vector<TemplateToken> template_tokens;
    std::string file_template;

    void update_name_start(size_t &name_start, size_t start, size_t end);
};
