#include "achivementdialog.h"
#include "ui_achivementdialog.h"

AchivementDialog::AchivementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AchivementDialog)
{
    ui->setupUi(this);
}

AchivementDialog::~AchivementDialog()
{
    delete ui;
}
