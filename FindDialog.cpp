#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>

#include "FindDialog.h"

TweeFindDialog::TweeFindDialog(QPlainTextEdit *textEdit, QWidget *parent) : QDialog(parent)
{
    currentTextEdit = new QPlainTextEdit;
    cancel = new QPushButton(tr("Cancel"));
    ok = new QPushButton(tr("Find"));
    findText = new QLineEdit;
    findIncaseSensitively = new QRadioButton(tr("&Find word incasesensitively"));
    findWholeWord = new QRadioButton(tr("&Find the whole word"));
    findBackward = new QRadioButton(tr("&Find backward from your cursor position"));
    findCaseSensitively = new QRadioButton(tr("&Find caseSensitively from your cursor position"));
    findForward=new QRadioButton(tr("&Find forward from your cursor position"));

    QGroupBox *findWay = new QGroupBox(tr("The way to find"));
    QVBoxLayout *findWayLayout = new QVBoxLayout;
    findWayLayout->addWidget(findIncaseSensitively);
    findWayLayout->addWidget(findWholeWord);
    findWayLayout->addWidget(findForward);
    findWayLayout->addWidget(findBackward);
    findWayLayout->addWidget(findCaseSensitively);
    findWay->setLayout(findWayLayout);

    findIncaseSensitively->setChecked(true);

    // that's because find forward is default option
    findForward->setChecked(true);

    currentTextEdit = textEdit;

    findText->setPlaceholderText(tr("Find the word"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);

    connect(findIncaseSensitively, SIGNAL(clicked()), this, SLOT(findIncase()));
    connect(findWholeWord, SIGNAL(clicked()), this, SLOT(findWhole()));
    connect(findBackward, SIGNAL(clicked()), this, SLOT(findBack()));
    connect(findCaseSensitively, SIGNAL(clicked()), this, SLOT(findCase()));
    connect(ok, SIGNAL(clicked()), this, SLOT(findEnteredText()));
    connect(cancel, &QPushButton::clicked, this, &QDialog::close);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(findText);
    mainLayout->addWidget(findWay);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);

    this->setWindowIcon(QIcon(":/ico/TweeEditor.jpg"));
    this->setWindowTitle(tr("Find"));
    this->setFixedSize(sizeHint());
}

void TweeFindDialog::findEnteredText()
{
    bool res = currentTextEdit->find(findText->text(),findWay);

    if (!res)
        QMessageBox::critical(this->window(), tr("Error"),
                              tr("There\'s no string named <b>%1</b>").arg(findText->text()),
                              QMessageBox::Ok);
}

// slots

void TweeFindDialog::findIncase()
{
    findWay = FIND_INCASESENTIVILY;
    findIncaseSensitively->setChecked(true);
    findForward->setChecked(true);
}

void TweeFindDialog::findWhole()
{
    findWay = FIND_WHOLEWORD;
    findForward->setChecked(true);
}

void TweeFindDialog::findBack()
{
    findWay = FIND_BACKWARD;
    findForward->setChecked(false);
}

void TweeFindDialog::findCase()
{
    findWay = FIND_CASESENTIVILY;
    findForward->setChecked(true);
}
