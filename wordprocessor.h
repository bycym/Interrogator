#ifndef WORDPROCESSOR_H
#define WORDPROCESSOR_H
#include <QString>
#include <vector>
#include <QStringList>
#include <QMap>

class wordProcessor
{
public:
    wordProcessor();
    bool checkAnswer(QString string);
    int goodAnswer() const;
    void setGoodAnswer(int goodAnswer);

    int badAnswer() const;
    void setBadAnswer(int badAnswer);

    QString getNewWord();

    int getAskedIndex() const;

    void init();

    int getAllWords() const;

    QStringList getPrevAnswer() const;
    QStringList getAnswer(int arrayIndex) const;

    bool openFile(QString fileName);
    bool saveFile(QString fileName, bool append = false);

    /*!
     * \brief switchLanguage switch language
     */
    void switchLanguage();

    /*!
     * \brief getSwitch witch language going to used
     * \return
     */
    bool getSwitch();

    bool getWrongAnswersOnly(bool append = false);

    int getWrongWordsSize()
    {
        return _wrongWords.size();
    }

private:
    /***
     * QVector<QStringList> matrix{{"foo", "bar", "baz"}, {"hello", "world", "!"}};
     *
     * qDebug() << "output: " << matrix[0];
     * //Will output :  output:  ("foo", "bar", "baz")
     *
     * qDebug() << "output: " << matrix[0][1];
     * //Will output :  "bar"
     *
     **/

    /*!
     * \brief _words contains English - Hungarian words
     */
    std::vector<QStringList> _words;
    std::vector<QStringList> _wrongWords;
    //std::vector<QStringList> _correctWords;
    QStringList _prevAnswer;

    int _goodAnswer;
    int _badAnswer;
    int _askedIndex;
    int _actualIndex;
    int _allWords;

    /*!
     * \brief _switched witch language going to used
     */
    int _switched;

    enum Separate
    {
        TABULATOR = 0,
        OLDWAY,
        COLON,
        EQUAL
    };

    QMap<Separate, QString> _separate;

};

#endif // WORDPROCESSOR_H
