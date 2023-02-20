#ifndef FILE_DIALOG_INFO
#define FILE_DIALOG_INFO

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <string>

#include "QtIncludes.h"
#include "RomReader.h"
#include "MyButton.h"
#include "RomViewer.h"

class RomViewer;

typedef struct FileDialogInfo : public QObject
{
public:
    const int INDEX_OFFSET = 0x200;
    const int NAME_ADDRESS_OFFSET = 0x7FC0;
    const int SIZE_OF_NAME = 21;

    unsigned char typeRomDataBuffer[1049088];

    std::string bytesToSearch;
    std::vector<std::string> opcodeMatchStrings;

    RomViewer* romViewer;

    QTextBrowser* textBrowser;
    QLineEdit* lineEdit;
    MyButton* searchButton;
    QGroupBox* groupBox;
    MyButton* opCodeSearch;
    QTextBrowser* searchResBrowse;
    QLineEdit*  searchParamEdit;

    FileDialogInfo(){}

    FileDialogInfo(RomViewer* romViewerRef);
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
