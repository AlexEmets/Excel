#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <parser.h>
#include <iostream>
#include <fstream>
#include <vector>
#include  <random>
#include  <iterator>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem(" ");
                ui->tableWidget->setItem(i, j, itm);
            }
    }
    ChangeExpressions();

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D");

    QTableWidgetItem* cur = ui->tableWidget->item(0,0);
    ui->tableWidget->setCurrentItem(cur);
    //std::cout << "Constructor done";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_helper_triggered()
{
    QMessageBox::about(this, "Helper", "Привіт(:\nУ таблиці ви можете використовувати такі операції:\n1)+,-.*,/\n2)mod,div\n3)min max\n"
                                       "Також ви можете використовувати силки на інші клітинки вказавши їх координати(В4)");
}


void MainWindow::on_actionQuit_triggered()
{
    this->close();
}


void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if(is_double_clicked){
        UpdateText(row, column);
        //is_double_clicked = 0;
    }
    //ChangeExpressions();
}
void MainWindow::UpdateText(int row, int column){
    if(row == changed_by_hands.first && column == changed_by_hands.second){
       // std::cout << "UpdateText! " << changed_by_hands.first << " " << changed_by_hands.second << "\n";
        expressions[row][column] = ui->tableWidget->item(row, column)->text();
        changed_by_hands.first = -1;
        changed_by_hands.second = -1;
    }
}
void MainWindow::ChangeExpressions(){
    std::cout <<"Change Expression!\n";
    int i = ui->tableWidget->rowCount();
    int j = ui->tableWidget->columnCount();
   // std::cout << "i = " <<i << " j = " << j << "\n";
    expressions.resize(i, vector<QString>(j));
    for(int a = 0; a < i; a++){
        for(int b = 0; b < j; b++){
            expressions[a][b] = ui->tableWidget->item(a, b)->text();
        }
    }
}
void MainWindow::on_CalculateButton_clicked()
{
    changed_by_hands.first = -1;
    changed_by_hands.second = -1;

    Parser p;
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    int n = 0;


    p.clearEntranceTable();
    double res = p.RecursiveRef(this, item, ui->tableWidget, n);
    if(res == CODE_NUMBER_FOR_CYCLE) {
        QMessageBox::critical(this, "Помилка!", "В виразі цикл:(\nЗмініть вираз");
    }
    else if(res == CODE_NUMBER_FOR_BAD_EXPRESSION)
    {
        QMessageBox::critical(this, "Помилка!", "Поганий вираз:(");
    }
    else
    {
        QString answer;
        if(res > 1e7) {
            answer = "~inf";
        }
        else answer = QString::number(res);
        item->setText(answer);
    }
}
void MainWindow::CleanTable(){
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem(" ");
                ui->tableWidget->setItem(i, j, itm);
            }
    }
    ChangeExpressions();
}
void MainWindow::on_SetAboba_clicked()
{
    CleanTable();

}

void MainWindow::on_SaveFile_clicked()
{

    string file_path = "/home/oyemets/Documents/MyExcelFinal/";
    file_path += ui->file_name->text().toStdString();
    ofstream out;
    out.open(file_path);
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){

                //out << ui->tableWidget->item(i, j)->text().toStdString() << "|";
                out << expressions[i][j].toStdString() << "|";
            }
    }
    out.close();
}
QString MainWindow::GetExpr(int row, int col){
    return expressions[row][col];
}

void MainWindow::on_OpenFile_clicked()
{

    CleanTable();
    string data;
    string file_path = "/home/oyemets/Documents/MyExcelFinal/";
    ifstream in;
    if(ui->file_name->text() != ""){
    file_path += ui->file_name->text().toStdString();

    in.open(file_path);}
    if(!in.is_open()){
        QMessageBox::critical(this, "Error!", "Unknown file!");
    }
    else{
        getline(in, data);

        vector<string> data_splitted;
        string word;
        for(int i =0; i < data.length(); i++){
            if(data[i] != '|'){
                word+=data[i];
            }
            else{
                data_splitted.push_back(word);
                word = "";
            }
        }
        int d = 0;

        for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
                for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                    QTableWidgetItem* itm = new QTableWidgetItem(QString::fromStdString(data_splitted[d]));
                    d++;
                    ui->tableWidget->setItem(i, j, itm);
                }
        }
        ChangeExpressions();
    }
}


void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    ShowText(currentRow, currentColumn);
}
void MainWindow::ShowText(int row, int column){
    ui->lineEdit->setText(expressions[row][column]);
}


void MainWindow::on_tableWidget_cellPressed(int row, int column)
{
    is_double_clicked = 1;
    changed_by_hands.first = row;
    changed_by_hands.second = column;

}


void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    ui->tableWidget->currentItem()->setText(arg1);
    changed_by_hands.first = ui->tableWidget->currentItem()->row();
    changed_by_hands.second = ui->tableWidget->currentItem()->column();
    UpdateText(changed_by_hands.first, changed_by_hands.second);
}


void MainWindow::on_pushButton_clicked()
{
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I");
  //  ChangeExpressions();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->tableWidget->rowCount()==9){
            QMessageBox::critical(this, "Помилка!","Неможливо додати рядок!");
        }
        else{
        ui->tableWidget->insertRow(1);
        std::cout << "done!" << std::endl;
        ui->tableWidget->setVerticalHeaderLabels(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10");
        //this->ChangeExpressions();
        }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->tableWidget->removeColumn(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I");
 //   ChangeExpressions();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->tableWidget->removeRow(1);
    ui->tableWidget->setVerticalHeaderLabels(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9");
 //   ChangeExpressions();
}
