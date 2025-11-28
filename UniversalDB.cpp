#include "UniversalDB.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

int UniversalDB::findRowIndexById(const string& id) {
    for (size_t i = 0; i < tableData.size(); ++i) {
        if (tableData[i][0] == id) return i;
    }
    return -1;
}

void UniversalDB::createTable() {
    if (!tableData.empty()) {
        char choice;
        std::cout << "警告：当前已有数据，创建新表将清空旧数据。继续吗？(y/n): ";
        std::cin >> choice;
        if (choice != 'y' && choice != 'Y') return;
    }

    columnHeaders.clear();
    tableData.clear();

    int colCount;
    std::cout << "=== 创建新表 ===" << endl;
    std::cout << "请输入你要管理的字段数量 (例如 3): ";
    while (!(cin >> colCount) || colCount < 1) {
        std::cout << "输入无效，至少需要 1 列。请重新输入: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    cout << "注意：系统默认 **第1个字段** 为唯一检索ID (主键)。" << endl;
    for (int i = 0; i < colCount; ++i) {
        std::string colName;
        std::cout << "请输入第 " << (i + 1) << " 个字段的名称 (例如: 学号/名称/价格): ";
        std::cin >> colName;
        columnHeaders.push_back(colName);
    }
    std::cout << ">> 表结构创建成功！" << endl;
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void UniversalDB::insertRecord() {
    if (columnHeaders.empty()) {
        std::cout << ">> 错误：表结构不存在，请先选择功能 '1' 创建表。" << endl;
        return;
    }

    std::vector<std::string> newRow;
    std::string inputVal;

    std::cout << "=== 插入数据 ===" << endl;

    // 处理第一列 (主键 ID) 的查重
    std::cout << "请输入 " << columnHeaders[0] << " (主键/唯一): ";
    std::cin >> inputVal;
    if (findRowIndexById(inputVal) != -1) {
        std::cout << ">> 错误：该 " << columnHeaders[0] << " (" << inputVal << ") 已存在！" << endl;
        return;
    }
    newRow.push_back(inputVal);

    // 处理剩余列
    for (size_t i = 1; i < columnHeaders.size(); ++i) {
        std::cout << "请输入 " << columnHeaders[i] << ": ";
        std::cin >> inputVal;
        newRow.push_back(inputVal);
    }

    tableData.push_back(newRow);
    std::cout << ">> 记录插入成功。" << endl;
}

void UniversalDB::displayAll() {
    if (columnHeaders.empty()) {
        std::cout << ">> 表是空的。" << endl;
        return;
    }

    std::cout << "\n------------------------------------------------------" << endl;
    // 打印表头
    for (const auto& header : columnHeaders) {
        std::cout << left << setw(15) << header;
    }
    std::cout << endl;
    std::cout << "------------------------------------------------------" << endl;

    // 打印数据
    for (const auto& row : tableData) {
        for (const auto& val : row) {
            std::cout << left << setw(15) << val;
        }
        std::cout << endl;
    }
    std::cout << "------------------------------------------------------" << endl;
    std::cout << "总记录数: " << tableData.size() << endl;
}

void UniversalDB::queryRecord() {
    if (columnHeaders.empty()) return;

    std::string id;
    std::cout << "请输入要查询的 " << columnHeaders[0] << ": ";
    std::cin >> id;

    int index = findRowIndexById(id);
    if (index != -1) {
        std::cout << "\n>> 查询结果:" << endl;
        for (size_t i = 0; i < columnHeaders.size(); ++i) {
            std::cout << columnHeaders[i] << ": " << tableData[index][i] << endl;
        }
    }
    else {
        std::cout << ">> 未找到该记录。" << endl;
    }
}

void UniversalDB::updateRecord() {
    if (columnHeaders.empty()) return;

    std::string id;
    std::cout << "请输入要修改记录的 " << columnHeaders[0] << ": ";
    std::cin >> id;

    int index = findRowIndexById(id);
    if (index != -1) {
        std::cout << ">> 找到记录。请更新除主键外的信息 (主键无法修改):" << endl;
        for (size_t i = 1; i < columnHeaders.size(); ++i) {
            std::string newVal;
            std::cout << "原 " << columnHeaders[i] << " (" << tableData[index][i] << ") -> 新值: ";
            std::cin >> newVal;
            tableData[index][i] = newVal;
        }
        std::cout << ">> 修改成功。" << endl;
    }
    else {
        std::cout << ">> 未找到该记录，无法修改。" << endl;
    }
}