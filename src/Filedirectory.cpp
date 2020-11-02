#include "../headers/Filedirectory.h"
#include <string>
#include <filesystem>
#include <utility>

namespace fs = std::filesystem;

Filedirectory::Filedirectory(std::string path) {
    reInit(std::move(path));
}

Exception Filedirectory::reInit(std::string path) {
    try {
        const auto& entry = fs::directory_iterator(path);
    } catch (const std::system_error& ex) {
        int error_code = ex.code().value();
        switch(error_code) {
            case 5:
                return Exception::ACCESS_DENIED;
            case 3:
                return Exception::FILE_NOT_FOUND;
            default:
                return Exception::UNHANDLED;
        }
    }
    fillList(path);
    return Exception::NO_EXCEPTION;
}

void Filedirectory::fillList(std::string path) {
    fs::current_path(path);
    list.clear();
    int last_dir = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        file f;
        f.type = getType(entry);
        f.name_orig = entry.path().filename().string();
        f.name_beautified = parseName(f.name_orig);
        f.size = parseSize(entry.file_size(), f.type == FileType::DIR);
        f.time = parseTime(entry.last_write_time());
        if (f.type == FileType::DIR) {
            auto it = list.begin() + last_dir;
            list.insert(it, f);
            last_dir++;
        } else {
            list.push_back(f);
        }
    }
}

FileType Filedirectory::getType(const fs::directory_entry& entry) {
    if(entry.is_directory())
        return FileType::DIR;
    if(isExe(entry.path().filename().string()))
        return FileType::EXE;
    return FileType::ORD;
}

std::string Filedirectory::parseSize(uintmax_t size, bool is_dir) {
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

std::string Filedirectory::parseTime(fs::file_time_type ftime) {
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(ftime - fs::file_time_type::clock::now()
                                                        + system_clock::now());
    time_t ttime = system_clock::to_time_t(sctp);

    char buff[17];
    tm current_time;
    localtime_s(&current_time, &ttime);
    strftime(buff, 17, "%d-%m-%Y %H:%M", &current_time);

    return buff;
}

std::string Filedirectory::parseName(std::string path) {
    if (path.length() <= MAX_CHARS)
        return path;

    std::string ext = getExt(path);
    if (ext == path) {
        path = path.substr(0, MAX_CHARS - strlen(TOO_LONG_POSTFIX));
        path.append(TOO_LONG_POSTFIX);
    } else {
        path = path.substr(0, MAX_CHARS - ext.length() - strlen(TOO_LONG_POSTFIX));
        path.append(TOO_LONG_POSTFIX + ext);
    }
    return path;
}

bool Filedirectory::isExe(const std::string& name) {
    std::string ext = getExt(name);
    return ext == "exe" || ext == "lnk";
}

std::string Filedirectory::getExt(const std::string& name) {
    return name.substr(name.find_last_of('.') + 1);
}

std::vector<file> Filedirectory::getFilesList() {
    return list;
}

std::string Filedirectory::getCurrentDirectory() {
    return fs::current_path().generic_string();
}
