#include "UniversalDB.h"
#include <iomanip>
#include <algorithm> // sort, min, transform, max_element
#include <fstream>   // 文件读写
#include <sstream>   // 用于字符串分割
#include <cmath>     // 数学计算
#include <cfloat>    // DBL_MAX

// DataProcessor 类的实现

bool DataProcessor::isNumber(const string& s, double& val) {
    try {
        size_t idx;
        val = stod(s, &idx);
        return idx == s.length(); // 确保整个字符串都是数字
    }
    catch (...) {
        return false;
    }
}

void DataProcessor::sortData(vector<vector<string>>& data, int colIndex, bool ascending) {
    // 使用 std::sort 和自定义 Lambda 表达式
    sort(data.begin(), data.end(), [colIndex, ascending](const vector<string>& rowA, const vector<string>& rowB) {
        string valA = rowA[colIndex];
        string valB = rowB[colIndex];
        double numA, numB;

        // 尝试作为数字比较
        bool isNumA = isNumber(valA, numA);
        bool isNumB = isNumber(valB, numB);

        bool result;
        if (isNumA && isNumB) {
            result = numA < numB; // 数字比较
        }
        else {
            result = valA < valB; // 字符串字典序比较
        }

        return ascending ? result : !result; // 根据升序/降序反转结果
        });
}

void DataProcessor::aggregateColumn(const vector<vector<string>>& data, int colIndex, const string& colName) {
    double sum = 0.0;
    double maxVal = -DBL_MAX;
    double minVal = DBL_MAX;
    int count = 0;
    int numericCount = 0;

    cout << "\n--- 正在分析列: [" << colName << "] ---" << endl;

    for (const auto& row : data) {
        if (row.size() <= colIndex) continue;
        count++;

        double val;
        if (isNumber(row[colIndex], val)) {
            sum += val;
            if (val > maxVal) maxVal = val;
            if (val < minVal) minVal = val;
            numericCount++;
        }
    }

    cout << "总行数 (Count): " << count << endl;

    if (numericCount > 0) {
        cout << "有效数字个数:   " << numericCount << endl;
        cout << "总和 (Sum):     " << sum << endl;
        cout << "平均值 (Avg):   " << sum / numericCount << endl;
        cout << "最大值 (Max):   " << maxVal << endl;
        cout << "最小值 (Min):   " << minVal << endl;
    }
    else {
        cout << ">> 该列不包含有效数字，无法计算 Sum/Avg/Max/Min。" << endl;
    }
    cout << "-----------------------------------" << endl;
}

// UniversalDB 类的实现

int UniversalDB::calculateLevenshtein(string s1, string s2) {
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    int m = s1.length(), n = s2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost });
        }
    }
    return dp[m][n];
}

int UniversalDB::findRowIndexById(const string& id) {
    for (size_t i = 0; i < tableData.size(); ++i) {
        if (!tableData[i].empty() && tableData[i][0] == id) return i;
    }
    return -1;
}

