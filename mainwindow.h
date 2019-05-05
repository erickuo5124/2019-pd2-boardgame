#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setUp();
    void reset();
    QPushButton *resetButton, *editButton;

private:
    Ui::MainWindow *ui;
    QTableWidget *boardTable;
    int board[9][9];
    bool moveBoard[9][9];
    bool team[9][9];
    int clickx, clicky, clickTeam, clickType;
    void setimage(int, int);
    void setmove(int, int);
    QList<QPixmap>pic;
    int captureChess[2][7];
    void capture(int, int);
    bool round;
    void promotion(int, int);
    QPushButton *capturebut[2][7];
    QLabel *label[2];

public slots:
    void myCellClicked(int, int);
    void secondClicked(int, int);
    void resetClicked();
    void editClicked();
    void drop(int, int);
    void dropSecondClicked(int, int);
};
//
#endif // MAINWINDOW_H
