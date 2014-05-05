#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H


#include <exception>
#include <qtconcurrentexception.h>


class invalidFile : public QtConcurrent::Exception
{
public:
    void raise() const {qDebug() << "Exception: "; throw *this; }
};

invalidFile badFile;

#endif // FILEEXCEPTION_H
