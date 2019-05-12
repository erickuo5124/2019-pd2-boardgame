#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#define moveable true

enum {fuhyo, kyosha, keima, ginsho, kakugyo, hisha, kinsho, gyokusho, osho,
      tokin, narikyo, narikei, narigin, ryuma, ryuo, empty};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("將棋"));

    setUp();

    reset();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUp(){

    this->setFixedSize(1200, 900);//need

    ui->mainToolBar->setVisible(false);
    this->setStyleSheet("QMainWindow{background-image:url(:/src/img/background.jpg);}");
    boardTable = new QTableWidget(9, 9, this);
    boardTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    boardTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    boardTable->horizontalHeader()->setVisible(false);
    boardTable->verticalHeader()->setVisible(false);
    boardTable->horizontalHeader()->setDefaultSectionSize(78);
    boardTable->verticalHeader()->setDefaultSectionSize(88);
    boardTable->setGeometry(250, 50, 702, 792);
    boardTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    boardTable->setFocusPolicy(Qt::NoFocus);
    boardTable->setSelectionMode(QAbstractItemView::NoSelection);
    boardTable->setIconSize(QSize(85, 85));
    boardTable->setStyleSheet("QTableWidget{background: transparent;}");
    boardTable->setShowGrid(false);
    boardTable->setFrameStyle(QFrame::NoFrame);

    resetButton = new QPushButton("reset", this);
    resetButton->setGeometry(1100, 0, 50, 50);
    editButton = new QPushButton("edit", this);
    editButton->setGeometry(1150, 0, 50, 50);

    clickType = empty; clickTeam = 0;
    clickx = 0; clicky = 0;

    label[0] = new QLabel(this);
    label[0]->setGeometry(1000, 800, 141, 81);
    label[0]->setText(tr("Player1"));
    label[0]->setAlignment(Qt::AlignCenter);
    label[0]->setStyleSheet("QLabel {font-size: 35px;font-family: Arial;background-color:transparent;}");
    label[1] = new QLabel(this);
    label[1]->setGeometry(50, 20, 141, 81);
    label[1]->setText(tr("Player2"));
    label[1]->setAlignment(Qt::AlignCenter);
    label[1]->setStyleSheet("QLabel {font-size: 35px;font-family: Arial;background-color:transparent;}");

    pic.push_back(QPixmap(":/src/img/fuhyo.png"));
    pic.push_back(QPixmap(":/src/img/kyosha.png"));
    pic.push_back(QPixmap(":/src/img/keima.png"));
    pic.push_back(QPixmap(":/src/img/ginsho.png"));
    pic.push_back(QPixmap(":/src/img/kakugyo.png"));
    pic.push_back(QPixmap(":/src/img/hisha.png"));
    pic.push_back(QPixmap(":/src/img/kinsho.png"));
    pic.push_back(QPixmap(":/src/img/gyokusho.png"));
    pic.push_back(QPixmap(":/src/img/osho.png"));
    pic.push_back(QPixmap(":/src/img/tokin.png"));
    pic.push_back(QPixmap(":/src/img/narikyo.png"));
    pic.push_back(QPixmap(":/src/img/narikei.png"));
    pic.push_back(QPixmap(":/src/img/narigin.png"));
    pic.push_back(QPixmap(":/src/img/ryuma.png"));
    pic.push_back(QPixmap(":/src/img/ryuo.png"));

    for(int i=0; i<2; ++i){
        for(int j=0; j<7; ++j){
            capturebut[i][j] = new QPushButton(QIcon(pic[j]), "", this);
            capturebut[i][j]->setStyleSheet("QPushButton {font-size: 22px;font-family: Arial;background-color:transparent;}");
            capturebut[i][j]->setIconSize(QSize(85, 85));
            capturebut[i][j]->setGeometry(1050-1000*i, 80+100*j, 85, 85);
            connect(capturebut[i][j], &QPushButton::clicked, [this, i, j](){drop(i, j);});
        }
    }

    king[0] = new QPushButton(QIcon(pic[osho]), "", this);
    king[0]->setStyleSheet("QPushButton {background-color:transparent;}");
    king[0]->setIconSize(QSize(85, 85));
    king[0]->setGeometry(110, 780, 85, 85);
    king[1] = new QPushButton(QIcon(pic[gyokusho]), "", this);
    king[1]->setStyleSheet("QPushButton {background-color:transparent;}");
    king[1]->setIconSize(QSize(85, 85));
    king[1]->setGeometry(25, 780, 85, 85);

    connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetClicked()));
    connect(editButton, SIGNAL(clicked(bool)), this, SLOT(editClicked()));
}

