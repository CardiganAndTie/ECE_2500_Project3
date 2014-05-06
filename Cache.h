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
    BLOCK_SIZE blockS;
    MAPPING_TYPE mapping;
    WRITE_POLICY writeP;
    QList<INSTRUCTION_TYPE> RWList;
    QList<int> addressList;

    QList< QVector<int>* >* cacheData;
    QList< QVector<int>* >* ageData;    //keep track of how recently used a block is; smaller the number, the older the data


    int blockNum;       //keeps track of how many blocks total there are
    int waySize;        //keeps track of how many blocks are in a way

    int dirtyBits;      //keeps track of how many dirty bits there are
    int hitNum;         //keeps track of how many hits there are
    int M2C;            //keeps track of how many bytes of transfer from memory to cache there are
    int C2M;            //keeps track of how many bytes of transfer from cache to memory there are
    int compNum;        //keeps track of number of ways
    int age;            //keeps track of how new the current instruction is

    //private functions
    bool findTagAt(int tagToFind, int setToCheck);
    bool findTag();
    void init();        //initializes the cacheData QList
    bool isNull(int wayToCheck, int setToCheck);
    int findOldest(int setToCheck);
    int at(int wayToCheck, int setToCheck);
    void replace(int wayToCheck, int setToCheck, int newVal);

public:
    Cache(CACHE_SIZE newCache = c_1K, BLOCK_SIZE newBlock = B_8b, MAPPING_TYPE newMap = DM, WRITE_POLICY newWrite = WB);

    void readFile(QString fileName);
    void iterateInput();

    void setCacheSize(CACHE_SIZE newCache);
    void setBlockSize(BLOCK_SIZE newBlock);
    void setMappingType(MAPPING_TYPE newMap);
    void setWritePolicy(WRITE_POLICY newWrite);

    CACHE_SIZE getCacheSize();
    BLOCK_SIZE getBlockSize();
    MAPPING_TYPE getMappingType();
    WRITE_POLICY getWritePolicy();


    void simulate();
};

///////////////////////////////
//PRIVATE HELPER FUNCTIONS
//////////////////////////////
//iterates through the ways and checks a par
bool Cache::findTagAt(int tagToFind, int setToCheck)
{
    for(int i = 0; i < cacheData->size(); i++)
    {
        if (cacheData->at(i)->at(setToCheck) == tagToFind) return true; //
    }
    return false;
}

bool Cache::findTag()
{
}

void Cache::init()
{
    cacheData = new QList< QVector<int>* >; //Create the database of QVectors (ways) that will store tags
    blockNum = cacheS/blockS;

    switch(mapping){
    case DM:
        compNum = 1;
        break;
    case two_way:
        compNum = 2;
        break;
    case four_way:
        compNum = 4;
        break;
    case FA:
        compNum = blockNum;
        break;
    default:
        compNum = 0;
    }


    waySize = blockNum/compNum;
    for(int i = 0; i < compNum; i++)
    {
        QVector<int>* temp = new QVector<int>(waySize);
        cacheData->append(temp);

        QVector<int>* ageTemp = new QVector<int>(waySize);
        ageTemp->fill(0);
        cacheData->append(temp);
    }
}

bool Cache::isNull(int wayToCheck, int setToCheck)
{
    if(this->at(wayToCheck,setToCheck) == NULL) return true;
    else return false;
}

int Cache::findOldest(int setToCheck)
{
    int oldestIndex = 0;
    for(int i = 0; i < compNum; i++)
    {
        if(ageData->at(i)->at(setToCheck) < ageData->at(oldestIndex)->at(setToCheck)) //if current interation is older
            oldestIndex = i;
    }
    return oldestIndex;
}

int Cache::at(int wayToCheck, int setToCheck)
{
    return cacheData->at(wayToCheck)->at(setToCheck);
}

void Cache::replace(int wayToCheck, int setToCheck, int newVal)
{
    cacheData->at(wayToCheck)->replace( setToCheck, newVal);
}

///////////////////////////////
//PUBLIC MEMBER FUNCTIONS
//////////////////////////////
Cache::Cache(CACHE_SIZE newCache, BLOCK_SIZE newBlock, MAPPING_TYPE newMap, WRITE_POLICY newWrite)
{
    this->cacheS = newCache;
    this->blockS = newBlock;
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

//for debugging purposes
void Cache::iterateInput()
{
    for(int i = 0; i < addressList.size(); i++)
    {
        qDebug() << RWList.at(i) << "\t" << addressList.at(i);
    }
}


void Cache::setCacheSize(CACHE_SIZE newCache)
{
    this->cacheS = newCache;
}

void Cache::setBlockSize(BLOCK_SIZE newBlock)
{
    this->blockS = newBlock;
}

void Cache::setMappingType(MAPPING_TYPE newMap)
{
    this->mapping = newMap;
}

void Cache::setWritePolicy(WRITE_POLICY newWrite)
{
    this->writeP = newWrite;
}

CACHE_SIZE Cache::getCacheSize()
{
    return cacheS;
}

BLOCK_SIZE Cache::getBlockSize()
{
    return blockS;
}

MAPPING_TYPE Cache::getMappingType()
{
    return mapping;
}

WRITE_POLICY Cache::getWritePolicy()
{
    return writeP;
}




void Cache::simulate()
{
    this->init();
    dirtyBits = 0;      //keeps track of how many dirty bits there are
    hitNum = 0;         //keeps track of how many hits there are
    M2C = 0;            //keeps track of how many bytes of transfer from memory to cache there are
    C2M = 0;            //keeps track of how many bytes of transfer from cache to memory there are
    age = 0;            //keeps track of when instruction was used

    for(int i = 0; i < RWList.size(); i++)  //iterate through the instructions
    {
        int currentAddress = addressList.at(i);
        int currentTag = (int)(currentAddress/compNum);
        //look for a place to put tag
        for(int wayInd = 0; wayInd < cacheData->size(); wayInd++)
        {
            bool placementFound = false;
            //iterate through the set
            for(int setInd = 0; setInd < cacheData->at(wayInd)->size(); setInd++)
            {
                if(this->isNull(wayInd,setInd))
                {
                    this->replace(wayInd,setInd,currentTag);
                    placementFound = true;
                }
            }
            if(!placementFound)
            {

            }
        }
    }
}

#endif // CACHE_H
