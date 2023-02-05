#include <iostream>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtCore/QTimer>
#include <QtGui/QPalette>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QFileDialog>

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <iomanip>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <string>

#include "FileDialogInfo.h"
#include "RomReader.h"
#include "myButton.h"

int main(int argc, char *argv[])
{
    const int MAIN_WINDOW_HEIGHT = 500;
    const int MAIN_WINDOW_WIDTH  = 1200;
    const int Y_BUFFER = 10;
    const int X_BUFFER = 10;
    QRect mainButtonSize;
    QSize fixedWindowSize;
    fixedWindowSize.setHeight(MAIN_WINDOW_HEIGHT);
    fixedWindowSize.setWidth(MAIN_WINDOW_WIDTH);

    QApplication* mainApplication = new QApplication(argc, argv);
    QMainWindow* mainWindow = new QMainWindow();

    QLineEdit* addressTextEdit = new QLineEdit(mainWindow);
    QRect addressTextEditRect = addressTextEdit->geometry();
    addressTextEdit->setGeometry(X_BUFFER,Y_BUFFER,addressTextEditRect.width(),addressTextEditRect.height());
    addressTextEdit->setInputMask(QString("HHHHHHHH"));
    addressTextEdit->setDisabled(true);

    QTextBrowser* romMemoryBrowser = new QTextBrowser(mainWindow);
    QFont memoryFont("Monospace");
    memoryFont.setStyleHint(QFont::Monospace);
    romMemoryBrowser->setFont(memoryFont);
    romMemoryBrowser->setDisabled(true);
    QRect romMemoryBrowserRect = romMemoryBrowser->geometry();
    romMemoryBrowser->setGeometry(
        X_BUFFER+MAIN_WINDOW_WIDTH/2,
        Y_BUFFER,
        MAIN_WINDOW_WIDTH/2 - X_BUFFER*2,
        MAIN_WINDOW_HEIGHT - Y_BUFFER*2);

    std::string searchMemoryString = std::string("Search");
    MyButton* searchMemoryButton = new MyButton(searchMemoryString, (QWidget*)mainWindow);
    searchMemoryButton->setDisabled(true);
    QRect searchMemoryButtonGeo = searchMemoryButton->geometry();
    searchMemoryButton->setGeometry(
        X_BUFFER, 
        2*Y_BUFFER + searchMemoryButtonGeo.height(), 
        searchMemoryButtonGeo.width(), 
        searchMemoryButtonGeo.height());

    std::string findRomString = "Find Rom";
    MyButton* findRomButton = new MyButton(findRomString,(QWidget*)mainWindow);
    QRect findRomButtonRect = findRomButton->geometry();
    findRomButton->setGeometry(
        X_BUFFER,
        MAIN_WINDOW_HEIGHT - Y_BUFFER - findRomButtonRect.height(),
        findRomButtonRect.width(),
        findRomButtonRect.height());

    std::string searchOpCodeString = std::string("Search for Opcode");
    MyButton* searchOpCodeButton = new MyButton(searchOpCodeString,(QWidget*)mainWindow );
    searchOpCodeButton->setDisabled(true);
    QRect searchOpCodeButtonGeo = searchOpCodeButton->geometry();
    searchOpCodeButton->setGeometry(
        X_BUFFER * 2 + findRomButton->width() * 2 + 50,
        MAIN_WINDOW_HEIGHT - Y_BUFFER - searchOpCodeButton->height(),
        searchOpCodeButton->width() + 50,
        searchOpCodeButton->height());

    QTextBrowser* opCodeSearchResult = new QTextBrowser(mainWindow);
    opCodeSearchResult->setGeometry(
        X_BUFFER * 2 + findRomButton->width() * 2 + 50,
        Y_BUFFER,
        MAIN_WINDOW_WIDTH/4 - X_BUFFER*2,
        MAIN_WINDOW_HEIGHT/2 + Y_BUFFER * 2);

    QLineEdit* opCodeSearchEdit = new QLineEdit(mainWindow);
    opCodeSearchEdit->setGeometry(
        opCodeSearchResult->x(),
        opCodeSearchResult->height() + 2 * Y_BUFFER,
        opCodeSearchEdit->width(),
        opCodeSearchEdit->height());
    opCodeSearchEdit->setInputMask(QString("HHHHHHHH"));
    opCodeSearchEdit->setDisabled(true);

    QFileDialog* romBrowserDialog = new QFileDialog(mainWindow);
    romBrowserDialog->setNameFilter(QString("*smc"));

    QGroupBox* formatSelector = new QGroupBox(mainWindow);
    formatSelector->setTitle(QString("Format Selector"));

    QVBoxLayout* formatVbox = new QVBoxLayout();
    QRadioButton* hexIntChoice = new QRadioButton(formatSelector);
    hexIntChoice->setText(QString("32 Bit Hex"));
    QRadioButton* hexShortChoice  = new QRadioButton(formatSelector);
    hexShortChoice->setText(QString("16 Bit Hex"));
    QRadioButton* hexCharChoice = new QRadioButton(formatSelector);
    hexCharChoice->setText(QString("8 Bit Hex"));
    QRadioButton* charChoice = new QRadioButton(formatSelector);
    charChoice->setText(QString("Character"));
    formatVbox->addWidget(hexIntChoice);
    formatVbox->addWidget(hexShortChoice);
    formatVbox->addWidget(hexCharChoice);
    formatVbox->addWidget(charChoice);
    QRect formatSelectorGeo = formatSelector->geometry();

    formatSelector->setGeometry(
        X_BUFFER,
        4*Y_BUFFER + searchMemoryButton->geometry().y(),
        formatSelectorGeo.width()*2,
        formatSelectorGeo.height()*5);

    formatSelector->setLayout(formatVbox);

    formatSelector->setDisabled(true);

    FileDialogInfo* romDialogInfo = new FileDialogInfo(
        romMemoryBrowser,
        addressTextEdit,
        searchMemoryButton,
        formatSelector,
        searchOpCodeButton, 
        opCodeSearchResult,
        opCodeSearchEdit);

    QWidget::connect(
        findRomButton,
        &MyButton::clicked,
        romBrowserDialog,
        &QFileDialog::exec);

    QWidget::connect(
        romBrowserDialog, 
        &QFileDialog::fileSelected, 
        romDialogInfo, 
        &FileDialogInfo::setFile);

    QWidget::connect(
        searchMemoryButton, 
        &MyButton::clicked, 
        romDialogInfo, 
        &FileDialogInfo::displayMemoryAtAddress);

    QWidget::connect(
        searchOpCodeButton, 
        &MyButton::clicked, 
        romDialogInfo, 
        &FileDialogInfo::searchBytePattern);

    mainWindow->setFixedSize(fixedWindowSize);
    mainWindow->show();
    mainApplication->exec();

    delete mainWindow;
    delete mainApplication;
}

