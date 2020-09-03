#include "Board.h"
#include <QPainter>
#include <QMessageBox>
#include <QPointF>
#include <QThread>

Board::Board(QWidget *parent) : QWidget(parent)
{

    for (int i = 0;i<32;i++) {
        stones[i].init(i,D);
    }
}

//工具函数
QPoint Board::getPosById(int id)
{
    return getPos(stones[id]._row,stones[id]._col);
}
QPoint Board::getXYById(int id)
{
    return QPoint(stones[id]._x,stones[id]._y);
}
QPoint Board::getPos(int row,int col){
    return QPoint((col+1) * D,(row+1) * D);
}
bool Board::sameColor(int id1,int id2)
{
    if(id1 != -1 && id2 != -1)
        return stones[id1]._red == stones[id2]._red;
    return false;
}
bool Board::getRowCol(QPoint p,int &row,int &col)
{
    for (row = 0; row <= 9; row++) {
        for (col = 0;col <= 8;col++) {
            QPoint center = getPos(row,col);
            int dx = center.x() - p.x();
            int dy = center.y() - p.y();
            if(dx*dx + dy*dy < D*D/4)
                return true;
        }
    }
    return false;
}
/*判断直线方向上有没有棋子*/
bool Board::lineHaveStone(int r1,int c1,int r2,int c2)
{
    if(lineStoneNum(r1,c1,r2,c2))
        return true;
    return false;
}
/*判断斜线方向上中间有没有棋子*/
bool Board::slashHaveStone(int r1,int c1,int r2,int c2)
{
    //中间位置的行列
    int r,c;
    r = (r1 + r2)/2;
    c = (c1 + c2)/2;
    for (int i = 0;i<32;i++) {
        if(stones[i]._row == r && stones[i]._col == c  && !stones[i]._dead)
            return true;
    }
    return false;
}
/*判断是否直线前进*/
bool Board::isLineMove(int moveid,int row,int col)
{
    int dr = abs(stones[moveid]._row - row);
    int dc = abs(stones[moveid]._col - col);
    if((dr>0 && dc ==0 )||(dr == 0 && dc > 0))
        return true;
    return false;
}
/*判断给定行列是否有棋子*/
bool Board::haveStone(int row,int col)
{
    for (int i = 0;i<32;i++) {
        if(stones[i]._row == row && stones[i]._col == col && stones[i]._dead == false)
        {
            return true;
        }
    }
    return false;
}
/*返回没有被吃的棋子的id，否则返回-1*/
int Board::getId(int row,int col)
{
    int i;
    for(i = 0;i<32;i++)
    {
        if(stones[i]._row == row && stones[i]._col == col && stones[i]._dead == false)
        {
            break;
        }
    }
    if(i<32)
    {
        return i;
    }
    return -1;
}
void Board::saveStep(int id,int pr,int pc,int killid)
{
    Step s;
    s.id = id;
    s.prior_c = pc;
    s.prior_r = pr;
    s.killid = killid;
    //
    s.red_turn = !redTurn;
    steps.push(s);
}
void Board::initAllStone(){
    for (int i = 0;i<32;i++) {
        stones[i].init(i,D);
    }
}
void Board::clearSteps(){
    if(!steps.isEmpty())
        steps.pop();
}
int Board::lineStoneNum(int r1,int c1,int r2,int c2)
{
    bool sameRow;
    bool forward;
    int r,c,num=0;
    if(r1 - r2 == 0)
    {
        sameRow = true;
        c1<c2?forward=true:forward=false;
        forward?c=c1+1:c=c1-1;
        r = r1;

    } else {
        sameRow = false;
        r1<r2?forward=true:forward=false;
        c = c1;
        forward?r=r1+1:r=r1-1;
    }
    while(sameRow?(forward?c<c2:c>c2):(forward?r<r2:r>r2)){
        for (int i = 0;i < 32; i++) {
            if(stones[i]._row == r && stones[i]._col == c && !stones[i]._dead)
                num++;
        }
        if(sameRow)
            forward?c++:c--;
        else
            forward?r++:r--;
    }
    return num;
}

