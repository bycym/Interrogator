#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QVector>
#include <QMessageBox>
#include "achivementdialog.h"
#define __VERZ__ "0.01"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _fileName("words.txt")
{
    ui->setupUi(this);
    wp = new wordProcessor();

    QString title("Interrogator - v" + QString(__VERZ__) + " - " + _fileName);
    this->setWindowTitle(title);

    ui->wordListWidget->insertItem(0,wp->getNewWord());
    ui->goodLabel->setText(QString::number(wp->goodAnswer()));
    ui->badLabel->setText(QString::number(wp->badAnswer()));
    ui->allLabel->setText(QString::number(wp->getAskedIndex())+"/"+QString::number(wp->getAllWords()));
    ui->answerLineEdit->setFocus();
    _askedIndex.push_back(wp->getAskedIndex());
}

MainWindow::~MainWindow()
{
    delete wp;
    delete ui;
}

void MainWindow::on_answerLineEdit_returnPressed()
{
    addElementToList();
    ui->answerLineEdit->clear();
}

void MainWindow::on_okButton_clicked()
{
    addElementToList();
    ui->answerLineEdit->clear();
    ui->answerLineEdit->setFocus();
}

void MainWindow::addElementToList()//(QString word)
{
    bool ret;
    ret = wp->checkAnswer(ui->answerLineEdit->text());
    //ui->wordListWidget->addItem();
    if(ret)
    {
        ui->wordListWidget->item(0)->setForeground(Qt::green);
    }
    else
    {
        ui->wordListWidget->item(0)->setForeground(Qt::red);
    }
    ui->possibleAnswerListWidget->clear();
    ui->possibleAnswerListWidget->addItems(wp->getPrevAnswer());

    qDebug() << "asked: " << wp->getAskedIndex() << " all words: " << wp->getAllWords();
    if(wp->getAskedIndex() < wp->getAllWords())
    {
        ui->wordListWidget->insertItem(0,wp->getNewWord());
    }

    ui->goodLabel->setText(QString::number(wp->goodAnswer()));
    ui->badLabel->setText(QString::number(wp->badAnswer()));
    ui->allLabel->setText(QString::number(wp->getAskedIndex())+"/"+QString::number(wp->getAllWords()));
    _askedIndex.push_back(wp->getAskedIndex());
}

void MainWindow::newWords()
{
    _askedIndex.clear();
    wp->init();
    ui->wordListWidget->clear();
    ui->wordListWidget->insertItem(0,wp->getNewWord());
    ui->goodLabel->setText(QString::number(wp->goodAnswer()));
    ui->badLabel->setText(QString::number(wp->badAnswer()));
    ui->allLabel->setText(QString::number(wp->getAskedIndex())+"/"+QString::number(wp->getAllWords()));
    ui->answerLineEdit->setFocus();
    ui->possibleAnswerListWidget->clear();
    _askedIndex.push_back(wp->getAskedIndex());
}

void MainWindow::on_newButton_clicked()
{
    newWords();
}

void MainWindow::on_actionOpen_file_2_triggered()
{
    QString path = QDir::homePath();
    bool ret = false;

    if(_fileName != ""){
        QFileInfo info(_fileName);
        path = info.absolutePath();
    }
    QString fn = QFileDialog::getOpenFileName(
            this,
            "Open file...",
            path,
            "Szövegfájlok(*.txt, *.md);;Minden fájl(*)"
            );
    QStringList thingsToDo;
    qDebug() << fn;
    if(!fn.isEmpty())
    {
        ret = wp->openFile(fn);

    }
    qDebug() << "ret: " << ret;
    if(ret)
    {
        _fileName = fn;

        wp->init();
        _askedIndex.clear();
        ui->wordListWidget->clear();
        ui->wordListWidget->insertItem(0,wp->getNewWord());
        ui->goodLabel->setText(QString::number(wp->goodAnswer()));
        ui->badLabel->setText(QString::number(wp->badAnswer()));
        ui->allLabel->setText(QString::number(wp->getAskedIndex())+"/"+QString::number(wp->getAllWords()));
        ui->answerLineEdit->setFocus();
        ui->possibleAnswerListWidget->clear();

        _askedIndex.push_back(wp->getAskedIndex());

        QStringList tmp = _fileName.split( "/" );
        QString openedFileName = tmp.value( tmp.length()-1);
        this->setWindowTitle("Interrogator - v" + QString(__VERZ__) + " - " + openedFileName);
    }
}

