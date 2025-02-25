#include <chrono>

enum class Level_log {
    DEBUG, INFO, WARNING, ERROR, CRITICAL
};

class Log_item {
public:
    Log_item() = delete;

    Log_item(time_t created_at, std::string level, std::string from, std::string content) {
        m_created_at = created_at;
        m_level = level;
        m_from = from;
        m_content = content;
    }

    Log_item(const Log_item& li) = delete;

    Log_item(const Log_item&& li) {
        m_created_at = li.get_created_at();
        m_level = li.get_level();
        m_from = li.get_from();
        m_content = li.get_content();
    }

    bool operator==(const Log_item& li) {
        bool equal = m_created_at == li.get_created_at() 
                    && m_level == li.get_level() 
                    && m_from == li.get_from() 
                    && m_content == li.get_content();

        return equal;
    }

    time_t get_created_at() const {
        return m_created_at;
    }

    //получение даты в формате YYYY-mm-DD
    std::string get_date() const {
        // Создаем объект tm для работы с датой
        struct tm *tm = std::localtime(&m_created_at);
        
        char datetime_str[20];
        strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d", tm);
        
        return datetime_str;
    }

    //получение даты в формате YYYY-mm-DD hh:MM:ss
    std::string get_datetime() const {
        // Создаем объект tm для работы с датой
        struct tm *tm = std::localtime(&m_created_at);
        
        char datetime_str[20];
        strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d %H:%M:%S", tm);
        
        return datetime_str;
    }

    std::string get_level() const {
        return m_level;
    }

    std::string get_from() const {
        return m_from;
    }

    std::string get_content() const {
        return m_content;
    }

    std::string get_str_log() {
        return std::to_string(m_created_at) + "\t from:" + m_from + "\t level:" + m_level + " -->" + m_content;
    }

private:
    time_t m_created_at;
    std::string m_level;
    std::string m_from;
    std::string m_content;
};