#ifndef SINGLEGAME_H
#define SINGLEGAME_H
#include "Board.h"

class SingleGame :public Board
{
private:

    Stone vir_board[32];
    Step last_step;//
    bool fack_RedTurn = true;

public:
    explicit SingleGame(QWidget *p = 0);
    void dealClick(int id, int row, int col);
    void reStart();

    //工具函数
    int highScoreStepIndex(QVector<AiStep> &);
    void backupBoard();
    void getAllPossibleMove(QVector<AiStep> &);

    //人工智能
    void AIMove();    
    void fackMove(AiStep*);
    void scoreStep(AiStep*);
    void cancelFackMove(AiStep *);
    AiStep foundBestStep(int);


};

#endif // SINGLEGAME_H
