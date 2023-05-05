#ifndef FECLASS_H
#define FECLASS_H
class FEClass
{
public:
    static const int CLASS_SIZE = 84;
    static const unsigned int CLASS_ADDR = 0x8071b8; 

    FEClass(unsigned char aRawClassData[CLASS_SIZE]);

private:
    unsigned char rawClassData[CLASS_SIZE];
    unsigned char classId;
    unsigned char hp;
    unsigned char strength;
    unsigned char skill;
    unsigned char speed;
    unsigned char defense;
    unsigned char res;
    unsigned char luck;
    unsigned char move;

    static const int ID_INDEX = 4;
    static const int HP_INDEX = 11;
    static const int STRENGTH_INDEX = 12;
    static const int SKILL_INDEX = 13;
    static const int SPEED_INDEX = 14;
    static const int DEFENSE_INDEX = 15;
    static const int RES_INDEX = 16;
    static const int LUCK_INDEX = 17;
    static const int MOVE_INDEX = 18;
};

#endif