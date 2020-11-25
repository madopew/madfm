#include <string>
#include <filesystem>
#ifndef MADFM_FILE_H
#define MADFM_FILE_H

enum class FileType {DIR, EXE, ORD};

class File {
private:
    FileType type;
    std::string name_orig;
    std::string name_parsed;
    std::string size;
    std::string time;
public:
    File(const std::filesystem::directory_entry&);
    FileType getType(void);
    std::string getName(void);
    std::string getParsedName(void);
    std::string getSize(void);
    std::string getTime(void);
};
#endif //MADFM_FILE_H
