#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QMouseEvent>
 #include <QStack>
#include "stone.h"
#include "Step.h"

class Board : public QWidget
{
public:
    int D = 40;
    //select代表选中的棋子
    int selectid = -1;
    //clickid代表再次点击的棋子id
    int clickid = -1;
    bool redTurn = false;
    QStack<Step> steps;
    Stone stones[32];

    explicit Board(QWidget *p = 0);

    //绘制
    void drawStones(QPainter &painter,int);
    double animationFuc(int d,int i);
    void Transition(int id,int to_row,int to_col);

    //工具函数
    QPoint getPos(int r,int c);
    QPoint getPosById(int);
    QPoint getXYById(int);
    bool sameColor(int,int);
    bool getRowCol(QPoint p,int &row,int &col);
    bool lineHaveStone(int r1,int c1,int r2,int c2);
    int lineStoneNum(int r1,int c1,int r2,int c2);
    bool slashHaveStone(int r1,int c1,int r2,int c2);
    bool isLineMove(int moveid,int row,int col);
    bool haveStone(int row,int col);
    int getId(int row, int col);
    void saveStep(int,int,int,int);
    void clearSteps();
    void initAllStone();

    //规则函数
    bool canMove(int,int,int,int);
    bool canMove_JIANG(int,int,int,int);
    bool canMove_SHI(int,int,int,int);
    bool canMove_XIANG(int,int,int,int);
    bool canMove_MA(int,int,int,int);
    bool canMove_CHE(int,int,int,int);
    bool canMove_PAO(int,int,int,int);
    bool canMove_BING(int,int,int,int);
    void checkGameover(int);

    //棋盘操作
    void makeMove(int,int,int,int);
    void cancelOneStep();
    void regretStep();
    virtual void dealClick(int,int,int);

    //事件监听
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //事件处理
    void click(QPoint);

};

#endif // BOARD_H