vector<string> UniversalDB::splitString(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void UniversalDB::createTable() {
    if (!tableData.empty()) {
        cout << "现有数据将被清除，确认？(y/n): ";
        char c; cin >> c; if (c != 'y' && c != 'Y') return;
    }
    columnHeaders.clear();
    tableData.clear();

    int colCount;
    cout << "请输入字段数量: ";
    while (!(cin >> colCount) || colCount < 1) { cin.clear(); cin.ignore(10000, '\n'); }

    cout << "注意：第1列默认为主键。" << endl;
    for (int i = 0; i < colCount; ++i) {
        string name;
        cout << "字段 " << i + 1 << " 名称: ";
        cin >> name;
        columnHeaders.push_back(name);
    }
    cout << ">> 表结构创建成功。" << endl;
}

void UniversalDB::insertRecord() {
    if (columnHeaders.empty()) { cout << "请先建表或加载文件。" << endl; return; }
    vector<string> row;
    string val;

    cout << "输入 " << columnHeaders[0] << " (主键): ";
    cin >> val;
    if (val.find(DELIMITER) != string::npos) { cout << "非法字符 '|'。" << endl; return; }
    if (findRowIndexById(val) != -1) { cout << "主键重复！" << endl; return; }
    row.push_back(val);

    for (size_t i = 1; i < columnHeaders.size(); ++i) {
        cout << "输入 " << columnHeaders[i] << ": ";
        cin >> val;
        if (val.find(DELIMITER) != string::npos) { cout << "非法字符 '|'。" << endl; return; }
        row.push_back(val);
    }
    tableData.push_back(row);
    cout << ">> 插入成功。" << endl;
}

void UniversalDB::displayAll() {
    if (columnHeaders.empty()) { cout << "表是空的。" << endl; return; }
    cout << "\n------------------------------------------------" << endl;
    for (const auto& h : columnHeaders) cout << left << setw(15) << h;
    cout << "\n------------------------------------------------" << endl;
    for (const auto& row : tableData) {
        for (const auto& v : row) cout << left << setw(15) << v;
        cout << endl;
    }
    cout << "------------------------------------------------" << endl;
    cout << "总记录: " << tableData.size() << endl;
}

void UniversalDB::fuzzySearch() {
    if (tableData.empty()) { cout << "无数据可搜。" << endl; return; }
    cout << "选择列索引 (0-" << columnHeaders.size() - 1 << "): ";
    int idx; cin >> idx;
    if (idx < 0 || idx >= columnHeaders.size()) return;

    string key;
    cout << "输入模糊关键词: ";
    cin >> key;
    int maxDist = max(2, (int)key.length() / 2);

    vector<SearchResult> results;
    for (size_t i = 0; i < tableData.size(); ++i) {
        int d = calculateLevenshtein(tableData[i][idx], key);
        if (d <= maxDist) results.push_back({ (int)i, d });
    }
    sort(results.begin(), results.end(), [](auto& a, auto& b) { return a.distance < b.distance; });

    cout << "\n>> 找到 " << results.size() << " 条匹配:" << endl;
    for (auto& res : results) {
        for (auto& v : tableData[res.rowIndex]) cout << v << " ";
        cout << "(差异度: " << res.distance << ")" << endl;
    }
}

void UniversalDB::saveToFile() {
    if (columnHeaders.empty()) {
        cout << ">> 没有数据需要保存。" << endl;
        return;
    }

    ofstream outFile(FILENAME);
    if (!outFile.is_open()) {
        cout << ">> 错误：无法打开文件进行写入。" << endl;
        return;
    }

    for (size_t i = 0; i < columnHeaders.size(); ++i) {
        outFile << columnHeaders[i];
        if (i < columnHeaders.size() - 1) outFile << DELIMITER;
    }
    outFile << endl;

    for (const auto& row : tableData) {
        for (size_t i = 0; i < row.size(); ++i) {
            outFile << row[i];
            if (i < row.size() - 1) outFile << DELIMITER;
        }
        outFile << endl;
    }

    outFile.close();
    cout << ">> 成功保存到 " << FILENAME << endl;
}

void UniversalDB::loadFromFile() {
    ifstream inFile(FILENAME);
    if (!inFile.is_open()) {
        cout << ">> 错误：找不到文件 " << FILENAME << "，请先保存数据。" << endl;
        return;
    }

    columnHeaders.clear();
    tableData.clear();

    string line;
    if (getline(inFile, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        columnHeaders = splitString(line, DELIMITER);
    }

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        vector<string> row = splitString(line, DELIMITER);
        if (row.size() == columnHeaders.size()) {
            tableData.push_back(row);
        }
    }

    inFile.close();
    cout << ">> 成功加载数据。包含 " << columnHeaders.size() << " 列，" << tableData.size() << " 行。" << endl;
}

void UniversalDB::sortTable() {
    if (tableData.empty()) { cout << "表是空的。" << endl; return; }

    cout << "选择排序的列索引 (0-" << columnHeaders.size() - 1 << "): ";
    int idx;
    if (!(cin >> idx) || idx < 0 || idx >= columnHeaders.size()) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "无效索引。" << endl; return;
    }

    cout << "排序方式 (0: 升序, 1: 降序): ";
    int order; cin >> order;
    bool ascending = (order == 0);

    // 调用 DataProcessor 类的静态方法，将私有数据传入
    DataProcessor::sortData(tableData, idx, ascending);
    cout << ">> 排序完成。" << endl;
}

void UniversalDB::analyzeTable() {
    if (tableData.empty()) { cout << "表是空的。" << endl; return; }

    cout << "选择需要统计的列索引 (0-" << columnHeaders.size() - 1 << "): ";
    int idx;
    if (!(cin >> idx) || idx < 0 || idx >= columnHeaders.size()) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "无效索引。" << endl; return;
    }

    // 调用 DataProcessor 类的静态方法
    DataProcessor::aggregateColumn(tableData, idx, columnHeaders[idx]);
}