#include "parser.h"
#include <iostream>
#include <sstream>
#include<iomanip>
#include<mainwindow.h>
string getItemCoordinates(QTableWidgetItem* item) {
    string result;
    char letter = item->column()+ 65;
    char digit = item->row() + 48;
    result+=letter;
    result+=digit;
    return result;
}
double Parser::RecursiveRef(MainWindow* MainWindow, QTableWidgetItem* item, QTableWidget* table,int& number_of_iterations){
    if(item->text() == " "){
        return CODE_NUMBER_FOR_BAD_EXPRESSION;
    }


    //e.g A2,B4
    string thisCellCoordinate = getItemCoordinates(item);

    number_of_iterations++;
    QString line = MainWindow->GetExpr(item->row(), item->column());
    string line_str = line.toStdString();
    string result;

    for(int i = 0; i < line_str.length(); i++)
    {
        if((int)line_str[i] >= 65 && (int)line_str[i] <=71)
        {

            int column_of_ref = (int)line_str[i]-65;

            string row;
            row +=line_str[i+1];
            if(row == "0") return CODE_NUMBER_FOR_BAD_EXPRESSION;
            int row_of_ref =0;
            row_of_ref= stoi(row) -1;


            //e.g A2, B4
            string refCellCoordinates = line_str[i]+row;

            if(entranceTable[hash<string>{}(thisCellCoordinate)%100][hash<string>{}(refCellCoordinates)%100] == true) {
                return CODE_NUMBER_FOR_CYCLE;
            }


            entranceTable[hash<string>{}(thisCellCoordinate)%100][hash<string>{}(refCellCoordinates)%100] = true;

            if(row_of_ref >= table->rowCount()){
                return CODE_NUMBER_FOR_BAD_EXPRESSION;
            }

            QTableWidgetItem* ref_item = table->item(row_of_ref, column_of_ref);


            double value_of_ref = RecursiveRef(MainWindow, ref_item, table, number_of_iterations);

            if(value_of_ref == CODE_NUMBER_FOR_CYCLE){
                return CODE_NUMBER_FOR_CYCLE;
            }


            if(value_of_ref == CODE_NUMBER_FOR_BAD_EXPRESSION){
                return CODE_NUMBER_FOR_BAD_EXPRESSION;
            }
            result+=to_string(value_of_ref);
            i++;
        }
        else
        {
            result+= line_str[i];
        }
    }

    double calculatedExpression = calculateExpression(result);
    return calculatedExpression;
}


bool Parser::isInteger(const std::string & s)
{
    if(s[0] == 'q') return true;


    auto result = double();
   auto i = std::istringstream(s);

   i >> result;

   return !i.fail() && i.eof();
}

bool Parser::isDigit(char a) {
    string tmp = "0123456789";
    for(auto it: tmp) if(it == a) return true;
    return false;
}
bool Parser::isOperation(const string& s) {
    return s == "*" || s == "/" || s == "-" || s == "+" || s == "^" || s == "mod" || s == "div" || s == "max" || s == "min";
}
bool Parser::isOperation(char s) {
    return s == '*' || s == '/' || s == '+' || s == '^' || s == '-';
}
std::vector<std::string> Parser::splitString(const std::string &str) {
    std::string word;
    std::vector<std::string> result;
    for (auto x : str) {
        if (x == ' ') {
            result.push_back(word);
            result.push_back(" ");
            word = "";
        }
        else word = word + x;
    }
    result.push_back(word);
    return result;
}

vector<string> Parser::parseExpression(const string& s) {
    vector<string> result;
   // result.push_back("(");
    bool to_reverse_next_number = 0;
    string resNumber;
    for(int i = 0; i < s.size(); ++i) {
        if(s[i] == '-' && (i == 0 || s[i-1] == '(' || s[i-1] == '+'||s[i-1] == '/'||s[i-1] == '-'||s[i-1] == '*')){
                to_reverse_next_number = 1;
        }
        else if(s[i] == ' ' || s[i] == '(' || s[i] == ')' || isOperation(s[i])) {
            if(!resNumber.empty()) {
                result.push_back(resNumber);
                resNumber = "";
            }
            if(s[i] == ' ') continue;
            string tmp(1, s[i]);
            result.push_back(tmp);

        }
        else if(s[i] == 'm' || s[i] == 'd') {
            string tmp(s.begin()+i, s.begin()+i+3);
            result.push_back(tmp);
            i+=2;
        }
        else {
            if(to_reverse_next_number ==1){
                resNumber+= 'q';
                to_reverse_next_number = 0;
            }
            resNumber+=s[i];
        }
        if(resNumber.back() == ',') {
            resNumber.pop_back();
            resNumber.push_back('.');
        }
    }
    if(!resNumber.empty()) result.push_back(resNumber);

    //result.push_back(")");
    return result;
}

