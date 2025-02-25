class Command_item {
public:
    // Command_item(std::string command, size_t size = 0) {
    //     m_command = command;

    //     if (size != 0) {
    //         m_size = size;
    //     }
    // }

    // Command_item(std::string command, std::string from = "", time_t created_from = 0, time_t created_to = 0, std::vector<std::string> level = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"}, std::string content = "") {
    //     m_command = command;
    //     m_from = from;
    //     m_created_from = created_from;
    //     m_created_to = created_to == 0 ? std::time(nullptr) : created_to;
    //     m_level = level;
    //     m_content = content;
    // }

    //будет использоваться для создания команды,
    //если не будут указаны условия для фильтрации
    Command_item(std::string command, time_t created_from) {
        m_command = command;
        m_created_from = created_from;
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    } 

    Command_item(std::string command, std::string from = "", time_t created_from = 0, time_t created_to = 0, std::string content = "") {
        m_command = command;
        m_from = from;
        m_created_from = created_from;
        m_created_to = created_to;
        m_content = content;
    }

    std::string get_command() const {
        return m_command;
    }

    std::string get_created_from() {
        return get_date_string(m_created_from); 
    } 

    std::string get_created_to() {
        return get_date_string(m_created_to); 
    } 
private:
    std::string m_command;
    std::string m_from;
    size_t m_size;
    time_t m_created_from;
    time_t m_created_to;
    std::vector<std::string> m_level;
    std::string m_content;

private:
    std::string get_date_string(time_t date) {
        if (date != 0) {

            using namespace boost::posix_time;
            using namespace boost::gregorian;

            // Конвертация time_t в ptime
            ptime pt = from_time_t(date);

            // Извлечение даты из ptime
            auto d = pt.date();

            std::cout << "to_iso_extended_string: " << to_iso_extended_string(d) << std::endl; 

            return to_iso_extended_string(d);

            // return datetime_str;
        } else {
            return "";
        }        
    }
};