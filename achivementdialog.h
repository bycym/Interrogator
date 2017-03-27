#ifndef ACHIVEMENTDIALOG_H
#define ACHIVEMENTDIALOG_H

#include <QDialog>

namespace Ui {
class AchivementDialog;
}

class AchivementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AchivementDialog(QWidget *parent = 0);
    ~AchivementDialog();

private:
    Ui::AchivementDialog *ui;
};

#endif // ACHIVEMENTDIALOG_H
