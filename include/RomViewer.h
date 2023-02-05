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

class RomViewer
{
public:
    RomViewer(int argc, char * argv[]);
    ~RomViewer();
    void Run();
private:
    const int MAIN_WINDOW_HEIGHT = 500;
    const int MAIN_WINDOW_WIDTH  = 1200;
    const int Y_BUFFER = 10;
    const int X_BUFFER = 10;

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

    void connectWidgets();

};
#endif