void MainWindow::reset(){
    round = 0;
    label[0]->setText(tr("*Player1*"));
    //reset
    for(int i=0; i<9; ++i)
        for(int j=0; j<9; j++){
            board[i][j] = empty;
            moveBoard[i][j] = false;
        }
    for(int i=0; i<2; ++i)
        for(int j=0; j<7; ++j)
            captureChess[i][j] = 0;
    for(int i=0; i<2; ++i){
        for(int j=0; j<7; ++j){
            capturebut[i][j]->setIcon(QIcon());
            capturebut[i][j]->setText("");
        }
    }
    king[0]->setIcon(QIcon());
    king[1]->setIcon(QIcon());

    //fuhyo
    for(int j=0; j<9; ++j){
        board[6][j] = fuhyo; team[6][j] = 0;
    }
    for(int j=0; j<9; ++j){
        board[2][j] = fuhyo; team[2][j] = 1;
    }

    //kyosha
    board[0][0] = kyosha; team[0][0] = 1;
    board[0][8] = kyosha; team[0][8] = 1;
    board[8][0] = kyosha; team[8][0] = 0;
    board[8][8] = kyosha; team[8][8] = 0;

    //keima
    board[0][1] = keima; team[0][1] = 1;
    board[0][7] = keima; team[0][7] = 1;
    board[8][1] = keima; team[8][1] = 0;
    board[8][7] = keima; team[8][7] = 0;

    //ginsho
    board[0][2] = ginsho; team[0][2] = 1;
    board[0][6] = ginsho; team[0][6] = 1;
    board[8][2] = ginsho; team[8][2] = 0;
    board[8][6] = ginsho; team[8][6] = 0;

    //kinsho
    board[0][3] = kinsho; team[0][3] = 1;
    board[0][5] = kinsho; team[0][5] = 1;
    board[8][3] = kinsho; team[8][3] = 0;
    board[8][5] = kinsho; team[8][5] = 0;

    //kakugyo
    board[1][7] = kakugyo; team[1][7] = 1;
    board[7][1] = kakugyo; team[7][1] = 0;

    //hisha
    board[1][1] = hisha; team[1][1] = 1;
    board[7][7] = hisha; team[7][7] = 0;

    //gyokusho
    board[0][4] = gyokusho; team[0][4] = 1;

    //osho
    board[8][4] = osho; team[8][4] = 0;

    for(int i=0; i<9; ++i)
        for(int j=0; j<9; ++j){
            setimage(i, j);
        }
    connect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(myCellClicked(int, int)));
    connect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(myCellClicked(int, int)));
    disconnect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(secondClicked(int, int)));
}

void MainWindow::setimage(int row, int col){
    switch(board[row][col]){
        case empty:
            boardTable->setItem(row, col, new QTableWidgetItem(QIcon(),""));
        break;
        default:
            if(team[row][col])
                boardTable->setItem(row, col, new QTableWidgetItem(QIcon(pic[board[row][col]].transformed(QTransform().rotate(180))),""));
            else
                boardTable->setItem(row, col, new QTableWidgetItem(QIcon(pic[board[row][col]]),""));
        break;
    }
}

void MainWindow::setmove(int row, int col){
    if(row>=9 || row<0 || col>=9 || col<0)
        return;
    else if (board[row][col]!=empty && team[row][col]==team[clickx][clicky])
        return;
    else {
        moveBoard[row][col] = moveable;
        boardTable->item(row, col)->setBackgroundColor(Qt::blue);
    }
}

