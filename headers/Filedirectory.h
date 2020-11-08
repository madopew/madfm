#include <string>
#include <vector>
#include <filesystem>
#include "File.h"

#ifndef MADFM_FILEDIRECTORY_H
#define MADFM_FILEDIRECTORY_H

enum class FiledirectoryException {NO_EXCEPTION, ACCESS_DENIED, FILE_NOT_FOUND, INCORRECT_NAME, UNHANDLED};

class Filedirectory {
private:
    std::vector<File> list;
    void fillList(const std::string&);
public:
    Filedirectory(const std::string &);
    FiledirectoryException reInit(const std::string&);
    std::vector<File> getFilesList(void);
    static std::string getCurrentDirectory(void);
    static FiledirectoryException changeName(const std::string&, const std::string&);
    static bool containsCurrent(const std::string&);
};
#endif //MADFM_FILEDIRECTORY_H
