#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui
{
    class AboutDialog;
}

namespace MC
{
    class AboutDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit AboutDialog(QWidget *parent = 0);
        ~AboutDialog();

    private:
        Ui::AboutDialog *ui;
    };
} // namespace MC
#endif // ABOUTDIALOG_H
