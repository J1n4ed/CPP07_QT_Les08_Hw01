#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "database.h"

namespace Ui
{
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:

    QString get_string();
    void sendData(QMap<fieldsForConnect, QString> dbData);

    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:

    void on_buttonBox_accepted();

signals:

    void signal_sendData(QMap<fieldsForConnect, QString> dbData);

private:

    Ui::Dialog *ui;
    QMap<fieldsForConnect, QString> data;
};

#endif // DIALOG_H
