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
    QList<long> addressList;

    QList< QVector<long int>* >* cacheData;
    QList< QVector<int>* >* ageData;    //keep track of how recently used a block is; smaller the number, the older the data
    QList< QVector<bool>* >* dirtyBits; //keeps track of dirty bits for each block


    int blockNum;       //keeps track of how many blocks total there are
    int waySize;        //keeps track of how many blocks are in a way; also the number of sets there are


    int hitNum;         //keeps track of how many hits there are
    int M2C;            //keeps track of how many bytes of transfer from memory to cache there are
    int C2M;            //keeps track of how many bytes of transfer from cache to memory there are
    int compNum;        //keeps track of number of ways
    int age;            //keeps track of how new the current instruction is
    int instructionNum;

    //private functions
    int findTag(long int tagToFind, int setToCheck);
    void init();        //initializes the cacheData QList
    bool isEmpty(int wayToCheck, int setToCheck);
    int findEmptyWay(int setToCheck);
    int countDirtyBits();

    //Access functions
    int findOldest(int setToCheck) const;
    long at(int wayToCheck, int setToCheck) const;
    bool DBAt(int wayToCheck, int setToCheck) const;



    //modifier functions
    void replace(int wayToCheck, int setToCheck, long int newVal);
    void setAge(int wayToCheck, int setToCheck, int newAge);
    void setDB(int wayToCheck, int setToCheck, bool newDB);




public:
    Cache(CACHE_SIZE newCache = c_1K, BLOCK_SIZE newBlock = B_8b, MAPPING_TYPE newMap = DM, WRITE_POLICY newWrite = WB);

    //Access functions
    int getM2C() const;
    int getC2M() const;
    int getCompNum() const;
    int getHits() const;
    int getInstructionNum() const;

    void readFile(QString fileName);
    void iterateInput();

    void setCacheSize(CACHE_SIZE newCache);
    void setBlockSize(BLOCK_SIZE newBlock);
    void setMappingType(MAPPING_TYPE newMap);
    void setWritePolicy(WRITE_POLICY newWrite);

    CACHE_SIZE getCacheSize();
    BLOCK_SIZE getBlockSize();
    QString getMappingType();
    QString getWritePolicy();


    void simulate();
};

///////////////////////////////
//PRIVATE HELPER FUNCTIONS
//////////////////////////////
//iterates through the ways and checks a particular set to see if a tag matches
//if it does, it returns the way's index, otherwise it returns -1
int Cache::findTag(long tagToFind, int setToCheck)
{
    for(int i = 0; i < cacheData->size(); i++)
    {
        if (cacheData->at(i)->at(setToCheck) == tagToFind)
        {
            return i;
        }
    }
    return -1;
}

void Cache::init()
{
    cacheData = new QList< QVector<long>* >; //Create the database of QVectors (ways) that will store tags
    ageData = new QList< QVector<int>* >;    //Create the database of ages for each block in the cacheData
    dirtyBits = new QList< QVector<bool>* >; //create the database of dirtybits for each block in cacheData
    this->blockNum = cacheS/blockS;
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


    this->waySize = blockNum/compNum;


    for(int i = 0; i < compNum; i++)
    {
        QVector<long>* temp = new QVector<long>(waySize);
        temp->fill(-1); //fills it with -1, indicating empty
        cacheData->append(temp);


        QVector<int>* ageTemp = new QVector<int>(waySize);
        ageTemp->fill(0);
        ageData->append(ageTemp);


        QVector<bool>* dirtyTemp = new QVector<bool>(waySize);
        dirtyTemp->fill(false);         //initialize everything as false
        dirtyBits->append(dirtyTemp);



    }

}

bool Cache::isEmpty(int wayToCheck, int setToCheck)
{
    if(this->at(wayToCheck,setToCheck) == -1) return true;
    else return false;
}

