#include "singleGame.h"
#include <QtWidgets>

SingleGame::SingleGame(QWidget *parent) : Board(parent)
{
    QPushButton * backStepBtn = new QPushButton(this);
    QPushButton * reStartBtn = new QPushButton(this);
    backStepBtn->setText(QString::fromLocal8Bit("悔棋"));
    backStepBtn->move(450,100);
    reStartBtn->setText(QString::fromLocal8Bit("重新开始"));
    reStartBtn->move(450,130);

    connect(backStepBtn,&QPushButton::clicked,this,&SingleGame::regretStep);
    connect(reStartBtn,&QPushButton::clicked,this,&SingleGame::reStart);
}

void SingleGame::reStart()
{
    initAllStone();
    clearSteps();
    selectid = -1;
    redTurn = false;
    update();
}

/*对id，移动到row col的操作，*/
void SingleGame::dealClick(int id,int row,int col)
{
    Board::dealClick(id,row,col);
    if(redTurn)
        AIMove();
}


//工具函数
void SingleGame::getAllPossibleMove(QVector<AiStep> &aiSteps)
{
    int start,end;
    if(fack_RedTurn){
        start=0;end=16;
    }else{
        start=16;end=32;
    }
    for (int i = start ;i < end; i++) {
        if(stones[i]._dead)
            continue;
        for (int row = 0;row <= 9;row++) {
            for (int col = 0;col <= 8; col++) {
                int killid = getId(row,col);
                //同色不能动
                if(sameColor(i,killid))
                    continue;
                if(canMove(i,row,col,killid))
                {
                    AiStep s;
                    s.id = i;
                    s.from_r = stones[i]._row;
                    s.from_c = stones[i]._col;
                    s.to_r = row;
                    s.to_c = col;
                    s.killid = killid;
                    s.score = 0;
                    aiSteps.append(s);
                }

            }
        }
    }
}
void SingleGame::backupBoard()
{
    for (int i = 0;i<32;i++) {
        vir_board[i] = stones[i];
    }
}
int SingleGame::highScoreStepIndex(QVector<AiStep> &aiSteps)
{
    int i=0,max=0,maxi=0;
    for(auto it = aiSteps.begin();it < aiSteps.end();it++,i++)
    {
        if((*it).score > max)
        {
            max = (*it).score;
            maxi = i;
        }
    }
    return maxi;
}

//人工智能
void SingleGame::AIMove(){
    AiStep bestStep;
    //备份棋盘
    //backupBoard();
    fack_RedTurn = true;
    //找最好的一步
    bestStep = foundBestStep(3);

    //走棋
    makeMove(bestStep.id,bestStep.to_r,bestStep.to_c,bestStep.killid);
}
inline void SingleGame::fackMove(AiStep *as)
{
    if(as->killid != -1)
    {
        stones[as->killid]._dead = true;
    }
    stones[as->id]._row = as->to_r;
    stones[as->id]._col = as->to_c;
}
void SingleGame::scoreStep(AiStep *as)
{
    if(as->killid != -1)
    {
        as->score += stones[as->killid]._value;
    }
}
inline void SingleGame::cancelFackMove(AiStep *as)
{
    if(as->killid != -1)
    {
        stones[as->killid]._dead = false;
    }
    stones[as->id]._row = as->from_r;
    stones[as->id]._col = as->from_c;
}
AiStep SingleGame::foundBestStep(int d)
{
    QVector<AiStep> allsteps;
    int maxi;
    AiStep bs;
    //找可能的走法
    getAllPossibleMove(allsteps);
    for(auto it = allsteps.begin();it < allsteps.end();it++)
    {
        fackMove(it);
        scoreStep(it);
        if(d>0)
        {
            AiStep s;
            fack_RedTurn = !fack_RedTurn;
            s = foundBestStep(d-1);
            it->score += (-s.score);
        }
        cancelFackMove(it);
    }
    //allsteps找一条评分最高的，返回
    maxi = highScoreStepIndex(allsteps);
    bs = allsteps[maxi];
    allsteps.clear();
    return bs;
}


