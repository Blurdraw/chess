#ifndef STONE_H
#define STONE_H
#include <qstring.h>
enum TYPE {SHUAI,JIANG,RED_XIANG,BLACK_XIANG,RED_SHI,BLACK_SHI,MA,CHE,PAO,BING,ZU};
class Stone
{
public:
    Stone();

    int _row;
    int _col;
    int _id;
    int _x;
    int _y;
    int _value;
    bool _dead;
    bool _red;
    TYPE _type;

    void init(int,int);
    QString getName();
};

#endif // STONE_H
