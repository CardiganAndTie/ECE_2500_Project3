#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include "Cache.h"

/*
enum INSTRUCTION_TYPE{
    read_I,
    write_I
};

enum CACHE_SIZE{
    c_1K=1024,
    c_4K=4096,
    c_64K=65536,
    c_128K=131072
};

enum BLOCK_SIZE{
    B_8b=8,
    B_16b=16,
    B_32b=32,
    B_128b=128
};

enum MAPPING_TYPE{
    DM,
    two_way,
    four_way,
    FA
};

enum WRITE_POLICY{
    WB,
    WT
};*/

int main()
{
    //Perpare vectors of each type of configuration
    //Create each vector
    QVector<CACHE_SIZE> cacheVector;
    QVector<BLOCK_SIZE> blockVector;
    QVector<MAPPING_TYPE> mappingVector;
    QVector<WRITE_POLICY> writeVector;

    //append the necessary elements
    cacheVector.append(c_1K);
    cacheVector.append(c_4K);
    cacheVector.append(c_64K);
    cacheVector.append(c_128K);

    blockVector.append(B_8b);
    blockVector.append(B_16b);
    blockVector.append(B_32b);
    blockVector.append(B_128b);

    mappingVector.append(DM);
    mappingVector.append(two_way);
    mappingVector.append(four_way);
    mappingVector.append(FA);

    writeVector.append(WB);
    writeVector.append(WT);

    QString everything;

    int loopcount = 0;
    for(int cInd = 0; cInd < cacheVector.size(); cInd++)
    {
        for(int bInd = 0; bInd < blockVector.size(); bInd++)
        {
            for(int mInd = 0; mInd < mappingVector.size(); mInd++)
            {
                for(int wInd = 0; wInd < writeVector.size(); wInd++)
                {
                    Cache* C1 = new Cache(cacheVector.at(cInd), blockVector.at(bInd), mappingVector.at(mInd), writeVector.at(wInd));

                    qDebug() << C1->getCacheSize() << "\t"
                             << C1->getBlockSize() << "\t"
                             << C1->getMappingType() << "\t"
                             << C1->getWritePolicy();

                    C1->readFile("test1");
                    C1->simulate();

                    QString output;
                    output.append(QString::number(C1->getCacheSize()));
                    output.append("\t");
                    output.append(QString::number(C1->getBlockSize()));
                    output.append("\t");
                    output.append(C1->getMappingType());
                    output.append("\t");
                    output.append(C1->getWritePolicy());
                    output.append("\t");
                    output.append(QString::number(((float)C1->getHits()/(float)C1->getInstructionNum()),'f',2));
                    output.append("\t");
                    output.append(QString::number(C1->getM2C()));
                    output.append("\t");
                    output.append(QString::number(C1->getC2M()));
                    output.append("\t");
                    output.append(QString::number(C1->getCompNum()));
                    output.append("\n");

                    everything.append(output);

                    delete C1;
                    qDebug() << "--------------------------------finished loop" << loopcount;
                    loopcount++;
                }
            }
        }
    }

    qDebug() << everything;

    QFile output("output.dat");

    if(!output.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qFatal("Error: could not open output file");
    }

    QTextStream stream(&output);

    stream << everything;

/*
    Cache* C1 = new Cache(c_1K, B_8b, two_way, WB);
    qDebug() << "cache constructed";
    C1->readFile("test1");
    qDebug() << "read done";


    C1->simulate();
    QString output;
    output.append(QString::number(C1->getCacheSize()));
    output.append("\t");
    output.append(QString::number(C1->getBlockSize()));
    output.append("\t");
    output.append(C1->getMappingType());
    output.append("\t");
    output.append(C1->getWritePolicy());
    output.append("\t");
    output.append(QString::number(((float)C1->getHits()/(float)C1->getInstructionNum()),'f',2));
    output.append("\t");
    output.append(QString::number(C1->getM2C()));
    output.append("\t");
    output.append(QString::number(C1->getC2M()));
    output.append("\t");
    output.append(QString::number(C1->getCompNum()));
    output.append("\n");

    qDebug() << output;

    /*
    qDebug() << C1->getCacheSize() << "\t"
             << C1->getBlockSize() << "\t"
             << C1->getMappingType() << "\t"
             << C1->getWritePolicy() << "\t"
             << QString::number(((float)C1->getHits()/(float)C1->getInstructionNum()),'f',2) << "\t"
             << C1->getM2C() << "\t"
             << C1->getC2M() << "\t"
                << C1->getCompNum();
    qDebug() << "There are " << C1->getInstructionNum() << " instructions and " << C1->getHits() << " hits.";
*/

    //C1->iterateInput();

    //delete C1;
    /*
    Cache* C1 = new Cache();
    C1->readFile("test");
    C1->iterateInput();
*/
	return 0;
}