//Finds an empty way given a set and returns that way's index
//Otherwise returns -1
int Cache::findEmptyWay(int setToCheck)
{
    for(int wayInd = 0; wayInd < cacheData->size(); wayInd++)
    {

        if(this->isEmpty(wayInd,setToCheck)) //Check if this is empty
        {
            return wayInd;

        }
    }
    return -1;
}

int Cache::countDirtyBits()
{
    int total = 0;
    for(int wayInd = 0; wayInd < dirtyBits->size(); wayInd++)
    {
        for(int setInd = 0; setInd < dirtyBits->at(wayInd)->size(); setInd++)
        {
            if(DBAt(wayInd,setInd)) total++;
        }
    }
    return total;
}



//Returns the oldest way in a set
int Cache::findOldest(int setToCheck) const
{
    int oldestIndex = 0;
    for(int i = 0; i < compNum; i++)
    {
        if(ageData->at(i)->at(setToCheck) < ageData->at(oldestIndex)->at(setToCheck)) //if current interation is older
            oldestIndex = i;
    }
    return oldestIndex;
}

long Cache::at(int wayToCheck, int setToCheck) const
{
    return cacheData->at(wayToCheck)->at(setToCheck);
}

bool Cache::DBAt(int wayToCheck, int setToCheck) const
{
    return dirtyBits->at(wayToCheck)->at(setToCheck);

}





void Cache::replace(int wayToCheck, int setToCheck, long newVal)
{
    cacheData->at(wayToCheck)->replace( setToCheck, newVal);
}

void Cache::setAge(int wayToCheck, int setToCheck, int newAge)
{
    ageData->at(wayToCheck)->replace( setToCheck, newAge);
}

