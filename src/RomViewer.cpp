#include "RomViewer.h"
#include "ColorUtils.h"
#include <sstream>

RomViewer::RomViewer(int argc, char* argv[])
{
    mainApplication = new QApplication(argc, argv);
    initializeWidgets();
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

    QWidget::connect(
        writeToMemoryButton,
        &MyButton::clicked,
        romDialogInfo,
        &FileDialogInfo::writeToMemoryAtAddress);
}

void RomViewer::initializeWidgets()
{
    QRect mainButtonSize;
    QSize fixedWindowSize;

    //colorBitmap->
    fixedWindowSize.setHeight(MAIN_WINDOW_HEIGHT);
    fixedWindowSize.setWidth(MAIN_WINDOW_WIDTH);
    
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
        X_BUFFER * 2 + findRomButton->width() * 2 + 20,
        MAIN_WINDOW_HEIGHT - Y_BUFFER - searchOpCodeButton->height(),
        searchOpCodeButton->width() + 30,
        searchOpCodeButton->height());

    int osw = 100;
    int osh = 30;

    opCodeSearchResult = new QTextBrowser(mainWindow);
    opCodeSearchResult->setGeometry(
        X_BUFFER * 2 + findRomButton->width() * 2 + 20,
        MAIN_WINDOW_HEIGHT - Y_BUFFER - (MAIN_WINDOW_HEIGHT/2 + Y_BUFFER * 3 + osh *2),
        MAIN_WINDOW_WIDTH/4 - X_BUFFER*2,
        MAIN_WINDOW_HEIGHT/2 + Y_BUFFER * 2);

    opCodeSearchEdit = new QLineEdit(mainWindow);
    opCodeSearchEdit->setGeometry(
        opCodeSearchResult->x(),
        MAIN_WINDOW_HEIGHT - (searchOpCodeButton->height() * 2) - Y_BUFFER,
        opCodeSearchEdit->width(),
        opCodeSearchEdit->height());
    opCodeSearchEdit->setInputMask(QString("HHHHHHHH"));
    opCodeSearchEdit->setDisabled(true);


    contentAddressEdit = new QLineEdit(mainWindow);
    contentAddressEdit->setGeometry(
        opCodeSearchResult->x(),
        Y_BUFFER,
        contentAddressEdit->width(),
        contentAddressEdit->height());
    contentAddressEdit->setInputMask(QString("HHHHHHHH"));
    contentAddressEdit->setDisabled(true);

    memoryContentEdit = new QLineEdit(mainWindow);
    memoryContentEdit->setGeometry(
        opCodeSearchResult->x(),
        Y_BUFFER + contentAddressEdit->height(),
        memoryContentEdit->width(),
        memoryContentEdit->height());
    memoryContentEdit->setInputMask(QString("HHHHHHHH"));
    memoryContentEdit->setDisabled(true);

    std::string writeToMemoryString = std::string("Write to Memory");
    writeToMemoryButton = new MyButton(writeToMemoryString, (QWidget*)mainWindow);
    writeToMemoryButton->setGeometry(
        opCodeSearchResult->x(),
        Y_BUFFER + contentAddressEdit->height()*2,
        writeToMemoryButton->width() + 10,
        writeToMemoryButton->height());
    writeToMemoryButton->setDisabled(true);

    QStringList fileNameFilters;
    fileNameFilters << "*.smc" << "*.gba";
    romBrowserDialog = new QFileDialog(mainWindow);
    romBrowserDialog->setNameFilters(fileNameFilters);

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
    mainWindow->setFixedSize(fixedWindowSize);

    formatSelector->setGeometry(
        X_BUFFER,
        4*Y_BUFFER + searchMemoryButton->geometry().y(),
        formatSelectorGeo.width()*2,
        formatSelectorGeo.height()*5);

    formatSelector->setLayout(formatVbox);

    formatSelector->setDisabled(true);
    romDialogInfo = new FileDialogInfo(this);


    QSize bitmapSize = QSize(64,64);
    unsigned char dataBitArray[64*64*2];
    memset(dataBitArray,0,64*64*2);
    int dataBitIndex = 0;
    const unsigned char COLOR_MASK = 0x1F;
    const unsigned char R_SHIFT = 10;
    const unsigned char G_SHIFT = 5;
    const unsigned char B_SHIFT = 0;
    unsigned short colorShort = 0x11ff;
    unsigned char greenPixel = ColorUtils::getShortGreenPixel(colorShort);
    unsigned char redPixel   = ColorUtils::getShortRedPixel(colorShort);
    unsigned char bluePixel  = ColorUtils::getShortBluePixel(colorShort);

    for (int i = 0; i < 64; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            dataBitArray[dataBitIndex] = ColorUtils::makeShortPixelLow(redPixel, bluePixel, greenPixel);
            dataBitIndex++;
            dataBitArray[dataBitIndex] = ColorUtils::makeShortPixelHigh(redPixel, bluePixel, greenPixel);
            dataBitIndex++;
        }
    }
    QImage pixelImage = QImage(dataBitArray, 64,64,QImage::Format_RGB555);
    QPixmap convertedImage = QPixmap::fromImage(pixelImage);
    //Bitmap colorBitmap;
    //QImage test;
    //colorBitmap = QBitmap::fromData(bitmapSize, dataBitArray, QImage::Format_RGB888);


    QLabel* imageLabel = new QLabel((QWidget*)mainWindow);
    imageLabel->setPixmap(convertedImage);
    imageLabel->setGeometry(
        20, 
        20, 
        imageLabel->geometry().width()*2, 
        imageLabel->geometry().height()*2);


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

