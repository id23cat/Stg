#ifndef STEGODIALOG_H
#define STEGODIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
    class StegoDialog;
}

class StegoDialog : public QDialog
{
    Q_OBJECT

public:
    StegoDialog(QWidget *parent = 0);
    ~StegoDialog();

private:
    Ui::StegoDialog *ui;
};

#endif // STEGODIALOG_H
