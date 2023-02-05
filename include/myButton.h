#ifndef MY_BUTTON
#define MY_BUTTON


#include <iostream>
#include <string>
#include "QtIncludes.h"

class MyButton : public QPushButton
{

public:
    MyButton(
        std::string& buttonText, 
        QWidget* parent);

private:
    MyButton();
    QString myButtonName;
    void onClick();
    
};

#endif

