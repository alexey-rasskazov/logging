#include "filename_template.h"

enum class TemplateTokenType
{
    TEXT,
    YEAR,
    MONTH,
    WEEK,
    DAY,
    YDAY,
    HOUR,
};

const char* get_token_type_format(TemplateTokenType type)
{
    switch (type) {
        case TemplateTokenType::YEAR:   return "%Y";
        case TemplateTokenType::MONTH:  return "%m";
        case TemplateTokenType::DAY:    return "%d";
        case TemplateTokenType::YDAY:   return "%j";
        case TemplateTokenType::WEEK:   return "%W";
        case TemplateTokenType::HOUR:   return "%H";
    }
    return "";
}

TemplateTokenType get_token_type(char fmt_char)
{
    switch (fmt_char) {
        case 'Y': return TemplateTokenType::YEAR;
        case 'm': return TemplateTokenType::MONTH;
        case 'd': return TemplateTokenType::DAY;
        case 'j': return TemplateTokenType::YDAY;
        case 'W': return TemplateTokenType::WEEK;
        case 'H': return TemplateTokenType::HOUR;
        case '%': return TemplateTokenType::TEXT;
    }
    throw std::exception();
}

bool is_equal_weeks(const std::tm& tm1, const std::tm& tm2)
{
    char w1[3], w2[3];
    std::strftime(w1, sizeof(w1), "%W", &tm1);
    std::strftime(w2, sizeof(w2), "%W", &tm2);
    return *(unsigned short*)w1 == *(unsigned short*)w2;
}

void FilenameTemplate::set_template(const std::string& file_template)
{
    template_tokens.clear();
    std::size_t p = 0, start = 0;
    std::size_t len = file_template.length();
    is_rotate = false;

    while ((p = file_template.find_first_of('%', start)) != std::string::npos && p + 1 < len) {
        if (p > start) {
            template_tokens.emplace_back(TemplateTokenType::TEXT, file_template.substr(start, p - start));
        }
        p++;
        auto type = get_token_type(file_template[p]);
        if (type == TemplateTokenType::TEXT) {
            start = p;
            continue;
        }
        template_tokens.emplace_back(type);
        is_rotate = true;
        p++;
        start = p;
    }

    if (p > start) {
        template_tokens.emplace_back(TemplateTokenType::TEXT, file_template.substr(start));
    }

    this->file_template = file_template;
}

bool FilenameTemplate::is_need_rotate(const std::tm& tm, const std::tm& current_tm) const
{
    if (is_rotate) {
        for (auto& token : template_tokens) {
            switch (token.type) {
                case TemplateTokenType::DAY:
                case TemplateTokenType::YDAY:
                    if (current_tm.tm_yday != tm.tm_yday || 
                        current_tm.tm_year != tm.tm_year)
                        return true;
                break;
                case TemplateTokenType::MONTH:
                    if (current_tm.tm_mon != tm.tm_mon ||
                        current_tm.tm_year != tm.tm_year)
                        return true;
                break;
                case TemplateTokenType::YEAR:
                    if (current_tm.tm_year != tm.tm_year)
                        return true;
                break;
                case TemplateTokenType::HOUR:
                    if (current_tm.tm_hour != tm.tm_hour || 
                        current_tm.tm_yday != tm.tm_yday || 
                        current_tm.tm_year != tm.tm_year)
                        return true;
                break;
                case TemplateTokenType::WEEK:
                    if (!is_equal_weeks(current_tm, tm))
                        return true;
                break;
            }
        }
    }

    return false;
}

std::string FilenameTemplate::generate_filename(const std::tm& tm) const
{
    std::string result;
    char tmp[8];
    for (auto& token : template_tokens) {
        const char* fmt = get_token_type_format(token.type);
        if (fmt[0]) {
            strftime(tmp, sizeof(tmp), fmt, &tm);
            result += tmp;
        } else {
            result += token.text;
        }
    }
    return result;
}