void MainWindow::myCellClicked(int row, int col){
    disconnect(editButton, SIGNAL(clicked(bool)), this, SLOT(editClicked()));
    if(team[row][col] != round)
        return;
    clickx = row;
    clicky = col;
    for(int i=0; i<9; ++i)
        for(int j=0; j<9; ++j)
            moveBoard[i][j] = false;

    switch (board[row][col]) {
        case empty:
            return;
        case fuhyo:
            if(team[row][col])
                setmove(row+1, col);
            else
                setmove(row-1, col);
        break;
        case kyosha:
            if(team[row][col]){
                for(int i=row+1; i<9; ++i){
                    if(board[i][col]!=empty && team[i][col]!=team[row][col]){
                        setmove(i, col);
                        break;
                    }else if (board[i][col]!=empty)
                        break;
                    setmove(i, col);
                }
            }else {
                for(int i=row-1; i>=0; --i){
                    if(board[i][col]!=empty && team[i][col]!=team[row][col]){
                        setmove(i, col);
                        break;
                    }else if (board[i][col]!=empty)
                        break;
                    setmove(i, col);
                }
            }
        break;
        case keima:
            if(team[row][col]){
                setmove(row+2, col+1);
                setmove(row+2, col-1);
            }else{
                setmove(row-2, col+1);
                setmove(row-2, col-1);
            }
        break;
        case ginsho:
            if(team[row][col]){
                setmove(row+1, col);
                setmove(row+1, col+1);
                setmove(row+1, col-1);
                setmove(row-1, col+1);
                setmove(row-1, col-1);
                setmove(row-1, col+1);
            }else{
                setmove(row-1, col);
                setmove(row-1, col+1);
                setmove(row-1, col-1);
                setmove(row+1, col+1);
                setmove(row+1, col-1);
            }
        break;
        case kinsho:case tokin:case narikyo: case narikei:case narigin:
            if(team[row][col]){
                setmove(row+1, col);
                setmove(row+1, col+1);
                setmove(row+1, col-1);
                setmove(row-1, col);
                setmove(row, col-1);
                setmove(row, col+1);
            }else{
                setmove(row-1, col);
                setmove(row-1, col+1);
                setmove(row-1, col-1);
                setmove(row, col+1);
                setmove(row, col-1);
                setmove(row+1, col);
            }
        break;
        case kakugyo:
            for(int i=row+1, j=col+1; i<9&&j<9; ++i, ++j){
                if(board[i][j]!=empty && team[i][j]!=team[row][col]){
                    setmove(i, j);
                    break;
                }else if (board[i][j]!=empty)
                    break;
                setmove(i, j);
            }
            for(int i=row-1, j=col-1; i>=0&&j>=0; --i, --j){
                if(board[i][j]!=empty && team[i][j]!=team[row][col]){
                    setmove(i, j);
                    break;
                }else if (board[i][j]!=empty)
                    break;
                setmove(i, j);
            }
            for(int i=row-1, j=col+1; i>=0&&j<9; --i, ++j){
                if(board[i][j]!=empty && team[i][j]!=team[row][col]){
                    setmove(i, j);
                    break;
                }else if (board[i][j]!=empty)
                    break;
                setmove(i, j);
            }
            for(int i=row+1, j=col-1; i<9&&j>=0; ++i, --j){
                if(board[i][j]!=empty && team[i][j]!=team[row][col]){
                    setmove(i, j);
                    break;
                }else if (board[i][j]!=empty)
                    break;
                setmove(i, j);
            }
        break;
        case hisha:
            for(int i=row+1; i<9; ++i){
                if(board[i][col]!=empty && team[i][col]!=team[row][col]){
                    setmove(i, col);
                    break;
                }else if (board[i][col]!=empty)
                    break;
                setmove(i, col);
            }
            for(int i=row-1; i>=0; --i){
                if(board[i][col]!=empty && team[i][col]!=team[row][col]){
                    setmove(i, col);
                    break;
                }else if (board[i][col]!=empty)
                    break;
                setmove(i, col);
            }
            for(int j=col+1; j<9; ++j){
                if(board[row][j]!=empty && team[row][j]!=team[row][col]){
                    setmove(row, j);
                    break;
                }else if (board[row][j]!=empty)
                    break;
                setmove(row, j);
            }
            for(int j=col-1; j>=0; --j){
                if(board[row][j]!=empty && team[row][j]!=team[row][col]){
                    setmove(row, j);
                    break;
                }else if (board[row][j]!=empty)
                    break;
                setmove(row, j);
            }
        break;
        case gyokusho: case osho:
            setmove(row+1, col);
            setmove(row+1, col+1);
            setmove(row+1, col-1);
            setmove(row-1, col);
            setmove(row-1, col+1);
            setmove(row-1, col-1);
            setmove(row, col-1);
            setmove(row, col+1);
        break;
        case ryuma:
            setmove(row+1, col);
            setmove(row-1, col);
            setmove(row, col+1);
            setmove(row, col-1);
            for(int i=row+1, j=col+1; i<9&&j<9; ++i, ++j){
                if(board[i][j]!=empty && team[i][j]!=team[row][col]){
                    setmove(i, j);
                    break;
                }else if (board[i][j]!=empty)
                    break;
                setmove(i, j);
            }
            for(int i=row-1, j=col-1; i>=0&&j>=0; --i, --j){
                if(board[i][j]!=empty && team[i][j]!=team[row][col]){
                    setmove(i, j);
                    break;
                }else if (board[i][j]!=empty)
                    break;
                setmove(i, j);
            }
            for(int i=row-1, j=col+1; i>=0&&j<9; --i, ++j){
                if(board[i][j]!=empty && team[i][j]!=team[row][col]){
                    setmove(i, j);
                    break;
                }else if (board[i][j]!=empty)
                    break;
                setmove(i, j);
            }
            for(int i=row+1, j=col-1; i<9&&j>=0; ++i, --j){
                if(board[i][j]!=empty && team[i][j]!=team[row][col]){
                    setmove(i, j);
                    break;
                }else if (board[i][j]!=empty)
                    break;
                setmove(i, j);
            }
        break;
        case ryuo:
            setmove(row+1, col+1);
            setmove(row+1, col-1);
            setmove(row-1, col+1);
            setmove(row-1, col-1);
            for(int i=row+1; i<9; ++i){
                if(board[i][col]!=empty && team[i][col]!=team[row][col]){
                    setmove(i, col);
                    break;
                }else if (board[i][col]!=empty)
                    break;
                setmove(i, col);
            }
            for(int i=row-1; i>=0; --i){
                if(board[i][col]!=empty && team[i][col]!=team[row][col]){
                    setmove(i, col);
                    break;
                }else if (board[i][col]!=empty)
                    break;
                setmove(i, col);
            }
            for(int j=col+1; j<9; ++j){
                if(board[row][j]!=empty && team[row][j]!=team[row][col]){
                    setmove(row, j);
                    break;
                }else if (board[row][j]!=empty)
                    break;
                setmove(row, j);
            }
            for(int j=col-1; j>=0; --j){
                if(board[row][j]!=empty && team[row][j]!=team[row][col]){
                    setmove(row, j);
                    break;
                }else if (board[row][j]!=empty)
                    break;
                setmove(row, j);
            }
        break;
    }
    connect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(secondClicked(int, int)));
    disconnect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(myCellClicked(int, int)));
}

