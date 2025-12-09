#ifndef UNIVERSAL_DB_H
#define UNIVERSAL_DB_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 搜索结果结构体
struct SearchResult {
    int rowIndex;
    int distance;
};

// 负责对给定的数据进行计算（统计）和重排（排序）
class DataProcessor {
public:
    // 排序功能：支持数字排序和字典序排序
    // 参数：数据引用，列索引，是否升序
    static void sortData(vector<vector<string>>& data, int colIndex, bool ascending);

    // 聚合功能：计算总和、平均值、最大最小值、计数
    static void aggregateColumn(const vector<vector<string>>& data, int colIndex, const string& colName);

private:
    // 辅助：判断字符串是否为数字
    static bool isNumber(const string& s, double& val);
};

// 通用数据库功能
class UniversalDB {
private:
    vector<string> columnHeaders;
    vector<vector<string>> tableData;
    const string FILENAME = "database.txt";
    const char DELIMITER = '|';

    // 核心算法：Levenshtein 编辑距离
    int calculateLevenshtein(string s1, string s2);

    // 辅助：查找ID索引
    int findRowIndexById(const string& id);

    // 辅助：字符串分割
    vector<string> splitString(const string& s, char delimiter);

public:
    // 1. 定义表结构
    void createTable();
    // 2. 插入数据
    void insertRecord();
    // 3. 显示所有
    void displayAll();
    // 4. 模糊搜索
    void fuzzySearch();
    // 5. 保存到文件
    void saveToFile();
    // 6. 从文件加载
    void loadFromFile();
    // 7. 排序数据的接口
    void sortTable();
    // 8. 统计分析接口
    void analyzeTable();
};

#endif