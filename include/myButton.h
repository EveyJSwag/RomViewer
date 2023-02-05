#ifndef MY_BUTTON
#define MY_BUTTON

#include <iostream>
#include <string>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

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