//规则函数
bool Board::canMove_JIANG(int moveid,int row,int col,int killid)
{
    //两将是否对面
    if(killid != -1 && (stones[killid]._type == JIANG || stones[killid]._type == SHUAI))
    {
        if(isLineMove(moveid,row,col) && !lineHaveStone(stones[moveid]._row,stones[moveid]._col,row,col))
            return true;
    }


    //确保位置在九宫格内
    if(stones[moveid]._red)
    {
        if(row > 2)return false;
    }
    else
    {
        if(row < 7) return false;
    }
    if(col < 3)return false;
    if(col > 5)return false;

    //确保移动一格
    int dr = stones[moveid]._row - row;
    int dc = stones[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d != 1 && d != 10)
        return false;
    return true;
}
bool Board::canMove_SHI(int moveid,int row,int col,int )
{
    //确保位置在九宫格内
    if(stones[moveid]._red)
    {
        if(row > 2)return false;
    }
    else
    {
        if(row < 7) return false;
    }
    if(col < 3)return false;
    if(col > 5)return false;

    //确保沿着对角线移动一格
    int dr = stones[moveid]._row - row;
    int dc = stones[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 11)
        return true;

    return false;
}
bool Board::canMove_XIANG(int moveid,int row,int col,int )
{
    //不能过河
    if(stones[moveid]._red)
    {
        if(row > 4)
            return false;
    } else {
        if(row < 5)
            return false;
    }

    //走田字
    int dr = stones[moveid]._row - row;
    int dc = stones[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d != 22)
        return false;

    //田中不能有字
    if(slashHaveStone(stones[moveid]._row,stones[moveid]._col,row,col))
        return false;

    return true;
}
bool Board::canMove_CHE(int moveid,int row,int col,int )
{
    //必须直线前进
    if(!isLineMove(moveid,row,col))
        return false;
    //路线上不能有棋子
    if(lineHaveStone(stones[moveid]._row,stones[moveid]._col,row,col))
        return false;
    return true;
}
bool Board::canMove_BING(int moveid,int row,int col,int )
{
    //在己方时
    if(stones[moveid]._red){
        if(stones[moveid]._row < 5){
            //在己侧，不能横向移动
            if(col != stones[moveid]._col)
                return false;
        }
        //只能前进
        if(row < stones[moveid]._row)
            return false;

    } else {
        if(stones[moveid]._row > 4){
            if(col != stones[moveid]._col)
                return false;
        }
        if(row > stones[moveid]._row)
            return false;
    }

    //只能动一格
    int dr = stones[moveid]._row - row;
    int dc = stones[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 1 || d == 10)
        return true;
    return false;
}
bool Board::canMove_MA(int moveid,int row,int col,int )
{
    //必须走日
    int r,c;
    int dr = row - stones[moveid]._row;
    int dc = col - stones[moveid]._col;
    int d = abs(dr)*10 + abs(dc);
    if(d != 12 && d != 21)
        return false;

    //不能绊脚
    if(d == 12)
    {
        r = stones[moveid]._row;
        if(dc > 0)
            c = col - 1;
         else
            c = col + 1;
    } else {
        c = stones[moveid]._col;
        if(dr > 0)
             r = row - 1;
         else
             r = row + 1;
    }
    if(haveStone(r,c))
        return false;
    return true;
}
bool Board::canMove_PAO(int moveid,int row,int col,int killid)
{
    //必须是直线
    if(!isLineMove(moveid,row,col))
        return false;

    //吃子否？
    if(killid != -1){
        //吃子，路线中间必须有且只有一个子
        if(lineStoneNum(stones[moveid]._row,stones[moveid]._col,row,col) != 1)
            return false;

    } else {
        //只是移动，路线中间不能有子
        if(lineHaveStone(stones[moveid]._row,stones[moveid]._col,row,col))
            return false;
    }

    return true;
}
bool Board::canMove(int moveid,int row,int col,int killid)
{
    //传入的killid对应的棋子一定是没有被吃掉的，修改后满足不同色
    if(killid != -1)
    {
        if(sameColor(selectid,killid))
        {
            selectid = killid;
            update();
            return false;
        }
    }
    switch (stones[moveid]._type) {
        case JIANG:
            return canMove_JIANG(moveid,row,col,killid);
        case SHUAI:
            return canMove_JIANG(moveid,row,col,killid);
        case RED_SHI:
        case BLACK_SHI:
            return canMove_SHI(moveid,row,col,killid);
        case RED_XIANG:
        case BLACK_XIANG:
            return canMove_XIANG(moveid,row,col,killid);
        case MA:
            return canMove_MA(moveid,row,col,killid);
        case PAO:
            return canMove_PAO(moveid,row,col,killid);
        case CHE:
            return canMove_CHE(moveid,row,col,killid);
        case BING:
        case ZU:
            return canMove_BING(moveid,row,col,killid);

    }
    return true;
}
void Board::checkGameover(int killid)
{
    if(stones[killid]._type == SHUAI || stones[killid]._type == JIANG)
    {
        QString text;
        text = (stones[killid]._type == SHUAI)?QString::fromLocal8Bit("你赢了！再来一局吧！"):QString::fromLocal8Bit("你输了！再来一局吧！");
        QMessageBox msg(this);
        redTurn = false;
        msg.setWindowTitle("GameOver");
        msg.setIcon(QMessageBox::Information);
        msg.setText(text);
        msg.setStandardButtons(QMessageBox::Ok);
        if(msg.exec() == QMessageBox::Ok)
        {
            initAllStone();
            clearSteps();
            selectid = -1;
            redTurn = false;
            update();
        }
    }
}

