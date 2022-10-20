#include "wordprocessor.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <set>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

wordProcessor::wordProcessor()
{
    _separate.insert(TABULATOR, "\t");
    _separate.insert(OLDWAY, " - ");
    _separate.insert(COLON, ":");
    _separate.insert(EQUAL, "=");

    QString fileName = "words.txt";
    openFile(fileName);
    init();
    _switched = false;
}

bool wordProcessor::checkAnswer(QString string)
{
    bool ret = false;
    int indexX, indexY;

    // first word will be asked (english)
    if(!_switched)
    {
        indexX = _askedIndex;
        indexY = 1; // hungarian
    }
    else
    {
        indexX = _askedIndex;
        indexY = 0; // english
        // second word will be asked (hungarian)

    }


    _prevAnswer.clear();

    QStringList fields = _words[indexX][indexY].split(",");
    string.replace(" ","");
    qDebug() << string;

    for(int i = 0; i < fields.size(); ++i)
    {
        QString str = fields[i];
        _prevAnswer << str;
        qDebug() << str;
        str.replace(" ", "");
        ret = (QString::compare(string,str, Qt::CaseInsensitive) == 0);
        if(ret)
        {
            break;
        }
    }

    qDebug() << ((ret == true)?"true":"false");
    qDebug() << "--------------";

    if(ret && _actualIndex <= _words.size())
    {
        ++_goodAnswer;
    }
    else if(_actualIndex <= _words.size())
    {
        ++_badAnswer;
        _wrongWords.push_back(_words[indexX]);
    }

    return ret;
}

int wordProcessor::goodAnswer() const
{
    return _goodAnswer;
}

void wordProcessor::setGoodAnswer(int goodAnswer)
{
    _goodAnswer = goodAnswer;
}

int wordProcessor::badAnswer() const
{
    return _badAnswer;
}

void wordProcessor::setBadAnswer(int badAnswer)
{
    _badAnswer = badAnswer;
}

QString wordProcessor::getNewWord()
{

    QString word = "no words";
    int indexX, indexY;
    // first word will be asked (english)
    if(!_switched)
    {
        indexX = _actualIndex;
        indexY = 0;
    }
    else
    {
        // second word will be asked (hungarian)
        indexX = _actualIndex;
        indexY = 1;
    }

    // english
    if(_actualIndex < _words.size())
    {
        _askedIndex = _actualIndex;
        ++_actualIndex;
        word = _words[indexX][indexY];
    }
    else if(0 >= _words.size())
    {
        QMessageBox mb(QMessageBox::Critical,
                       "ERROR",
                       "There is no word!");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
    }


    return word;
}

int wordProcessor::getAskedIndex() const
{
    return _actualIndex;
    /*
    if(_askedIndex <= 0)
    {
        return 0;
    }
    else if(_askedIndex >= _words.size())
    {
        return _words.size();
    }
    return _askedIndex;*/
}

void wordProcessor::init()
{
    _actualIndex = 0;
    _badAnswer = 0;
    _goodAnswer = 0;
    _allWords = _words.size();
    _askedIndex = 0;
    std::srand(time(0));
    std::random_shuffle(_words.begin(),_words.end());
    _wrongWords.clear();
}

int wordProcessor::getAllWords() const
{
    return _allWords;
}

QStringList wordProcessor::getPrevAnswer() const
{
    return _prevAnswer;
}

QStringList wordProcessor::getAnswer(int arrayIndex) const
{
    QStringList words;
    words << "no answer";
    int indexX, indexY;

    if(!(arrayIndex >= _words.size()) && (arrayIndex >= 0))
    {
        words.clear();
        // answer (hungarian)
        if(!_switched)
        {
            indexX = arrayIndex;
            indexY = 1;
        }
        else
        {
            // if hungarian asked then the answer in english
            indexX = arrayIndex;
            indexY = 0;
        }
        words = _words[indexX][indexY].split(",");
    }
    else
    {
        QMessageBox mb(QMessageBox::Critical,
                    "ERROR",
                    "There is no word!");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
    }

    return words;
}

bool wordProcessor::openFile(QString fileName)
{
    bool ret = false;
    _words.clear();
    /*QString path = QCoreApplication::applicationDirPath(); Qt::current
    path.append("/words.txt");*/
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "no file?", file.errorString());

        QString line = "no words\tnincs szó";
        QStringList fields = line.split("\t");
        //model->appendRow(fields);
        _words.push_back(fields);
        ret = false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(_separate[EQUAL]);
        if(2 <= fields.size())
        {
            _words.push_back(fields);
        }
        else
        {
            qDebug() << "Hibás szó:";
            qDebug() << fields;
        }
        //model->appendRow(fields);
    }
    qDebug() << "words size: " << _words.size();
    if(_words.size() > 0)
    {
        ret = true;
    }
    file.close();

    return ret;
}

bool wordProcessor::saveFile(QString fileName, bool append)
{
    bool ret = false;

    QFile file(fileName);

    if(append)
    {
        file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
    else
    {
        file.open(QIODevice::WriteOnly | QIODevice::Text);
    }

    if(!file.isOpen()){
        qDebug() << "- Error, unable to open" << "outputFilename" << "for output";
        return false;
    }

    //QDataStream outStream(&file);
    QTextStream outStream(&file);
    /*
    for(QStringList sl : _wrongWords)
    {
        for(QString s : sl)
        {
            outStream << s;
        }
    }*/

    for(int i = 0; i < _wrongWords.size(); ++i)
    {
        qDebug() << "export line: " << i;
        outStream << _wrongWords[i][0];
        outStream << _separate[TABULATOR];
        outStream << _wrongWords[i][1];
        outStream << "\n";
    }

    file.close();

    return true;
}

bool wordProcessor::getSwitch()
{
    return _switched;
}

bool wordProcessor::getWrongAnswersOnly(bool append)
{
    bool ret = false;

    qDebug() << "wrong words size: " << _wrongWords.size();
    if(0 >= _wrongWords.size())
    {
        QMessageBox mb(QMessageBox::Critical,
                       "No word",
                       "There is no wrong word!\nNoice!");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setIcon(QMessageBox::Information);
        mb.exec();
    }
    else
    {
        ret = true;
        if(!append)
        {
            _words.clear();
        }
        for(QStringList s : _wrongWords)
        {
            _words.push_back(s);
        }

        /*std::set<QStringList> s( _words.begin(), _words.end() );
        _words.assign( s.begin(), s.end() );*/
/*
        std::set<QStringList> s;
        unsigned size = _words.size();
        for( unsigned i = 0; i < size; ++i ) s.insert( _words[i] );
        _words.assign( s.begin(), s.end() );*/

    }
    return ret;
}

void wordProcessor::switchLanguage()
{
    _switched = !_switched;
    qDebug() << "--------------";
    qDebug() << ((_switched == true)?"true":"false");
    qDebug() << "--------------";
}

