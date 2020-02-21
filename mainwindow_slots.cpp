#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QTabWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QDebug>

#include "mainwindow.h"

// private slots:

// basic
void MainWindow::getCurrentPage()
{
    currentText = tabWidget->currentIndex();
    qDebug() << "currentText: " << currentText << "\n";
}

void MainWindow::newDocument(const QString &text, const QString &title, const QString &textPath)
{
    save->setEnabled(true);
    saveAs->setEnabled(true);
    closeTab->setEnabled(true);

    findText->setEnabled(true);

    // default: put the new widget to the back of the queue
    if (totalText == MAX_QPLAINTEXT - 1)
    {
        QMessageBox::critical(this, tr("Error!"), tr("Can\'t create any document now because it\'s full."), QMessageBox::Ok);
        return;
    }
    totalText++;
    countText++;

    document.push_back(Document());

    document.back().textEdit->setPlainText(text);
    document.back().textPath = textPath;
    if (!textPath.isEmpty())
        document.back().bFirstCreate = false;
    if (title == untitledFile)
    {
        QString tit = QString("%1-%2").arg(title).arg(countText);
        tabWidget->addTab(document.back().textEdit, QIcon(":/ico/document.png"), tit);
    }
    else
        tabWidget->addTab(document.back().textEdit, QIcon(":/ico/document.png"), title);
    tabWidget->update();
    tabWidget->setCurrentWidget(document.back().textEdit);

    qDebug() << "Is first create? " << document.back().bFirstCreate << "\n";
    qDebug() << "newDocument called, currentText: " << currentText << "\n";
}

void MainWindow::openDocument()
{
    // opens text
    const QString path = QFileDialog::getOpenFileName(this, tr("Select a documnet"), "", tr("All files (*.*)"));
    qDebug() << "Opened path: " << path << "\n";
    if (path.isEmpty())
        return;

    // reads the text
    QFile file(path);
retry:
    if (!file.open(QIODevice::ReadOnly))
    {
        int res = QMessageBox::critical(this, tr("Error!"), tr("Can\'t open this file."), QMessageBox::Retry, QMessageBox::Ok);
        if (res == QMessageBox::Retry)
            goto retry;
        return;
    }
    QTextStream stream(&file);
    QString text = stream.readAll();

    // gets the name of the text
    QFileInfo info(path);

    // displays the text
    newDocument(text, info.fileName(), path);
}

void MainWindow::findTextDialog()
{
    findDialog = new TweeFindDialog(document[currentText].textEdit, this);
    findDialog->setModal(true);
    findDialog->show();
}

// 'help'
void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::aboutThisApp()
{
    const QString introduce =
    "<h2>TweeEditor</h2> \n\
    A simple text editor made by Qt 5.\n\
    This program is an open-source application.\n\
    Developed by <a href=\"https:\/\/github.com\/TweeChalice\">TweeChalice</a>.\n";
    QMessageBox *messageBox = new QMessageBox;
    messageBox->setIcon(QMessageBox::Icon::Information);
    messageBox->setText(introduce);
    messageBox->setWindowTitle(tr("About TweeEditor"));
    messageBox->setDefaultButton(QMessageBox::Ok);
    messageBox->show();
}

void MainWindow::saveDocument()
{
    qDebug() << "The path of the current file you want to save: " << document[currentText].textPath << "\n";
    qDebug() << "Is first create? " << document[currentText].bFirstCreate << "\n";
    qDebug() << "saveDocument called, currentText: " << currentText << "\n";

    // gets the text of the current file
    const QString text = document[currentText].textEdit->toPlainText();
    qDebug() << "The text of the current file: " << text << "\n";

    // checks if the user is the first time to create this document
    if (document[currentText].bFirstCreate)
    {
        qDebug() << "The file needs SaveAs\n";
        saveAsDocument();
        return;
    }

    // gets the path of the saving file
    const QString path = document[currentText].textPath;
    if (path.isEmpty())
        return;

    // writes the text
    QFile file(path);
retry:
    if (!file.open(QIODevice::WriteOnly))
    {
        int res = QMessageBox::critical(this, tr("Error!"), tr("Can\'t save this file."), QMessageBox::Retry, QMessageBox::Ok);
        if (res == QMessageBox::Retry)
            goto retry;
        return;
    }
    file.write(text.toUtf8());
    QFileInfo info(file);
    tabWidget->setTabText(currentText, info.fileName());

    file.close();

    qDebug() << "Save file successfully\n";
}

void MainWindow::closeDocument()
{
    totalText--;
    tabWidget->removeTab(currentText);

    // cleans up
    document.removeAt(currentText + 1);
    if (totalText == 0)
    {
        save->setEnabled(false);
        saveAs->setEnabled(false);
        closeTab->setEnabled(false);
        findText->setEnabled(false);
    }
}

void MainWindow::closeDocument2(int index)
{
     delete tabWidget->widget(index);
//    QMessageBox::warning(NULL, "TweeEdit", "Sorry, I don't know how to\ndeal with it yet.\n\t--Linhk1606", QMessageBox::Ok);
}

void MainWindow::saveAsDocument()
{
    qDebug() << "SaveAs called\n";
    qDebug() << "The path of the current file you want to save: " << document[currentText].textPath << "\n";
    qDebug() << "Is first create? " << document[currentText].bFirstCreate << "\n";
    qDebug() << "saveDocument called, currentText: " << currentText << "\n";

    // gets the text of the current file
    const QString text = document[currentText].textEdit->toPlainText();
    qDebug() << "The text of the current file: " << text << "\n";

    // gets the path of the saving file
    const QString path = QFileDialog::getSaveFileName(this, tr("Save this text"), "", tr("All files (*.*)"));
    if (path.isEmpty())
        return;

    // writes the text
    QFile file(path);
retry:
    if (!file.open(QIODevice::WriteOnly))
    {
        int res = QMessageBox::critical(this, tr("Error!"), tr("Can\'t save this file."), QMessageBox::Retry, QMessageBox::Ok);
        if (res == QMessageBox::Retry)
            goto retry;
        return;
    }
    file.write(text.toUtf8());

    document.remove(currentText);

    QFileInfo info(file);
    tabWidget->setTabText(currentText, info.fileName());

    file.close();

    qDebug() << "SaveAs successfully\n";
}