map<string, int> priorities = {{"+", 1},{"-", 1},{"*", 2},{"/", 2},{"^", 3} ,{"mod", 2}, {"div", 2}};
double Parser::calculateExpression(const string& inputExpression) {


    vector<string> tokens = parseExpression(inputExpression);

    tokens.emplace_back("X");
    vector<double> numbersStack;
    vector<string> operationsStack;
    map<std::string, std::function<double(const double&, const double&)>> operations;
    operations["+"] = [](double a, double b) {return a+b;};
    operations["-"] = [](double a, double b) {return a-b;};
    operations["*"] = [](double a, double b) {return a*b;};
    operations["/"] = [](double a, double b) {
        if(b == 0){ return CODE_NUMBER_FOR_BAD_EXPRESSION;}
        return a/b;
    };
    operations["^"] = [](double a, double b) {return pow(a,b);};
    operations["mod"] = [](double a, double b) {
        if (b == 0){return (int)CODE_NUMBER_FOR_BAD_EXPRESSION;}
        return (int)a%(int)b;
    };

    operations["div"] = [](double a, double b) {
        if(b == 0) {return (int)CODE_NUMBER_FOR_BAD_EXPRESSION;}
        return (int)a/(int)b;
    };
    operations["max"] = [](double a, double b) {return max(a,b);};
    operations["min"] = [](double a, double b) {return min(a,b);};

    for(auto it: tokens)
    {
        if(isInteger(it))
        {
            string reverted;
            if(it[0] == 'q')
            {
                for(int i = 1; i < it.length(); i++)
                {
                    reverted+=it[i];
                }
                double num = (atof(reverted.c_str())*(-1));
                numbersStack.push_back(num);

            }
            else
            {
                numbersStack.push_back(atof(it.c_str()));
            }
        }

        else
        {
            if(isOperation(it))
            {
                while(!operationsStack.empty() && priorities[operationsStack.back()] >= priorities[it])
                {
                    std::string OpBack = operationsStack.back();
                    int OpBackPrior = priorities[OpBack];
                    int itPrior = priorities[it];
                    if(operationsStack.back() == "(" || operationsStack.back() == ")") break;
                    double secondOperand = numbersStack.back();
                    numbersStack.pop_back();
                    double firstOperand;
                    firstOperand = numbersStack.back();

                    numbersStack.pop_back();
                    numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));
                    operationsStack.pop_back();
                }
                operationsStack.push_back(it);
            }
            else if(it == "(") operationsStack.push_back(it);

            else if(it == ")")
            {
                if(numbersStack.empty()) {return CODE_NUMBER_FOR_BAD_EXPRESSION;}
                while(operationsStack.back() != "(")
                {
                    if(numbersStack.size() < 2) {return CODE_NUMBER_FOR_BAD_EXPRESSION;}
                    double secondOperand = numbersStack.back();
                    numbersStack.pop_back();
                    double firstOperand = numbersStack.back();
                    numbersStack.pop_back();
                    numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));
                    operationsStack.pop_back();
                }
                operationsStack.pop_back();
            }
            else if(it == "X")
            {
                if(numbersStack.size() - operationsStack.size() == 1)
                {
                    while(!operationsStack.empty())
                    {
                        double secondOperand = numbersStack.back();
                        numbersStack.pop_back();
                        double firstOperand = numbersStack.back();
                        numbersStack.pop_back();
                        try
                        {
                        numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));
                        }
                        catch(const std::exception& ex)
                        {
                            std::cerr<<ex.what();
                        }

                        operationsStack.pop_back();
                    }
                    if(numbersStack.empty()) {return CODE_NUMBER_FOR_BAD_EXPRESSION;}
                    return numbersStack.back();
                }
                else
                {
                    return CODE_NUMBER_FOR_BAD_EXPRESSION;
                }
            }
        }
    }
    return numbersStack.back();
}

void Parser:: clearEntranceTable() {
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++) {
            entranceTable[i][j] = false;
        }
    }
}
