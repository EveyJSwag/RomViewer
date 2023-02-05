#ifndef FILE_DIALOG_INFO
#define FILE_DIALOG_INFO

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
#include "RomReader.h"
#include "myButton.h"
#include <vector>
#include <string>


typedef struct FileDialogInfo : public QObject
{
public:
    const int INDEX_OFFSET = 0x200;
    const int NAME_ADDRESS_OFFSET = 0x7FC0;
    const int SIZE_OF_NAME = 21;

    unsigned char typeRomDataBuffer[1049088];

    std::string bytesToSearch;
    std::vector<std::string> opcodeMatchStrings;

    QTextBrowser* textBrowser;
    QLineEdit* lineEdit;
    MyButton* searchButton;
    QGroupBox* groupBox;
    MyButton* opCodeSearch;
    QTextBrowser* searchResBrowse;
    QLineEdit*  searchParamEdit;

    FileDialogInfo(){}
    FileDialogInfo(
        QTextBrowser* textBrowserRef,
        QLineEdit* lineEditRef,
        MyButton* searchButtonRef,
        QGroupBox* groupBoxRef,
        MyButton* opCodeSearchRef,
        QTextBrowser* searchResBrowseRef,
        QLineEdit*  searchParamEditRef);
    std::string selectedFile;

    typedef enum FormatEnum{ 
        int_hex_format = 0,
        short_hex_format,
        char_hex_format,
        character_format
    } FormatEnum;

    void setFile(const QString& file);
    void dumpCurrentBuffer();
    void displayMemoryAtAddress();
    void searchBytePattern();

    void* romDataBuffer;

} FileDialogInfo;

#endif
