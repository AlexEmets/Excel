#ifndef TESTPARSER_PARSER_H
#define TESTPARSER_PARSER_H


#include<stack>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
#include<functional>
#include<string>
#include <QTableWidget>
#include <QMessageBox>
#include "mainwindow.h"

#define CODE_NUMBER_FOR_BAD_EXPRESSION -2147483648.0
#define CODE_NUMBER_FOR_CYCLE -2147483647.0


using namespace std;
class Parser {
public:
    double RecursiveRef(MainWindow* MainWindow, QTableWidgetItem* item, QTableWidget* table,int& number_of_iterations);
    bool isInteger(const std::string & s);
    bool isDigit(char a);
    bool isOperation(const string& s);
    bool isOperation(char s);
    std::vector<std::string> splitString(const std::string &str);
    vector<string> parseExpression(const string& s);
    vector<string> findRef(const string& s);
    double calculateExpression(const string& inputExpression);
    void clearEntranceTable();
private:
    map<string, int> priorities = {{"+", 1},{"-", 1},{"*", 2},{"/", 2},{"^", 3} ,{"mod", 2}, {"div", 2}};
    bool entranceTable[100][100];
};

#endif //TESTPARSER_PARSER_H
