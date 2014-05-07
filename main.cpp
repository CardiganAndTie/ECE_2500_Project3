#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include "Cache.h"

int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        qFatal("Error: incorrent number of arguments");
    }
    QString inFileName = argv[1];

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

    QString everything; //will hold all the data in QString format

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

                    C1->readFile(inFileName);
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
                    loopcount++;
                }
            }
        }
    }


    //output to a file
    QString outFileName = argv[1];
    outFileName.append(".result");
    QFile output(outFileName);

    if(!output.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qFatal("Error: could not open output file");
    }

    QTextStream stream(&output);

    stream << everything;


	return 0;
}
