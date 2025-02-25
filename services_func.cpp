std::string read_file(std::string file_path) {
    std::ifstream in(file_path);
    std::string file_content = "";

    if (in.is_open()) {
        std::string tmp_str = "";
        while (std::getline(in, tmp_str)) {
            file_content += tmp_str;
        }
    } else {
        std::cout << "File not open" << std::endl;
    }

    in.close();

    return file_content;
}

void write_file(std::string file_path , std::string content) {
    std::ofstream file(file_path, std::ios::app);
    if (file.is_open()) {
        file << content << '\n';
    }
    file.close();
}

bool compare_dates(const std::string& date1, const std::string& date2) {
    using namespace boost::gregorian;

    return from_string(date1) < from_string(date2);
}

json parse_json(const std::string& json_str) {
    auto j = json::parse(json_str);
    return j;
}