void MainWindow::secondClicked(int row, int col){
    if(moveBoard[row][col]){
        //capture
        if(board[row][col]!=empty){
            if(board[row][col]==gyokusho || board[row][col]==osho){
                QMessageBox::information(this, tr("遊戲結束"),"玩家"+QString::number(int(!team[row][col])+1)+"獲勝", QMessageBox::Ok, QMessageBox::Ok);
                reset();
                return;
            }else if(board[row][col]>osho){
                capture(int(team[row][col]),board[row][col]-9);
            }else{
                capture(int(team[row][col]),board[row][col]);
            }
        }

        board[row][col] = board[clickx][clicky];
        team[row][col] = team[clickx][clicky];
        board[clickx][clicky] = empty;
        for(int i=0; i<9; ++i)
            for(int j=0; j<9; ++j)
                if(moveBoard[i][j])
                    boardTable->item(i, j)->setBackground(Qt::transparent);
        setimage(clickx, clicky);

        //promotion
        if(team[row][col]){
            if(clickx>5 || row>5)
                if(board[row][col]<kinsho)
                    promotion(row, col);
        }else{
            if(clickx<3 || row<3)
                if(board[row][col]<kinsho)
                    promotion(row, col);
        }
        setimage(row, col);
        round = !round;
        label[round]->setText("*Player"+QString::number(int(round)+1)+"*");
        label[!round]->setText("Player"+QString::number(int(!round)+1));
    }else {
        for(int i=0; i<9; ++i)
            for(int j=0; j<9; ++j)
                if(moveBoard[i][j]){
                    moveBoard[i][j] = !moveBoard[i][j];
                    setimage(i, j);
                    boardTable->item(i, j)->setBackground(Qt::transparent);
                }
    }
    connect(editButton, SIGNAL(clicked(bool)), this, SLOT(editClicked()));
    connect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(myCellClicked(int, int)));
    disconnect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(secondClicked(int, int)));
}

