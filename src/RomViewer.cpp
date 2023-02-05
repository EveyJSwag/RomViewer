#include "RomViewer.h"

RomViewer::RomViewer(int argc, char* argv[])
{
    QRect mainButtonSize;
    QSize fixedWindowSize;
    fixedWindowSize.setHeight(MAIN_WINDOW_HEIGHT);
    fixedWindowSize.setWidth(MAIN_WINDOW_WIDTH);

    mainApplication = new QApplication(argc, argv);
    mainWindow = new QMainWindow();

    addressTextEdit = new QLineEdit(mainWindow);
    QRect addressTextEditRect = addressTextEdit->geometry();
    addressTextEdit->setGeometry(X_BUFFER,Y_BUFFER,addressTextEditRect.width(),addressTextEditRect.height());
    addressTextEdit->setInputMask(QString("HHHHHHHH"));
    addressTextEdit->setDisabled(true);

    romMemoryBrowser = new QTextBrowser(mainWindow);
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
    searchMemoryButton = new MyButton(searchMemoryString, (QWidget*)mainWindow);
    searchMemoryButton->setDisabled(true);
    QRect searchMemoryButtonGeo = searchMemoryButton->geometry();
    searchMemoryButton->setGeometry(
        X_BUFFER, 
        2*Y_BUFFER + searchMemoryButtonGeo.height(), 
        searchMemoryButtonGeo.width(), 
        searchMemoryButtonGeo.height());

    std::string findRomString = "Find Rom";
    findRomButton = new MyButton(findRomString,(QWidget*)mainWindow);
    QRect findRomButtonRect = findRomButton->geometry();
    findRomButton->setGeometry(
        X_BUFFER,
        MAIN_WINDOW_HEIGHT - Y_BUFFER - findRomButtonRect.height(),
        findRomButtonRect.width(),
        findRomButtonRect.height());

    std::string searchOpCodeString = std::string("Search for Opcode");
    searchOpCodeButton = new MyButton(searchOpCodeString,(QWidget*)mainWindow );
    searchOpCodeButton->setDisabled(true);
    QRect searchOpCodeButtonGeo = searchOpCodeButton->geometry();
    searchOpCodeButton->setGeometry(
        X_BUFFER * 2 + findRomButton->width() * 2 + 50,
        MAIN_WINDOW_HEIGHT - Y_BUFFER - searchOpCodeButton->height(),
        searchOpCodeButton->width() + 50,
        searchOpCodeButton->height());

    opCodeSearchResult = new QTextBrowser(mainWindow);
    opCodeSearchResult->setGeometry(
        X_BUFFER * 2 + findRomButton->width() * 2 + 50,
        Y_BUFFER,
        MAIN_WINDOW_WIDTH/4 - X_BUFFER*2,
        MAIN_WINDOW_HEIGHT/2 + Y_BUFFER * 2);

    opCodeSearchEdit = new QLineEdit(mainWindow);
    opCodeSearchEdit->setGeometry(
        opCodeSearchResult->x(),
        opCodeSearchResult->height() + 2 * Y_BUFFER,
        opCodeSearchEdit->width(),
        opCodeSearchEdit->height());
    opCodeSearchEdit->setInputMask(QString("HHHHHHHH"));
    opCodeSearchEdit->setDisabled(true);

    romBrowserDialog = new QFileDialog(mainWindow);
    romBrowserDialog->setNameFilter(QString("*smc"));

    formatSelector = new QGroupBox(mainWindow);
    formatSelector->setTitle(QString("Format Selector"));

    formatVbox = new QVBoxLayout();
    hexIntChoice = new QRadioButton(formatSelector);
    hexIntChoice->setText(QString("32 Bit Hex"));
    hexShortChoice  = new QRadioButton(formatSelector);
    hexShortChoice->setText(QString("16 Bit Hex"));
    hexCharChoice = new QRadioButton(formatSelector);
    hexCharChoice->setText(QString("8 Bit Hex"));
    charChoice = new QRadioButton(formatSelector);
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

    romDialogInfo = new FileDialogInfo(
        romMemoryBrowser,
        addressTextEdit,
        searchMemoryButton,
        formatSelector,
        searchOpCodeButton, 
        opCodeSearchResult,
        opCodeSearchEdit);

    mainWindow->setFixedSize(fixedWindowSize);

    connectWidgets();
}

void RomViewer::connectWidgets()
{
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
}


void RomViewer::Run()
{
    mainWindow->show();
    mainApplication->exec();
}

RomViewer::~RomViewer()
{
    delete mainApplication;
    delete mainWindow;
}