#include "../headers/Filedirectory.h"
#include "../headers/FileDirectoryUtils.h"
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

Filedirectory::Filedirectory(const std::string &path) {
    reInit(path);
}

FiledirectoryException Filedirectory::reInit(const std::string &path) {
    try {
        const auto& entry = fs::directory_iterator(path);
    } catch (const std::system_error& e) {
        return FileDirectoryUtils::handleExceptionCode(e.code().value());
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
    auto old_p = fs::current_path() / old_name;
    auto new_p = fs::current_path() / new_name;
    try {
        fs::rename(old_p, new_p);
    } catch (const fs::filesystem_error& e) {
        return FileDirectoryUtils::handleExceptionCode(e.code().value());
    }
    return FiledirectoryException::NO_EXCEPTION;
}

FiledirectoryException Filedirectory::createDir(const std::string &name) {
    auto path = fs::current_path() / name;
    try {
        fs::create_directory(path);
    } catch(const fs::filesystem_error& e) {
        //printf(" %d ", e.code().value());
        return FileDirectoryUtils::handleExceptionCode(e.code().value());
    }
    return FiledirectoryException::NO_EXCEPTION;
}

FiledirectoryException Filedirectory::createFile(const std::string &name) {
    auto path = fs::current_path() / name;
    std::ofstream file;
    std::ios_base::iostate exceptionMask = file.exceptions() | std::ios::failbit;
    file.exceptions(exceptionMask);
    try {
        file.open(path.string());
        file.close();
    } catch (std::ios_base::failure& e) {
        //printf(" %s %d ", strerror(errno), errno);
        return FileDirectoryUtils::handleExceptionCode(errno);
    }
    return FiledirectoryException::NO_EXCEPTION;
}

bool Filedirectory::containsCurrent(const std::string &path) {
    return fs::exists(fs::current_path() / path);
}
