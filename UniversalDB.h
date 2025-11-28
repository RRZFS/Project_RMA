#ifndef UNIVERSAL_DB_H
#define UNIVERSAL_DB_H

#include <vector>
#include <string>

class UniversalDB {
private:
    std::vector<std::string> columnHeaders;
    std::vector<std::vector<std::string>> tableData;
    int findRowIndexById(const std::string& id);

public:
    void createTable();
    void insertRecord();
    void displayAll();
    void queryRecord();
    void updateRecord();
};

#endif // UNIVERSAL_DB_H
