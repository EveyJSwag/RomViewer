#ifndef ROM_VIEWER_H
#define ROM_VIEWER_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <string>

#include "QtIncludes.h"
#include "MyButton.h"
#include "FileDialogInfo.h"

typedef struct FileDialogInfo FileDialogInfo;

class RomViewer
{
public:
    RomViewer(int argc, char * argv[]);
    ~RomViewer();
    void Run();
    void enableWidgets();

    void setRomMemoryBrowserText(QString& textToSet);
    void updateRomMemoryBrowser();

    QString getAddressText();

    QObjectList getFormatSelectorChildren();

    QString getSearchParamText();

    void clearOpCodeSearchResult();
    void setOpCodeSearchResultText(std::string textToSet);
    void appendOpCodeSearchResultText(QString textToSet);

    void displayMemoryAddressLabels(int numRows);

private:
    const int MAIN_WINDOW_HEIGHT = 500;
    const int MAIN_WINDOW_WIDTH  = 1200;
    const int Y_BUFFER = 10;
    const int X_BUFFER = 10;

    const int ROW_WIDTH = 24;

    QApplication* mainApplication;
    QMainWindow* mainWindow;

    QLineEdit* addressTextEdit;

    QTextBrowser* romMemoryBrowser;

    MyButton* findRomButton;

    MyButton* searchOpCodeButton;

    QTextBrowser* opCodeSearchResult;

    QLineEdit* opCodeSearchEdit;

    QFileDialog* romBrowserDialog;

    QGroupBox* formatSelector;

    MyButton* searchMemoryButton;

    QVBoxLayout* formatVbox;
    QRadioButton* hexIntChoice;
    QRadioButton* hexShortChoice;
    QRadioButton* hexCharChoice;
    QRadioButton* charChoice;

    FileDialogInfo* romDialogInfo;

    std::vector<QLabel*> memoryAddressLabels;

    void initializeWidgets();

    void connectWidgets();


};
#endif