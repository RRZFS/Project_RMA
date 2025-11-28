#include "UniversalDB.h"
#include <iostream>
#include <limits>

using namespace std;

int main() {
    UniversalDB db;
    int choice;

    while (true) {
        cout << "\n========== 通用自定义数据库系统 ==========\n"
            << "1. 定义/重建表结构\n"
            << "2. 插入记录\n"
            << "3. 显示所有数据\n"
            << "4. 查询记录\n"
            << "5. 修改记录\n"
            << "0. 退出\n"
            << "==========================================\n"
            << "请选择: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1: db.createTable(); break;
        case 2: db.insertRecord(); break;
        case 3: db.displayAll(); break;
        case 4: db.queryRecord(); break;
        case 5: db.updateRecord(); break;
        case 0: return 0;
        default: cout << "无效选项。\n";
        }
    }
}