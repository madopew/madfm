#include "../headers/Filedirectory.h"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

Filedirectory::Filedirectory(const std::string &path) {
    reInit(path);
}

FiledirectoryException Filedirectory::reInit(const std::string &path) {
    try {
        const auto& entry = fs::directory_iterator(path);
    } catch (const std::system_error& ex) {
        int error_code = ex.code().value();
        switch(error_code) {
            case 5:
                return FiledirectoryException::ACCESS_DENIED;
            case 3:
                return FiledirectoryException::FILE_NOT_FOUND;
            default:
                return FiledirectoryException::UNHANDLED;
        }
    }
    fillList(path);
    return FiledirectoryException::NO_EXCEPTION;
}

void Filedirectory::fillList(const std::string& path) {
    fs::current_path(path);
    list.clear();
    int last_dir_index = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
       File f(entry);
        if (f.getType() == FileType::DIR) {
            auto it = list.begin() + last_dir_index;
            list.insert(it, f);
            last_dir_index++;
        } else {
            list.push_back(f);
        }
    }
}

std::vector<File> Filedirectory::getFilesList() {
    return list;
}

std::string Filedirectory::getCurrentDirectory() {
    return fs::current_path().generic_string();
}

FiledirectoryException Filedirectory::changeName(const std::string& old_name, const std::string& new_name) {
    try {
        auto old_p = fs::current_path() / old_name;
        auto new_p = fs::current_path() / new_name;
        fs::rename(old_p, new_p);
    } catch (const fs::filesystem_error& e) {
        switch(e.code().value()) {
            case 5:
                return FiledirectoryException::ACCESS_DENIED;
            default:
                return FiledirectoryException::INCORRECT_NAME;
        }
    }
    return FiledirectoryException::NO_EXCEPTION;
}

bool Filedirectory::containsCurrent(const std::string &path) {
    return fs::exists(fs::current_path() / path);
}
