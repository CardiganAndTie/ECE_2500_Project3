#ifndef CACHE_H
#define CACHE_H

#include <QDebug>
#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QStringList>

#include "FileException.h"

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
};

class Cache{
private:
    CACHE_SIZE cacheS;
    BLOCK_SIZE block;
    MAPPING_TYPE mapping;
    WRITE_POLICY writeP;
    QList<INSTRUCTION_TYPE> RWList;
    QList<int> addressList;

    int hitNum;
    int M2C;
    int C2M;
    int compNum;

public:
    Cache(CACHE_SIZE newCache = c_1K, BLOCK_SIZE newBlock = B_8b, MAPPING_TYPE newMap = DM, WRITE_POLICY newWrite = WB);

    void readFile(QString fileName);
    void iterateInput();

    void init();
    void simulate();
};


///////////////////////////////
//PUBLIC MEMBER FUNCTIONS
//////////////////////////////
Cache::Cache(CACHE_SIZE newCache, BLOCK_SIZE newBlock, MAPPING_TYPE newMap, WRITE_POLICY newWrite)
{
    this->cacheS = newCache;
    this->block = newBlock;
    this->mapping = newMap;
    this->writeP = newWrite;
}

void Cache::readFile(QString fileName)
{
    fileName.append(".trace");
    QFile input(fileName);
    try
    {
        if(!input.open(QIODevice::ReadOnly))
        {
            badFile.raise();
        }
    }
    catch(invalidFile &badFile)
    {
        qFatal("Could not open file.");
    }

    QTextStream stream(&input);

    while(!stream.atEnd())
    {
        QString readLine = stream.readLine();
        QStringList lineData = readLine.split("\t");
        if(lineData.at(0) == "read")
            RWList.append(read_I);
        else RWList.append(write_I);
        int tempAddress = lineData.at(1).trimmed().remove(0,2).toInt(0,16); //remove the newline character and convert to int
        addressList.append(tempAddress);
    }

}

void Cache::iterateInput()
{
    for(int i = 0; i < addressList.size(); i++)
    {
        //int x = addressList.at(i);
        //QString xAsHex = QString("%1").arg(x,0,16);
        qDebug() << RWList.at(i) << "\t" << addressList.at(i);
    }
}

void Cache::init()
{

}

void Cache::simulate()
{

}

#endif // CACHE_H
