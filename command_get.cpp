Command_item creat_get_command(const json& result_parse) {
    std::string command = result_parse["command"].get<std::string>();
    Command_item command_item(command);

    //если не будут указаны условия для фильтрации, то возвращаются логи за текущий день
    if (!result_parse.contains("from") 
        && !result_parse.contains("created_from")
        && !result_parse.contains("created_to")
        && !result_parse.contains("level")
        && !result_parse.contains("content")) 
    {
        // std::cout << __LINE__ << ' ' << __PRETTY_FUNCTION__ << std::endl;
        command_item = {command, time(nullptr)};
    }

    // if (result_parse.contains("from")) {
    //     std::string from = result_parse["from"].get<std::string>();
    //     command_item = {command, from};
    // }

    if (result_parse.contains("created_from")) {
        // std::cout << __LINE__ << ' ' << __PRETTY_FUNCTION__ << std::endl;
        time_t created_from = result_parse["created_from"].get<time_t>();
        command_item = {command, created_from}; 
    }

    // if (result_parse.contains("created_to")) {
    //     time_t created_to = result_parse["created_to"].get<std::time_t>();
    //     command_item = {command, created_to}; 
    // }

    // if (result_parse.contains("content")) {
    //     std::string content = result_parse["content"].get<std::string>();
    //     command_item = {command, content}; 
    // }

    //дата ПО должна быть больше или равна дате С
    //если нет, то учитывается только дата С
    if (result_parse.contains("created_from") && result_parse.contains("created_to")) {
        if (compare_dates(result_parse["create_to"], result_parse["create_from"])) {
            std::string crated_from = result_parse["created_from"].get<std::string>();
            command_item = {command, crated_from}; 
        }
    }

    // std::string level = result_parse["level"].get<std::string>();
    return command;
}

void find_log(Command_item& get_command) {
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

        for(const auto& entry: std::filesystem::directory_iterator(dir_path)) {
            if (entry.is_regular_file()) {
                std::string file_path = entry.path().string();
                std::string file_name = entry.path().filename().string();
                
                // std::cout << __LINE__ << std::endl;

                std::string date_from = get_command.get_created_from();
                std::string date_to = get_command.get_created_to();
                // std::cout << "date_from: " << date_from << "  date_to: " << date_to << std::endl;

                //поведение зависит от наличия дат С и ПО
                // if (!date_from.empty() && !date_to.empty()) {
                //     std::cout << __LINE__ << std::endl;
                //     if (!compare_dates(file_name, date_from) && compare_dates(date_to, file_name)) {
                //         std::cout << __LINE__ << std::endl;
                //         std::cout << "Нашлись логи в данном периоде" << std::endl;
                //     }
                // } else if (!date_from.empty() && date_to.empty()) {
                //     std::cout << __LINE__ << std::endl;
                //     if (!compare_dates(file_name, date_from)) {
                //         std::cout << "Нашлись логи от указанной даты" << std::endl;
                //     }
                // } else if (date_from.empty() && !date_to.empty()) {
                //     std::cout << __LINE__ << std::endl;
                //     if (compare_dates(date_to, file_name)) {
                //         std::cout << "Нашлись логи до указанной даты" << std::endl;
                //     }
                // } else {
                //     std::cout << __LINE__ << std::endl;
                // }

                // if (file_name == log_item.get_date()) {
                //     write_file(file_path, log_item.get_str_log());
                //     is_saved = true;
                //     break;
                // }
            } else if (entry.is_directory()) {
                std::string dir_path = entry.path().string();
                std::cout << dir_path << std::endl; 
            }
        }

    } catch (const ErrorType& err) {
        std::cout << "err: " << std::get<0>(err) << ' ' << std::get<1>(err) << std::endl;
    }
}
