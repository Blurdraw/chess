#include "stone.h"
struct {
    int row, col,value;
    TYPE type;
} pos[32] = {
    {0,0,500,CHE},
    {0,1,200,MA},
    {0,2,50,RED_XIANG},
    {0,3,100,RED_SHI},
    {0,4,10000,SHUAI},
    {0,5,100,RED_SHI},
    {0,6,50,RED_XIANG},
    {0,7,200,MA},
    {0,8,500,CHE},

    {2,1,300,PAO},
    {2,7,300,PAO},

    {3,0,10,BING},
    {3,2,10,BING},
    {3,4,10,BING},
    {3,6,10,BING},
    {3,8,10,BING},

    {6,0,10,ZU},
    {6,2,10,ZU},
    {6,4,10,ZU},
    {6,6,10,ZU},
    {6,8,10,ZU},

    {7,1,300,PAO},
    {7,7,300,PAO},

    {9,0,500,CHE},
    {9,1,200,MA},
    {9,2,50,BLACK_XIANG},
    {9,3,100,BLACK_SHI},
    {9,4,10000,JIANG},
    {9,5,100,BLACK_SHI},
    {9,6,50,BLACK_XIANG},
    {9,7,200,MA},
    {9,8,500,CHE}

};

Stone::Stone()
{

}
void Stone::init(int id,int d){

    _id = id;
    _dead = false;
    _red = id < 16;
    _row = pos[id].row;
    _col = pos[id].col;
    _x = (_col + 1) * d;
    _y = (_row + 1) * d;
    _type = pos[id].type;
    _value = pos[id].value;

}
QString Stone::getName()
{
    switch(this->_type){
        case JIANG:
            return QString::fromLocal8Bit("将");
        case SHUAI:
            return QString::fromLocal8Bit("帅");
        case RED_SHI:
            return QString::fromLocal8Bit("仕");
        case BLACK_SHI:
            return QString::fromLocal8Bit("士");
        case RED_XIANG:
            return QString::fromLocal8Bit("相");
        case BLACK_XIANG:
            return QString::fromLocal8Bit("象");
        case MA:
            return QString::fromLocal8Bit("马");
        case CHE:
            return QString::fromLocal8Bit("车");
        case PAO:
            return QString::fromLocal8Bit("炮");
        case BING:
            return QString::fromLocal8Bit("兵");
        case ZU:
            return QString::fromLocal8Bit("卒");
        }
}