//棋盘操作
/*处理棋子的点击移动操作*/
void Board::click(QPoint p)
{

    int row,col;
    //将p转化成行列
    //判断行列有没有超出范围
    bool r = getRowCol(p,row,col);
    if (r == false)
        return;

    clickid = getId(row,col);

    dealClick(clickid,row,col);
}
void Board::dealClick(int , int row, int col)
{

    if(selectid == -1)
    {
        //之前前没有棋子被选中，设置选中了棋子
        //确保选中的棋子颜色与下棋方颜色一致
        if((clickid < 16 && redTurn) || (clickid >= 16 && !redTurn))
        {
            selectid = clickid;
            update();
        }
    } else {
        //走棋
        if(canMove(selectid,row,col,clickid))
            makeMove(selectid,row,col,clickid);

    }
}
void Board::cancelOneStep()
{
    if(!steps.isEmpty())
    {
        Step laststep = steps.pop();
        redTurn = laststep.red_turn;
        selectid = laststep.id;
        stones[selectid]._row = laststep.prior_r;
        stones[selectid]._col = laststep.prior_c;
        stones[selectid]._x = (laststep.prior_c + 1) * D;
        stones[selectid]._y = (laststep.prior_r + 1) * D;
        if(laststep.killid != -1 && stones[laststep.killid]._dead)
        {
            stones[laststep.killid]._dead = false;
        }

    }
}
void Board::regretStep()
{
    cancelOneStep();
    cancelOneStep();
    update();
}
void Board::makeMove(int id,int row,int col,int killid)
{
    //提供的走法必须经过验证，能够走
    int last_r,last_c;
    last_r = stones[id]._row;
    last_c = stones[id]._col;
    //stones[id]._row = row;
    //stones[id]._col = col;

    Transition(id,row,col);

    if(killid != -1)
        stones[killid]._dead = true;
    redTurn = !redTurn;
    saveStep(id,last_r,last_c,killid);
    update();
    checkGameover(killid);
}

void Board::mouseReleaseEvent(QMouseEvent *event){ 
    QPoint p = event->pos();
    click(p);
}

void Board::paintEvent(QPaintEvent *){
    QPainter painter(this);
    int d = D;
    //10横线
    for (int i = 1;i <= 10;i++)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //9竖线
    for (int i = 1;i <= 9; i++) {
        if(i == 1 || i==9)
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
        painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
    }
    //斜线
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,d),QPoint(4*d,3*d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    //绘制棋子
    for(int i=0;i<32;i++)
    {
        if(stones[i]._dead)
            continue;
        drawStones(painter,i);
    }
}

/*在棋盘上画棋子*/
void Board::drawStones(QPainter &painter,int id){
    int r = D/2;
    QPoint center = getXYById(id);
    QRect rect = QRect(center.x() - r,center.y() - r, D,D);
    painter.setFont(QFont("STXingkai",D/2,700));
    if(id == selectid)
        painter.setBrush(QBrush(Qt::gray));
    else
        painter.setBrush(QBrush(Qt::yellow));

    if(stones[id]._red)
       painter.setPen(QColor(255,0,0));
    else
       painter.setPen(QColor(0,0,0));

    painter.drawEllipse(center,D/2,D/2);
    painter.drawText(rect,stones[id].getName(),QTextOption(Qt::AlignCenter));
}
void Board::Transition(int id,int to_row,int to_col)
{
    int dx,dy,r=D/2,fps=24,time = 1000;
    int n = time * 0.001 * fps;
    int dt = (int)(1/fps) * 1000;
    QPoint p1,p2; 
    p1 = getPos(stones[id]._row,stones[id]._col);    
    p2 = getPos(to_row,to_col);
    dx = p2.x() - p1.x();
    dy = p2.y() - p1.y();

    //绘制动画
    for(int i = 0;i<n;i++)
    {
        stones[id]._x += (int)animationFuc(dx,i);
        stones[id]._y += (int)animationFuc(dy,i);
        repaint();
        QThread::msleep(dt);
    }

    //恢复棋子
    stones[id]._row = to_row;
    stones[id]._col =to_col;
    stones[id]._x = p2.x();
    stones[id]._y = p2.y();
    selectid = -1;
    repaint();
}
double Board::animationFuc(int d,int ){
    //二次函数型过渡
    //return d * ((-0.008889)*i + 0.1333);

    return d * 1 / 24.0;
}

