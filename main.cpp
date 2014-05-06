#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include "Cache.h"


enum test1
{
    five1 = 5,
    twenty1 = 20
};

enum test2
{
    five2 = 5,
    twenty2 = 20
};

int main()
{
    int currentAddress = 70;
    int compNum = 37;
    int currentTag = (int)(currentAddress/compNum);
    qDebug() << currentTag;
    /*
    Cache* C1 = new Cache();
    C1->readFile("test");
    C1->iterateInput();
*/
	return 0;
}
