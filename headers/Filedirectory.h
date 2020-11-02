#ifndef MADFM_FILEDIRECTORY_H
#define MADFM_FILEDIRECTORY_H

#include <string>
#include <vector>
#include <filesystem>

const char TOO_LONG_POSTFIX[] = " ...";
const int MAX_CHARS = 30;
const int BASE_TWO_MUL = 1024;
const int SIZE_LENGTH = 7;

enum class Exception {NO_EXCEPTION, ACCESS_DENIED, FILE_NOT_FOUND, UNHANDLED};
enum class FileType {DIR, EXE, ORD};

struct file {
    std::string name_beautified;
    std::string name_orig;
    FileType type;
    std::string size;
    std::string time;
};

class Filedirectory {
private:
    const char* UNITS[5] = { "B", "kB", "MB", "GB", "TB" };
    std::vector<file> list;
    static std::string parseName(std::string);
    static bool isExe(const std::string&);
    static std::string parseTime(std::filesystem::file_time_type);
    std::string parseSize(uintmax_t, bool);
    FileType getType(const std::filesystem::directory_entry&);
    void fillList(std::string);
    static std::string getExt(const std::string&);
public:
    Filedirectory(std::string);
    Exception reInit(std::string);
    std::vector<file> getFilesList(void);
    static std::string getCurrentDirectory(void);
};
#endif //MADFM_FILEDIRECTORY_H
