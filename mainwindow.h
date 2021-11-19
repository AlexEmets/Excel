#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTabWidget>
#include <vector>
#include <QPixmap>
#include <utility>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString GetExpr(int row, int col);
private slots:
    void on_actionOpen_helper_triggered();

    void on_actionQuit_triggered();

    void on_tableWidget_cellChanged(int row, int column);

    void on_CalculateButton_clicked();

    void on_SetAboba_clicked();

    void on_SaveFile_clicked();

    void on_OpenFile_clicked();

   // void update_text_expression();

    //void on_expression_cursorPositionChanged(int arg1, int arg2);

    //void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_tableWidget_cellPressed(int row, int column);

    void on_lineEdit_textEdited(const QString &arg1);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    void CleanTable();
    void ShowText(int row, int column);
    void UpdateText(int row, int column);
    void ChangeExpressions();
    std::vector<std::vector<QString>> expressions;
    void changePic();
    std::vector<QPixmap> pixes;
    Ui::MainWindow *ui;
    std::pair<int, int> changed_by_hands;
    bool is_double_clicked = 0;
};
#endif // MAINWINDOW_H