void MainWindow::capture(int team, int type){
    captureChess[!team][type]++;
    capturebut[!team][type]->setIcon(QIcon(pic[type]));
    capturebut[!team][type]->setText(QString::number(captureChess[!team][type]));
}

void MainWindow::promotion(int row, int col){
    switch (board[row][col]) {
        case fuhyo: case kyosha:
            if(team[row][col]){
                if(row == 8){
                    board[row][col]+=9;
                    return;
                }
            }else if(row == 0){
                board[row][col]+=9;
                return;
            }
        break;
        case keima:
            if(team[row][col]){
                if(row > 6){
                    board[row][col]+=9;
                    return;
                }
            }else if(row < 2){
                board[row][col]+=9;
                return;
            }
        break;
        default:
        break;
    }
    switch(QMessageBox::question(this,tr("升變"),tr("要升變嗎?"),QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel)){
        case QMessageBox::Ok:
            board[row][col]+=9;
        break;
        default:
        break;
    }
}

void MainWindow::drop(int i, int j){
    disconnect(editButton, SIGNAL(clicked(bool)), this, SLOT(editClicked()));
    clickTeam = i; clickType = j;
    if(bool(clickTeam)!=round)
        return;
    if(captureChess[clickTeam][clickType] == 0)
        return;
    disconnect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(myCellClicked(int, int)));


    for(int i=0; i<9; ++i)
        for(int j=0; j<9; ++j)
            if(moveBoard[i][j]){
                moveBoard[i][j] = !moveBoard[i][j];
                setimage(i, j);
                boardTable->item(i, j)->setBackground(Qt::transparent);
            }

    switch (clickType) {
        case fuhyo:
            for(int j=0, i; j<9; ++j){
                for(i=0; i<9; ++i)
                    if(board[i][j] == fuhyo && team[i][j] == round)
                        break;

                if(i == 9)
                    for(i=0+(clickTeam==0); i<9-clickTeam; ++i)
                        if(board[i][j] == empty)
                            setmove(i, j);
            }
        break;
        case kyosha:
            if(clickTeam){
                for(int i=0; i<8; ++i)
                    for(int j=0; j<9; ++j)
                        if(board[i][j] == empty)
                            setmove(i, j);
            }else{
                for(int i=1; i<9; ++i)
                    for(int j=0; j<9; ++j)
                        if(board[i][j] == empty)
                            setmove(i, j);
            }
        break;
        case keima:
            if(clickTeam){
                for(int i=0; i<7; ++i)
                    for(int j=0; j<9; ++j)
                        if(board[i][j] == empty)
                            setmove(i, j);
            }else{
                for(int i=2; i<9; ++i)
                    for(int j=0; j<9; ++j)
                        if(board[i][j] == empty)
                            setmove(i, j);
            }
        break;
        default:
            for(int i=0; i<9; ++i)
                for(int j=0; j<9; ++j)
                    if(board[i][j] == empty)
                        setmove(i, j);
        break;
    }
    connect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(dropSecondClicked(int, int)));
}