void MainWindow::on_switchButton_clicked()
{
    wp->init();  
    wp->switchLanguage();
    _askedIndex.clear();
    ui->wordListWidget->clear();
    ui->wordListWidget->insertItem(0,wp->getNewWord());
    ui->goodLabel->setText(QString::number(wp->goodAnswer()));
    ui->badLabel->setText(QString::number(wp->badAnswer()));
    ui->allLabel->setText(QString::number(wp->getAskedIndex())+"/"+QString::number(wp->getAllWords()));
    ui->answerLineEdit->setFocus();
    ui->possibleAnswerListWidget->clear();
    _askedIndex.push_back(wp->getAskedIndex());
}

void MainWindow::on_wordListWidget_itemClicked(QListWidgetItem *item)
{
    int currentRow = ui->wordListWidget->currentIndex().row();
    qDebug() << "---------------";
    qDebug() << "current row: " << ui->wordListWidget->currentIndex().row();
    qDebug() << "all words: " << wp->getAskedIndex();
    qDebug() << "asked index size: " << _askedIndex.size()-1;
    int indexAsked = _askedIndex.size() - ui->wordListWidget->currentIndex().row()-1;
    qDebug() << "asked index: " << indexAsked;
    if(indexAsked >= 0 && _askedIndex.size() >= indexAsked && currentRow != 0)
    {
        qDebug() << "answer: " << _askedIndex[indexAsked];
        ui->possibleAnswerListWidget->clear();
        ui->possibleAnswerListWidget->addItems(wp->getAnswer(indexAsked));
    }
}

void MainWindow::on_actionAsk_wrongs_answers_only_triggered()
{
    bool ret = false;

    ret = wp->getWrongAnswersOnly();
    if(ret)
    {
        newWords();
    }
    this->statusBar()->showMessage("there is " + QString(wp->getWrongWordsSize()) + " wrong word(s).");
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox mb(QMessageBox::Information,
                   "About",
                   "by Arcsibo\nhttps://github.com/arcsibo/Interrogator");
    mb.setStandardButtons(QMessageBox::Ok);
    mb.setIcon(QMessageBox::Information);
    mb.exec();
}

void MainWindow::on_actionExport_wrong_answers_triggered()
{
    QString path = QDir::homePath();
    bool ret = false;

    if(_fileName != ""){
        QFileInfo info(_fileName);
        path = info.absolutePath();
    }

    QString fn = QFileDialog::getSaveFileName(
            this,
            "Save file...",
            path,
            "Szövegfájlok(*.txt);;Minden fájl(*)"
            );
    QStringList thingsToDo;
    qDebug() << fn;
    if(!fn.isEmpty())
    {
        ret = wp->saveFile(fn,false);

    }
    qDebug() << "ret: " << ret;

}

void MainWindow::on_actionExport_wrong_answers_append_triggered()
{
    QString path = QDir::homePath();
    bool ret = false;

    if(_fileName != ""){
        QFileInfo info(_fileName);
        path = info.absolutePath();
    }

    QString fn = QFileDialog::getSaveFileName(
            this,
            "Save file...",
            path,
            "Szövegfájlok(*.txt);;Minden fájl(*)"
            );
    QStringList thingsToDo;
    qDebug() << fn;
    if(!fn.isEmpty())
    {
        ret = wp->saveFile(fn,true);

    }
    qDebug() << "ret: " << ret;
}

void MainWindow::on_actionAk_wrong_answers_only_append_triggered()
{
    bool ret = false;
    this->statusBar()->showMessage("there is " + QString(wp->getWrongWordsSize()) + " wrong word(s).");
    ret = wp->getWrongAnswersOnly(true);
    if(ret)
    {
        // TODO! new name "push wrong answers"
        // TODO! newWords has a init, modify it!
        newWords();
    }

}

void MainWindow::on_actionAchivements_triggered()
{
    AchivementDialog ad(this);
    ad.exec();
}
