#include <iostream>
#include "UniversalDB.h"

using namespace std;

int main() {
    UniversalDB db;
    int choice;

    while (true) {
        cout << "\n========== 文件型通用数据库 ==========" << endl;
        cout << "1. 定义表结构 (Create)" << endl;
        cout << "2. 插入记录 (Insert)" << endl;
        cout << "3. 显示所有 (Display)" << endl;
        cout << "4. 模糊搜索 (Fuzzy Search)" << endl;
        cout << "5. 保存到文件 (Save)" << endl;
        cout << "6. 从文件读取 (Load)" << endl;
        cout << "7. 数据排序 (Sort)" << endl;
        cout << "8. 统计分析 (Stats)" << endl;
        cout << "0. 退出 (Exit)" << endl;
        cout << "======================================" << endl;
        cout << "请选择: ";

        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1: db.createTable(); break;
        case 2: db.insertRecord(); break;
        case 3: db.displayAll(); break;
        case 4: db.fuzzySearch(); break;
        case 5: db.saveToFile(); break;
        case 6: db.loadFromFile(); break;
        case 7: db.sortTable(); break;
        case 8: db.analyzeTable(); break;
        case 0: return 0;
        default: cout << "无效选项。" << endl;
        }
    }
}