void RomViewer::enableWidgets()
{
    romMemoryBrowser->setEnabled(true);
    addressTextEdit->setEnabled(true);
    searchMemoryButton->setEnabled(true);
    formatSelector->setEnabled(true);
    searchOpCodeButton->setEnabled(true);
    opCodeSearchResult->setEnabled(true);
    opCodeSearchEdit->setEnabled(true);
    writeToMemoryButton->setEnabled(true);
    contentAddressEdit->setEnabled(true);
    memoryContentEdit->setEnabled(true);
}

void RomViewer::setRomMemoryBrowserText(QString& textToSet)
{
    romMemoryBrowser->setText(textToSet);
}
void RomViewer::updateRomMemoryBrowser()
{
    romMemoryBrowser->update();
}

QString RomViewer::getAddressText()
{
    return addressTextEdit->text();
}
QObjectList RomViewer::getFormatSelectorChildren()
{
    return formatSelector->children();
}

QString RomViewer::getSearchParamText()
{
    return opCodeSearchEdit->text();
}

void RomViewer::clearOpCodeSearchResult()
{
    opCodeSearchResult->clear();
}

void RomViewer::setOpCodeSearchResultText(std::string textToSet)
{
    opCodeSearchResult->setText(textToSet.c_str());
}

void RomViewer::appendOpCodeSearchResultText(QString textToSet)
{
    opCodeSearchResult->append(textToSet);
}

QString RomViewer::getTargetAddressText()
{
    return contentAddressEdit->text();
}

QString RomViewer::getTargetContentText()
{
    return memoryContentEdit->text();
}

void RomViewer::displayMemoryAddressLabels(int numRows)
{
    QRect locationOfMemAddrWidget;
    int yLocation = 0;
    QFont memoryFont("Monospace");
    memoryFont.setStyleHint(QFont::Monospace);
    for (int i = 0; i < numRows; i++)
    {
        QLabel* rowLabel = new QLabel(mainWindow);
        std::stringstream addrHexStream;
        addrHexStream << std::hex << std::setw(4) << ROW_WIDTH * (i);
        rowLabel->setText(QString(addrHexStream.str().c_str()));

        rowLabel->setGeometry(
            romMemoryBrowser->x() - 40,
            romMemoryBrowser->y() + yLocation - 6,
            rowLabel->width(),
            rowLabel->height());

        rowLabel->setFont(memoryFont);

        rowLabel->setEnabled(true);
        rowLabel->show();
        yLocation += 18;

        memoryAddressLabels.push_back(rowLabel);
    }
}
