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
    Cache* C1 = new Cache(c_1K, B_8b, four_way, WT);
    qDebug() << "cache constructed";
    C1->readFile("test");
    qDebug() << "read done";


    C1->simulate();
    qDebug() << C1->getCacheSize() << "\t"
             << C1->getBlockSize() << "\t"
             << C1->getMappingType() << "\t"
             << C1->getWritePolicy() << "\t"
             << QString::number((C1->getHits()/C1->getInstructionNum()),'f',2) << "\t"
             << C1->getM2C() << "\t"
             << C1->getC2M() << "\t"
                << C1->getCompNum();
    qDebug() << "There are " << C1->getInstructionNum() << " instructions and " << C1->getHits() << " hits.";


    //C1->iterateInput();

    delete C1;
    /*
    Cache* C1 = new Cache();
    C1->readFile("test");
    C1->iterateInput();
*/
	return 0;
}