void MainWindow::dropSecondClicked(int row, int col){
    if(moveBoard[row][col]){
        board[row][col] = clickType;
        team[row][col] = clickTeam;
        captureChess[clickTeam][clickType]--;
        if(captureChess[clickTeam][clickType] == 0){
            capturebut[clickTeam][clickType]->setIcon(QIcon());
            capturebut[clickTeam][clickType]->setText("");
        }else
            capturebut[clickTeam][clickType]->setText(QString::number(captureChess[clickTeam][clickType]));

        for(int i=0; i<9; ++i)
            for(int j=0; j<9; ++j)
                if(moveBoard[i][j])
                    boardTable->item(i, j)->setBackground(Qt::transparent);
        setimage(row, col);
        round = !round;
        label[round]->setText("*Player"+QString::number(int(round)+1)+"*");
        label[!round]->setText("Player"+QString::number(int(!round)+1));
    }else {
        for(int i=0; i<9; ++i)
            for(int j=0; j<9; ++j)
                if(moveBoard[i][j]){
                    moveBoard[i][j] = !moveBoard[i][j];
                    setimage(i, j);
                    boardTable->item(i, j)->setBackground(Qt::transparent);
                }
    }
    connect(editButton, SIGNAL(clicked(bool)), this, SLOT(editClicked()));
    connect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(myCellClicked(int, int)));
    disconnect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(dropSecondClicked(int, int)));
}

void MainWindow::resetClicked(){
    reset();
}

void MainWindow::editClicked(){
    disconnect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(myCellClicked(int, int)));
    for(int i=0; i<2; ++i){
        for(int j=0; j<7; ++j){
            capturebut[i][j]->setIcon(QIcon(pic[j]));
            capturebut[i][j]->setText("");
            capturebut[i][j]->disconnect();
            connect(capturebut[i][j], &QPushButton::clicked, [this, i, j](){putChess(i, j);});
        }
    }

    int i=0, j=8;
    king[0]->setIcon(QIcon(pic[osho]));
    connect(king[0], &QPushButton::clicked, [this, i, j](){putChess(i, j);});
    i=1; j=7;
    king[1]->setIcon(QIcon(pic[gyokusho]));
    connect(king[1], &QPushButton::clicked, [this, i, j](){putChess(i, j);});

    connect(editButton, SIGNAL(clicked(bool)), this, SLOT(editSecondClicked()));
    disconnect(editButton, SIGNAL(clicked(bool)), this, SLOT(editClicked()));
}

void MainWindow::putChess(int i, int j){
    clickTeam = i; clickType = j;
    connect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(putChess2(int, int)));
}

void MainWindow::putChess2(int i, int j){
    if(board[i][j] == empty){
        board[i][j] = clickType;
        team[i][j] = clickTeam;
        setimage(i, j);
    }else {
        board[i][j] = empty;
        setimage(i, j);
    }
}

void MainWindow::editSecondClicked(){
    for(int i=0; i<2; ++i){
        for(int j=0; j<7; ++j){
            capturebut[i][j]->disconnect();
            connect(capturebut[i][j], &QPushButton::clicked, [this, i, j](){drop(i, j);});
            if(captureChess[i][j]>0){
                capturebut[i][j]->setIcon(QIcon(pic[j]));
                capturebut[i][j]->setText(QString::number(captureChess[i][j]));
            }else {
                capturebut[i][j]->setIcon(QIcon());
            }
        }
    }
    king[0]->disconnect();
    king[0]->setIcon(QIcon());
    king[1]->disconnect();
    king[1]->setIcon(QIcon());

    connect(editButton, SIGNAL(clicked(bool)), this, SLOT(editClicked()));
    connect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(myCellClicked(int, int)));
    disconnect(boardTable, SIGNAL(cellClicked(int, int)), this, SLOT(putChess2(int, int)));
    disconnect(editButton, SIGNAL(clicked(bool)), this, SLOT(editSecondClicked()));
}
