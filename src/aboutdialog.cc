#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "utils.h"

namespace MC
{
    AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AboutDialog)
    {
        ui->setupUi(this);
        ui->label_version->setText("Version " + VERSION);

        connect(ui->pushButton_close, SIGNAL(pressed()), this, SLOT(close()));
    }

    AboutDialog::~AboutDialog()
    {
        delete ui;
    }
} // namespace MC
