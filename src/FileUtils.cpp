//
// Created by Madi on 05.11.2020.
//

#include "../headers/FileUtils.h"

std::string FileUtils::parseName(std::string name) {
    if (name.length() <= MAX_CHARS)
        return name;

    name = name.substr(0, MAX_CHARS - TOO_LONG_POSTFIX.length());
    name.append(TOO_LONG_POSTFIX);
    return name;
}

std::string FileUtils::parseTime(std::filesystem::file_time_type ftime) {
    using namespace std::chrono;
    using namespace std::filesystem;

    auto sctp = time_point_cast<system_clock::duration>(ftime - file_time_type::clock::now()
                                                        + system_clock::now());
    time_t ttime = system_clock::to_time_t(sctp);

    char buff[17];
    tm current_time;
    localtime_s(&current_time, &ttime);
    strftime(buff, 17, TIME_FORMAT.c_str(), &current_time);

    return buff;
}

std::string FileUtils::parseSize(uintmax_t size, bool is_dir) {
    char directory_size[SIZE_LENGTH + 1] = {0};
    if (is_dir) {
        sprintf_s(directory_size, SIZE_LENGTH + 1, "%*s", SIZE_LENGTH, "/");
        return std::string(directory_size);
    }

    int index = 0;
    while (size > BASE_TWO_MUL) {
        size /= BASE_TWO_MUL;
        index++;
    }
    std::string size_string = std::to_string(size).append(" ").append(UNITS[index]);
    sprintf_s(directory_size, SIZE_LENGTH + 1, "%*s", SIZE_LENGTH, size_string.c_str());
    return std::string(directory_size);
}

FileType FileUtils::defineType(const std::filesystem::directory_entry &entry) {
    if(entry.is_directory())
        return FileType::DIR;
    if(isFileExe(entry.path().filename().string()))
        return FileType::EXE;
    return FileType::ORD;
}

bool FileUtils::isFileExe(const std::string &name) {
    std::string ext = defineExtension(name);
    return ext == "exe" || ext == "lnk";
}

std::string FileUtils::defineExtension(const std::string &name) {
    return name.substr(name.find_last_of('.') + 1);
}

