#include "../../headers/files/File.h"
#include "../../headers/files/FileDirectoryUtils.h"

File::File(const std::filesystem::directory_entry &entry) {
    name_orig = entry.path().filename().string();
    name_parsed = FileDirectoryUtils::parseName(name_orig);
    type = FileDirectoryUtils::defineType(entry);
    size = FileDirectoryUtils::parseSize(entry.file_size(), type == FileType::DIR);
    time = FileDirectoryUtils::parseTime(entry.last_write_time());
}

std::string File::getName() {
    return name_orig;
}

std::string File::getParsedName() {
    return name_parsed;
}

FileType File::getType() {
    return type;
}

std::string File::getSize() {
    return size;
}

std::string File::getTime() {
    return time;
}

