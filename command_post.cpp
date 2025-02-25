void save_log(Log_item&& log_item) {
    std::string path = "./logs";
    std::filesystem::path dir_path = path;

    using ErrorType = std::tuple<int, std::string>;

    try {
        if(!std::filesystem::exists(dir_path)) {
            throw ErrorType(-1, "path not exists");
        }

        if(!std::filesystem::is_directory(dir_path)) {
            throw ErrorType(-1, "not directory");
        }

        bool is_saved = false;

        for(const auto& entry: std::filesystem::directory_iterator(dir_path)) {
            if (entry.is_regular_file()) {
                std::string file_path = entry.path().string();
                std::string file_name = entry.path().filename().string();

                if (file_name == log_item.get_date()) {
                    write_file(file_path, log_item.get_str_log());
                    is_saved = true;
                    break;
                }
            } else if (entry.is_directory()) {
                std::string dir_path = entry.path().string();
                std::cout << dir_path << std::endl; 
            }
        }

        if (!is_saved) {
            std::string new_file_path = path + '/' + log_item.get_date();
            write_file(new_file_path, log_item.get_str_log()); 
        }

    } catch (const ErrorType& err) {
        std::cout << "err: " << std::get<0>(err) << ' ' << std::get<1>(err) << std::endl;
    }
}

Log_item creat_log_item(const json& result_parse) {
    time_t created_at = result_parse["created_at"].get<time_t>();
    std::string level = result_parse["level"].get<std::string>();
    std::string from = result_parse["from"].get<std::string>();
    std::string content = result_parse["content"].get<std::string>();

    Log_item log_item(created_at, level, from, content);
    return log_item;
}

bool check_post_request(json result_parse) {
    return result_parse.contains("from") 
            && result_parse.contains("created_at")
            && result_parse.contains("level")
            && result_parse.contains("content");
}