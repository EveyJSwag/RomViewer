#include "FileDialogInfo.h"

FileDialogInfo::FileDialogInfo(
    QTextBrowser* textBrowserRef,
    QLineEdit* lineEditRef,
    MyButton* searchButtonRef,
    QGroupBox* groupBoxRef,
    MyButton* opCodeSearchRef,
    QTextBrowser* searchResBrowseRef,
    QLineEdit*  searchParamEditRef)
{
    textBrowser = textBrowserRef;
    lineEdit = lineEditRef;
    searchButton = searchButtonRef;
    groupBox = groupBoxRef;
    opCodeSearch = opCodeSearchRef;
    searchResBrowse = searchResBrowseRef;
    searchParamEdit = searchParamEditRef;
}

void FileDialogInfo::setFile(const QString& file)
{
    selectedFile = file.toStdString();
    romDataBuffer = (char*)malloc(1049088);
    getRomData(selectedFile,1049088,romDataBuffer);
    textBrowser->setEnabled(true);
    lineEdit->setEnabled(true);
    searchButton->setEnabled(true);
    groupBox->setEnabled(true);
    opCodeSearch->setEnabled(true);
    searchResBrowse->setEnabled(true);
    searchParamEdit->setEnabled(true);
}

void FileDialogInfo::dumpCurrentBuffer()
{
    int dumpTarget = open("dumpedrom.smc", O_CREAT | O_RDWR);
    int rc = write(dumpTarget,typeRomDataBuffer,1049088);
    close(dumpTarget);
}

void FileDialogInfo::displayMemoryAtAddress()
{
    FormatEnum formatToUse = FormatEnum::character_format;
    QObjectList formatChildren = groupBox->children();
    for (int i = 0; i < formatChildren.size() - 1; i++)
    {
        QRadioButton* qrbRef = (QRadioButton*)formatChildren[i];
        if (qrbRef->isChecked())
        {
            formatToUse = (FormatEnum)i;
            break;
        }
    }

    QString lineText = lineEdit->text();
    std::string lineString = lineText.toStdString();
    int addressToView = std::stoi(lineString,0,16);
    std::string stringToDisplay;
    std::stringstream displayStream;
    switch(formatToUse)
    {
        case FormatEnum::int_hex_format:
            for (int i = (addressToView + INDEX_OFFSET)/4; i < (addressToView + INDEX_OFFSET)/4 + 32; i++)
            {
                int* intArray = (int*)romDataBuffer;
                displayStream << std::hex << intArray[i] << " ";
            }
            break;
        case FormatEnum::short_hex_format:
            for (int i = (addressToView + INDEX_OFFSET)/2; i < (addressToView + INDEX_OFFSET)/2 + 32; i++)
            {
                short* shortArray = (short*)romDataBuffer;
                displayStream << std::hex << shortArray[i] << " ";
            }
            break;
        case FormatEnum::char_hex_format:
            for (int i = addressToView + INDEX_OFFSET; i < addressToView + INDEX_OFFSET + 128; i++)
            {
                unsigned int currentElement = ((unsigned char*)romDataBuffer)[i];
                displayStream << std::hex << std::setw(2) << std::setfill('0') << currentElement << " ";
            }
            break;
        case FormatEnum::character_format:
            for (int i = addressToView + INDEX_OFFSET; i < addressToView + INDEX_OFFSET + 32; i++)
            {
                char currentElement = ((char*)romDataBuffer)[i];
                displayStream << currentElement << " ";
            }
        break;
    }
    bool saveString = false;
    
    stringToDisplay = displayStream.str();
    textBrowser->setText(QString(stringToDisplay.c_str()));
    textBrowser->update();
}

void FileDialogInfo::searchBytePattern()
{
    opcodeMatchStrings.clear();
    QString lineText = searchParamEdit->text();
    std::string lineString = lineText.toStdString();
    int addressToView = std::stoi(lineString,0,16);
    unsigned char* searchBytes = (unsigned char*)malloc(lineString.size()/2);
    int searchByteIndex = 0;
    for (int i = 0; i < lineString.size(); i+=2)
    {
        searchBytes[searchByteIndex] = (unsigned char)std::stoi(lineString.substr(i,2),0,16);
        searchByteIndex+=1;
    }

    int numBytes = lineString.size() / 2;
    int numOfConsecutiveMatches = 0;
    unsigned char* searchByteStartAddr = searchBytes;
    for (int i = 0; i < 1049088; i++)
    {
        unsigned char currentByte = ((unsigned char*) romDataBuffer)[i];
        if ( currentByte == *searchBytes)
        {
            numOfConsecutiveMatches++;
            searchBytes++;
        }
        else
        {
            numOfConsecutiveMatches = 0;
            searchBytes = searchByteStartAddr;
        }
        if (numOfConsecutiveMatches == numBytes)
        {
            std::stringstream entryStream;
            entryStream << "0x" << std::hex << (i - numBytes + 1) - 0x200;
            opcodeMatchStrings.push_back(entryStream.str());
            numOfConsecutiveMatches = 0;
            searchBytes = searchByteStartAddr;
        }
    }

    searchResBrowse->clear();
    searchResBrowse->setText("");
    for (std::vector<std::string>::iterator firstMatch = opcodeMatchStrings.begin(); 
         firstMatch != opcodeMatchStrings.end(); 
         firstMatch++)
    {
        std::string browseString = *firstMatch;
        searchResBrowse->append(QString(browseString.c_str()));
    }
}
