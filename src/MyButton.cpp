#include "MyButton.h"

void idk()
{
    std::cout << "HELLO" << std::endl;
}

MyButton::MyButton(
    std::string& buttonText,
    QWidget* parent) :
        QPushButton(parent)
{
    myButtonName = QString::fromStdString(buttonText);
    setText(myButtonName);
}

void MyButton::onClick()
{
    setGeometry(100,100,100,100);
}
