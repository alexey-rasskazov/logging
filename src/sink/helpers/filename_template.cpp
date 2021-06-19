#include "filename_template.h"
#include <cstring>
#include <logging/sink/file_template_exception.h>

const char* logging::FileTemplateException::what() const noexcept
{
    return desc.c_str();
}

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
    }
    throw logging::FileTemplateException("Wrong file template format");
}

bool is_equal_weeks(const std::tm& tm1, const std::tm& tm2)
{
    char w1[3], w2[3];
    std::strftime(w1, sizeof(w1), "%W", &tm1);
    std::strftime(w2, sizeof(w2), "%W", &tm2);
    return *(unsigned short*)w1 == *(unsigned short*)w2;
}

bool TemplateFileParams::operator < (const TemplateFileParams &rhs) const
{
    if (year != rhs.year) {
        return year < rhs.year;
    }
    if (month != rhs.month) {
        return month < rhs.month;
    }
    if (yday != rhs.yday) {
        return yday < rhs.yday;
    }
    if (mday != rhs.mday) {
        return mday < rhs.mday;
    }
    if (week != rhs.week) {
        return week < rhs.week;
    }
    
    return hour < rhs.hour;
}

FilenameTemplate::FilenameTemplate(const std::string& filename_template)
    : is_rotatable{false}
    , name_start_token{0}
    , name_pos{0}
    , template_tokens{}
    , file_template{filename_template}
{
    if (filename_template.empty()) {
        throw logging::FileTemplateException("File template is empty");
    }

    std::size_t p = 0, start = 0;
    std::size_t len = file_template.length();

    auto np = file_template.find_last_of("/\\");
    if (np != std::string::npos) {
        if (np == len - 1) {
            throw logging::FileTemplateException("Trailing slash isn't allowed");
        }
        ++np;
    }

    while ((p = file_template.find_first_of('%', start)) != std::string::npos && p + 1 < len) {
        if (p > start) {
            update_name_start(np, start, p);
            template_tokens.emplace_back(TemplateTokenType::TEXT, file_template.substr(start, p - start));
        }
        p++;
        auto type = get_token_type(file_template[p]);
        update_name_start(np, start, p + 1);
        template_tokens.emplace_back(type);
        is_rotatable = true;
        p++;
        start = p;
    }

    if (p > start) {
        update_name_start(np, start, file_template.length());
        template_tokens.emplace_back(TemplateTokenType::TEXT, file_template.substr(start));
    }
}

void FilenameTemplate::update_name_start(size_t &name_start, size_t start, size_t end)
{
    if (name_start != std::string::npos) {
        if (name_start >= start && name_start < end) {
            name_start_token = template_tokens.size();
            name_pos = name_start - start;
            name_start = std::string::npos;
        }
    }
}

bool FilenameTemplate::is_need_rotate(const std::tm& tm, const std::tm& current_tm) const
{
    if (is_rotatable) {
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

static bool parse_int(const char *str, int count, int &result)
{
    int res = 0;
    for (int i = 0; i < count; ++i) {
        char c = *str++;
        if (c < '0' || c > '9') {
            return false;
        }
        res *= 10;
        res += c - '0';
    }
    result = res;
    return true;
}

std::optional<TemplateFileParams>
    FilenameTemplate::parse_filename(const std::string &filename)
{
    const char *cstr = filename.c_str();
    TemplateFileParams params;

    for (size_t i = name_start_token; i < template_tokens.size(); ++i) {
        auto &token = template_tokens[i];
        auto p = (i == name_start_token ? name_pos : 0);
        switch (token.type) {
            case TemplateTokenType::TEXT:
                if (strncmp(token.text.c_str() + p, cstr, token.text.length() - p) != 0) {
                    return std::nullopt;
                }
                cstr += token.text.length() - p;
                break;
            case TemplateTokenType::YEAR:
                if (!parse_int(cstr, 4, params.year)) {
                    return std::nullopt;
                }
                cstr += 4;
                break;
            case TemplateTokenType::MONTH:
                if (!parse_int(cstr, 2, params.month)) {
                    return std::nullopt;
                }
                cstr += 2;
                break;
            case TemplateTokenType::WEEK:
                if (!parse_int(cstr, 2, params.week)) {
                    return std::nullopt;
                }
                cstr += 2;
                break;
            case TemplateTokenType::DAY:
                if (!parse_int(cstr, 2, params.mday)) {
                    return std::nullopt;
                }
                cstr += 2;
                break;
            case TemplateTokenType::YDAY:
                if (!parse_int(cstr, 3, params.yday)) {
                    return std::nullopt;
                }
                cstr += 3;
                break;
            case TemplateTokenType::HOUR:
                if (!parse_int(cstr, 2, params.hour)) {
                    return std::nullopt;
                }
                cstr += 2;
                break;
        }
    }
    return params;
}