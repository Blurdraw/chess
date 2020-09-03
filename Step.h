#ifndef STEP_H
#define STEP_H
/*
id为此次操作的id
prior_r 为原先所在的行
 killid代表被吃的子，若没有吃，置为-1.
*/
struct Step
{
   int id;
   int prior_r;
   int prior_c;
   int killid;
   bool red_turn;
};

struct AiStep
{
    int id;
    int from_r;
    int from_c;
    int to_r;
    int to_c;
    int killid;
    int score;
};

#endif // STEP_H
