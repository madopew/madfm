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
    bool is_drives;
    void fillList(const std::string&);
    void fillListDrives();
public:
    Filedirectory(const std::string &);
    FiledirectoryException reInit(const std::string&);
    std::vector<File> getFilesList(void);
    std::string getCurrentDirectory(void) const;
    bool containsCurrent(const std::string&);
    static FiledirectoryException move(const std::string&, const std::string&);
    static FiledirectoryException copy(const std::string&, const std::string&);
    static FiledirectoryException changeName(const std::string&, const std::string&);
    static FiledirectoryException createDir(const std::string&);
    static FiledirectoryException createFile(const std::string&);
    static FiledirectoryException deleteFile(const std::string&);
};
#endif //MADFM_FILEDIRECTORY_H
