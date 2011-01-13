#include "stegodialog.h"
#include "ui_stegodialog.h"

StegoDialog::StegoDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::StegoDialog)
{
    ui->setupUi(this);
}

StegoDialog::~StegoDialog()
{
    delete ui;
}
