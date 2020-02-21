#ifndef EXITWINDOW_H
#define EXITWINDOW_H

#include "mainwindow.h"
#include <QCloseEvent>

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool accepted = true;
    for (auto i = document.begin(); i != document.end(); i++)
    {
        // Search for each file unsaved(by Linhk)
        if (document[currentText].textEdit->document()->isModified())
        {
            auto temp = QMessageBox::information(this, "TweeEdit Tip", \
                                                 QString::fromLocal8Bit("At least one file unsaved.\n \
Do you want to exit WITHOUT\n saving the file?"), QMessageBox::Yes | QMessageBox::No);
            if (temp == QMessageBox::Yes)
                event->accept();
            else
                event->ignore(), accepted = false;
            break;
        }
    }
    if (accepted)
        event->accept();
}

#endif // EXITWINDOW_H
