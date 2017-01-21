#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wordprocessor.h"
#include <memory>
#include <QListWidget>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_answerLineEdit_returnPressed();

    void on_okButton_clicked();

    void on_newButton_clicked();

    void on_actionOpen_file_2_triggered();

    void on_switchButton_clicked();

    void on_wordListWidget_itemClicked(QListWidgetItem *item);

    void on_actionAsk_wrongs_answers_only_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    void addElementToList();//(QString word);
   // std::unique_ptr<wordProcessor> wp;
    wordProcessor * wp;
    QVector<int> _askedIndex;

    QString _fileName;

    void newWords();
};

#endif // MAINWINDOW_H
