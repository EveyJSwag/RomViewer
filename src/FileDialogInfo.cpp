#include "FileDialogInfo.h"

FileDialogInfo::FileDialogInfo(RomViewer* romViewerRef)
{
    romViewer = romViewerRef;
}

void FileDialogInfo::setFile(const QString& file)
{
    selectedFile = file.toStdString();
    romDataBuffer = (char*)malloc(1049088);
    getRomData(selectedFile,1049088,romDataBuffer);
    romViewer->enableWidgets();
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
    QObjectList formatChildren = romViewer->getFormatSelectorChildren();
    for (int i = 0; i < formatChildren.size() - 1; i++)
    {
        QRadioButton* qrbRef = (QRadioButton*)formatChildren[i];
        if (qrbRef->isChecked())
        {
            formatToUse = (FormatEnum)i;
            break;
        }
    }

    QString lineText = romViewer->getAddressText();
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
            romViewer->displayMemoryAddressLabels(128/24 + 1);
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
    QString qStringToSet = QString(stringToDisplay.c_str());
    romViewer->setRomMemoryBrowserText(qStringToSet);
    romViewer->updateRomMemoryBrowser();
}

void FileDialogInfo::searchBytePattern()
{
    opcodeMatchStrings.clear();
    QString lineText = romViewer->getSearchParamText();
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

    romViewer->clearOpCodeSearchResult();
    romViewer->setOpCodeSearchResultText("");
    for (std::vector<std::string>::iterator firstMatch = opcodeMatchStrings.begin(); 
         firstMatch != opcodeMatchStrings.end(); 
         firstMatch++)
    {
        std::string browseString = *firstMatch;
        romViewer->appendOpCodeSearchResultText(QString(browseString.c_str()));
    }
}