void Cache::setDB(int wayToCheck, int setToCheck, bool newDB)
{
    dirtyBits->at(wayToCheck)->replace( setToCheck, newDB);
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

int Cache::getM2C() const
{
    return M2C;
}
int Cache::getC2M() const
{
    return C2M;
}
int Cache::getCompNum() const
{
    return compNum;
}
int Cache::getHits() const
{
    return hitNum;
}
int Cache::getInstructionNum() const
{
    return instructionNum;
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

    //bool isTab = false;

    while(!stream.atEnd())
    {
        QString readLine = stream.readLine();
        QStringList lineData;
        if(readLine.contains("\t"))
            lineData = readLine.split("\t");
        else
            lineData = readLine.split(" ");
        if(lineData.at(0).contains("re"))
            RWList.append(read_I);
        else if(lineData.at(0).contains("wr")) RWList.append(write_I);
        long tempAddress = lineData.at(1).trimmed().remove(0,2).toLong(0,16); //remove the newline character and convert to int
        addressList.append(tempAddress);
    }
    instructionNum = RWList.size();



}

//for debugging purposes
void Cache::iterateInput()
{
    for(int i = 0; i < addressList.size(); i++)
    {
        QString insType;
        if(RWList.at(i) == read_I)
        {
            insType = "read";
        }
        else if(RWList.at(i) == write_I)
        {
            insType = "write";
        }
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

QString Cache::getMappingType()
{
    QString cacheType;
    switch(mapping){
    case DM:
        cacheType = "DM";
        break;
    case two_way:
        cacheType = "2W";
        break;
    case four_way:
        cacheType = "4W";
        break;
    case FA:
        cacheType = "FA";
        break;
    default:
        cacheType = "none";
    }
    return cacheType;
}

QString Cache::getWritePolicy()
{
    QString writeType;
    if(writeP == WB) writeType = "WB";
    else writeType = "WT";
    return writeType;
}



//Does a simulation on a set of instructions
void Cache::simulate()
{
    this->init();
    hitNum = 0;         //keeps track of how many hits there are
    M2C = 0;            //keeps track of how many bytes of transfer from memory to cache there are
    C2M = 0;            //keeps track of how many bytes of transfer from cache to memory there are
    age = 0;            //keeps track of when instruction was used
    //FDB

    for(int insInd = 0; insInd < RWList.size(); insInd++)  //iterate through the instructions
    {
        long currentAddress = addressList.at(insInd);
        long currentTag = (long)(currentAddress/blockS/waySize);
        int currentSet = (int)((currentAddress/blockS)%waySize);
        int currentWay;     //goal is to find this


        currentWay = findTag(currentTag, currentSet);
        //ON A READ
        if(RWList.at(insInd) == read_I)
        {
            //////////////////////////////////////////////////////
            //ON A HIT
            if(currentWay != -1)   //if a hit
            {
                hitNum++;
            }
            else  //ON A MISS
            {
                //look for a place to put tag
                ////////////////////
                //EMPTY BLOCK FOUND
                if((currentWay = this->findEmptyWay(currentSet)) != -1)
                {
                    this->replace(currentWay,currentSet,currentTag);    //if so put it in there
                }
                else    //NO EMPTY BLOCKS
                {

                    currentWay = findOldest(currentSet);                    //Find the least recently use
                    this->replace(currentWay, currentSet, currentTag);      //replace the least recently used
                    if(this->DBAt(currentWay,currentSet))                   //if there is a dirty bit
                    {
                        C2M += blockS;                                      //cache to memory transfer due to writeback
                        this->setDB(currentWay,currentSet,false);           //set dirty bit to false
                    }

                }

                M2C += blockS;  //memory to cache transfer
            }

        }
        else if(RWList.at(insInd) == write_I)   //ON A WRITE
        {
            /////////////////////////////////////////////////
            //ON A WRITEBACK
            if(writeP == WB)
            {

                ///////////////////////////////////////////
                //ON A HIT
                if(currentWay != -1)   //if a hit
                {
                    hitNum++;
                }
                //////////////////////////////////
                //ON A MISS
                else
                {
                    /////////////////////
                    //EMPTY BLOCK FOUND
                    if((currentWay = this->findEmptyWay(currentSet)) != -1)
                    {
                        this->replace(currentWay,currentSet,currentTag);    //if so put it in there
                    }
                    else     //NO EMPTY BLOCKS
                    {
                        currentWay = findOldest(currentSet);                    //Find the least recently used
                        this->replace(currentWay, currentSet, currentTag);      //replace the least recently used
                        if(this->DBAt(currentWay,currentSet))                   //if there is a dirty bit
                        {
                            C2M += blockS;                                          //cache to memory transfer due to writeback
                        }


                    }
                    M2C += blockS;  //memory to cache transfer
                }
                this->setDB(currentWay,currentSet,true);                //set dirty bit to true
            }
            else if(writeP == WT) //ON A WRITETHROUGH
            {
                //currentWay = findTag(currentTag, currentSet);
                ////////////////////////////////////////
                //ON A HIT
                if(currentWay != -1)
                {
                    hitNum++;
                }
                else    //ON A MISS
                {
                    //////////////////////
                    //EMPTY BLOCK FOUND
                    if((currentWay = this->findEmptyWay(currentSet)) != -1)
                    {
                        this->replace(currentWay,currentSet,currentTag);        //put tag in empty block
                    }
                    else    //NO EMPTY BLOCKS
                    {
                        currentWay = findOldest(currentSet);                    //Find the least recently used
                        this->replace(currentWay, currentSet, currentTag);      //replace the least recently used
                    }

                    M2C += blockS;    //memory to cache transfer bceause miss
                }
                C2M += 4;      //cache to memory tranfer due to immediate write from writethrough
            }

        }

        if(currentWay == -1) qFatal("Error: -1 index");
        this->setAge(currentWay,currentSet,age);    //set the age
        age++;                                      //increment age
    }
    //FDB
    if(writeP == WB) C2M += (countDirtyBits() * blockS);    //evict the rest of the cache and update C2M
}

#endif // CACHE_H
