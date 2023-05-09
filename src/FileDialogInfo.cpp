#include <sys/stat.h>

#include "FECharacter.h"
#include "FEClass.h"
#include "FileDialogInfo.h"

FileDialogInfo::FileDialogInfo(RomViewer* romViewerRef)
{
    romViewer = romViewerRef;
}

void FileDialogInfo::setFile(const QString& file)
{
    selectedFile = file.toStdString();
    getFileSize(selectedFile);
    romDataBuffer = (char*)malloc(romFileSize);
    memset(romDataBuffer, 0, romFileSize);
    getRomData(selectedFile,romFileSize,romDataBuffer);
    romViewer->enableWidgets();
}

void FileDialogInfo::dumpCurrentBuffer()
{
    int dumpTarget = open("dumpedrom.smc", O_CREAT | O_RDWR);
    int rc = write(dumpTarget,typeRomDataBuffer,1049088);
    close(dumpTarget);
}

void FileDialogInfo::getFileSize(std::string romFile)
{
    FILE* filePointer;
    filePointer = fopen(romFile.c_str(), "r");

    fseek(filePointer, 0L, SEEK_END);
    romFileSize = ftell(filePointer);

    fclose(filePointer);
}

void FileDialogInfo::writeToMemoryAtAddress()
{
    unsigned int addressToWriteAt = std::stoi(romViewer->getTargetAddressText().toStdString(), 0,16);
    unsigned long uIntToWrite      = std::stol(romViewer->getTargetContentText().toStdString(), 0,16);
    memcpy((unsigned char*)romDataBuffer+addressToWriteAt, &uIntToWrite, 4);

    remove("roms/modified.gba");
    int dumpTarget = open("roms/modified.gba", O_CREAT | O_RDWR | O_APPEND);
    mode_t modifiedMode = 0666;
    int rc = write(dumpTarget, romDataBuffer, romFileSize);
    chmod("roms/modified.gba", modifiedMode);
    int filler = 0;
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
    unsigned long long addressToView = std::stoull(lineString,0,16);
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
            //for (int i = addressToView + INDEX_OFFSET; i < addressToView + INDEX_OFFSET + 128; i++)
            for (unsigned long long i = addressToView; i < addressToView + 256; i++)
            {
                unsigned int currentElement = ((unsigned char*)romDataBuffer)[i];
                displayStream << std::hex << std::setw(2) << std::setfill('0') << currentElement << " ";
            }
            //romViewer->displayMemoryAddressLabels(128/24 + 1);
            break;
        case FormatEnum::character_format:
            //for (int i = addressToView + INDEX_OFFSET; i < addressToView + INDEX_OFFSET + 32; i++)
            for (int i = addressToView; i < addressToView + 256; i++)
            {
                char currentElement = ((char*)romDataBuffer)[i];
                displayStream << currentElement << " ";
            }
        break;
    }

    unsigned int baseClassAddr = 0x8071b8;
    unsigned char characterData[FECharacter::CHARACTER_SIZE];
    unsigned char classData[FEClass::CLASS_SIZE];
    memcpy(
        characterData, 
        (unsigned char*)romDataBuffer + 0x803d64, 
        FECharacter::CHARACTER_SIZE);

    memcpy(
        classData, 
        (unsigned char*)romDataBuffer + baseClassAddr + FEClass::CLASS_SIZE, 
        FEClass::CLASS_SIZE);
  
    FECharacter erika = FECharacter(characterData);
    FEClass lord = FEClass(classData);

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
    long addressToView = std::stol(lineString,0,16);
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
    for (unsigned long i = 0; i < romFileSize; i++)
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
            //entryStream << "0x" << std::hex << (i - numBytes + 1) - 0x200;
            entryStream << "0x" << std::hex << (i - numBytes + 1);
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
