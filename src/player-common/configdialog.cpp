#include "configdialog.h"
#include <QPushButton>
ConfigDialog::ConfigDialog(QWidget *parent, MainConfiguration *Config) :  QDialog(parent), ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    MyConfiguration = Config;
    ui->lineEditPlayerName->setText(MyConfiguration->getPlayerName());
    ui->lineEditContentUrl->setText(MyConfiguration->getIndexUri());
#if !defined  Q_OS_ANDROID
    // cause in Android it shows fullscreen and not as dialog
    setWindowFlags(Qt::WindowStaysOnTopHint);
#endif
    QPushButton *defUrlButton = new QPushButton("Use default CMS",this);
    defUrlButton->setStyleSheet("margin-right:50px;height:100%;min-width:180px;");
    ui->buttonBox->addButton(defUrlButton,QDialogButtonBox::ActionRole);
    connect(defUrlButton,SIGNAL(clicked()),this,SLOT(on_defUrlButton_clicked()));
    ui->labelContentUrl->setText("<html><head/><body><p>Content-URL (You can use "+MyConfiguration->getDefaultURLName()+" "+MyConfiguration->getDefaultURL()+")</p></body></html>");
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

// virtual override
void ConfigDialog::showEvent( QShowEvent* showEvent )
{
    QDialog::showEvent( showEvent);
    activateWindow();
}

void ConfigDialog::accept()
{
    if (MyConfiguration->validateContentUrl(ui->lineEditContentUrl->text()))
    {
        MyConfiguration->setPlayerName(ui->lineEditPlayerName->text());
        MyConfiguration->determineIndexUri(MyConfiguration->getValidatedContentUrl());
        MyConfiguration->determineUserAgent();
        QDialog::accept();
    }
    else
        ui->labelErrorMessage->setText(MyConfiguration->getErrorText());
}

void ConfigDialog::on_defUrlButton_clicked()
{

    ui->lineEditContentUrl->setText(MyConfiguration->getDefaultURL());
}


