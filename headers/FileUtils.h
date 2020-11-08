#include <iostream>
#include <string>
#include <filesystem>
#include "File.h"

#ifndef MADFM_FILEUTILS_H
#define MADFM_FILEUTILS_H
class FileUtils{
private:
    inline static const std::string TOO_LONG_POSTFIX = " ...";
    inline static const std::string TIME_FORMAT = "%d-%m-%Y %H:%M";
    inline static const std::string UNITS[] = { "B", "kB", "MB", "GB", "TB" };
    static const int MAX_CHARS = 45;
    static const int BASE_TWO_MUL = 1024;
    static const int SIZE_LENGTH = 7;

    static bool isFileExe(const std::string&);
    static std::string defineExtension(const std::string&);
public:
    static std::string parseName(std::string);
    static std::string parseTime(std::filesystem::file_time_type);
    static std::string parseSize(uintmax_t, bool);
    static FileType defineType(const std::filesystem::directory_entry&);
};
#endif //MADFM_FILEUTILS_H
