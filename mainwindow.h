#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QVector>
#include "FindDialog.h"

constexpr int MAX_QPLAINTEXT = 11;

// the information and the widget of the text document
struct Document
{
    QPlainTextEdit *textEdit;
    bool bFirstCreate = true; // creates the document for the first time
    QString textPath;

    Document(QPlainTextEdit *widget = new QPlainTextEdit, const QString path = "", const bool first = true) : textEdit(widget), textPath(path), bFirstCreate(first) {}
};

class QMenuBar;
class QToolBar;
class QAction;
class QStatusBar;
class QTabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void createActions();     // creates the basic actions
    void createMenuBar();     // creates the menu
    void createStatuBar();    // creates the state bar
    void createTextEdit();    // creates the text editor window
    void createContextMenu(); // creates context menu

private slots:

    // 'file' slots
    void getCurrentPage();                                                                                           // gets current QPlainText
    void newDocument(const QString &text = "", const QString &title = "untitled.txt", const QString &textPath = ""); // displays a new text documnet
    void openDocument();                                                                                             // opens a text documnet
    void saveDocument();                                                                                             // saves the text document
    void saveAsDocument();                                                                                           // the dialog of 'Save As'
    void closeDocument();                                                                                            // closes current tab

    // 'edit' slots
    void findTextDialog();

    // 'help' slots
    void aboutQt();
    void aboutThisApp();

private:
    QMenuBar *mainMenu;
    QToolBar *toolBar;
    QStatusBar *statuBar;

    QTabWidget *tabWidget;
    QVector<Document> document;

    // 'file' actions
    QAction *newText;
    QAction *open;
    QAction *save;
    QAction *saveAs;
    QAction *closeTab;
    QAction *exitApp;

    // 'edit' actions
    QAction *findText;

    // 'help' actions
    QAction *aboutQtAction;
    QAction *aboutThisAppAction;

    TweeFindDialog* findDialog;

    int currentText = 0;
    int totalText = 0; // records the number of QPlainText created
};
#endif // MAINWINDOW_H
