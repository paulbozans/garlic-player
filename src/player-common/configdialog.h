#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "tools/main_configuration.h"
#include <QDialog>
namespace Ui
{
    class ConfigDialog;
}

#include "ui_configdialog.h"

class ConfigDialog : public QDialog
{
        Q_OBJECT
    public:
        explicit ConfigDialog(QWidget *parent, MainConfiguration *Config);
        ~ConfigDialog();


    void showEvent(QShowEvent *showEvent);
protected:
        Ui::ConfigDialog *ui;
        MainConfiguration   *MyConfiguration = NULL;

    protected slots:
        void accept();

     private slots:
        void on_defUrlButton_clicked();

};

#endif // CONFIGDIALOG_H
