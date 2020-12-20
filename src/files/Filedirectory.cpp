#include "../../headers/files/Filedirectory.h"
#include "../../headers/files/FileDirectoryUtils.h"
#include <string>
#include <filesystem>
#include <fstream>

#include <windows.h>

namespace fs = std::filesystem;

Filedirectory::Filedirectory(const std::string &path) {
    reInit(path);
    is_drives = false;
}

FiledirectoryException Filedirectory::reInit(const std::string &path) {
    std::error_code ec;
    const auto& entry = fs::directory_iterator(path, ec);
    if (ec.value() != 0)
        return FileDirectoryUtils::handleExceptionCode(ec.value());
    fillList(path);
    return FiledirectoryException::NO_EXCEPTION;
}

void Filedirectory::fillList(const std::string& path) {
    auto prev_path = fs::current_path();
    fs::current_path(path);
    auto current_path = fs::current_path();
    if (path == ".." && prev_path == current_path && !is_drives) {
        is_drives = true;
        fillListDrives();
        return;
    }
    is_drives = false;
    list.clear();
    int last_dir_index = 0;
    for (const auto& entry : fs::directory_iterator(current_path)) {
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

void Filedirectory::fillListDrives() {
    list.clear();
    DWORD drives = GetLogicalDrives();
    for (unsigned int drive = 1, letter = 'A'; drive != 0x80000000; drive <<= 1, letter++) {
        if ((drives & drive) != 0) {
            std::string drive_name = std::string(1, letter);
            drive_name.append(":\\");
            File f(drive_name);
            list.push_back(f);
        }
    }
}

std::vector<File> Filedirectory::getFilesList() {
    return list;
}

std::string Filedirectory::getCurrentDirectory() const {
    if (is_drives) {
        return "";
    }
    return fs::current_path().generic_string();
}

FiledirectoryException Filedirectory::move(const std::string &old_path, const std::string &new_path) {
    std::error_code ec;
    fs::rename(old_path, new_path, ec);
    if(ec.value() != 0)
        return FileDirectoryUtils::handleExceptionCode(ec.value());
    return FiledirectoryException::NO_EXCEPTION;
}

FiledirectoryException Filedirectory::copy(const std::string &old_path, const std::string &new_path) {
    std::error_code ec;
    fs::copy(old_path, new_path, ec);
    if(ec.value() != 0)
        return FileDirectoryUtils::handleExceptionCode(ec.value());
    return FiledirectoryException::NO_EXCEPTION;
}

FiledirectoryException Filedirectory::changeName(const std::string& old_name, const std::string& new_name) {
    auto old_p = fs::current_path() / old_name;
    auto new_p = fs::current_path() / new_name;
    return move(old_p.string(), new_p.string());
}

FiledirectoryException Filedirectory::createDir(const std::string &name) {
    std::error_code ec;
    auto path = fs::current_path() / name;
    fs::create_directory(path, ec);
    if (ec.value() != 0)
        return FileDirectoryUtils::handleExceptionCode(ec.value());
    return FiledirectoryException::NO_EXCEPTION;
}

FiledirectoryException Filedirectory::createFile(const std::string &name) {
    auto path = fs::current_path() / name;
    std::ofstream file;
    file.open(path.string());
    file.close();
    if(errno != 0)
        return FileDirectoryUtils::handleExceptionCode(errno);
    return FiledirectoryException::NO_EXCEPTION;
}

bool Filedirectory::containsCurrent(const std::string &path) {
    return fs::exists(fs::current_path() / path);
}

FiledirectoryException Filedirectory::deleteFile(const std::string &name) {
    std::error_code ec;
    auto path = fs::current_path() / name;
    fs::remove_all(path, ec);
    if (ec.value() != 0)
        return FileDirectoryUtils::handleExceptionCode(ec.value());
    return FiledirectoryException::NO_EXCEPTION;
}