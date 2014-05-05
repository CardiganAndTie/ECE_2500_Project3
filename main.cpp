#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include "Cache.h"



int main()
{

    Cache* C1 = new Cache();
    C1->readFile("test");
    C1->iterateInput();

	return 0;
}
