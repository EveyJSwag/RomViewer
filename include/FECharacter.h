#ifndef FECHARACTER_H
#define FECHARACTER_H
class FECharacter
{
public:
    static const int CHARACTER_SIZE = 52;

    FECharacter(unsigned char aRawCharacterData[CHARACTER_SIZE]);

private:
    unsigned char rawCharacterData[CHARACTER_SIZE];

    unsigned short name;
    unsigned short details;
    unsigned char id;
    unsigned char level;
    unsigned char hp;
    unsigned char strength;
    unsigned char skill;
    unsigned char speed;
    unsigned char defense;
    unsigned char res;
    unsigned char luck;
    unsigned char con;

    unsigned char growthHp;
    unsigned char growthStrength;
    unsigned char growthSkill;
    unsigned char growthSpeed;
    unsigned char growthDefense;
    unsigned char growthRes;
    unsigned char growthLuck;

    static const int NAME_INDEX = 0;
    static const int DETAIL_INDEX = 2;
    static const int ID_INDEX = 4;

    static const int LEVEL_INDEX = 11;
    static const int HP_INDEX = 12;
    static const int STRENGTH_INDEX = 13;
    static const int SKILL_INDEX = 14;
    static const int SPEED_INDEX = 15;
    static const int DEFENSE_INDEX = 16;
    static const int RES_INDEX = 17;
    static const int LUCK_INDEX = 18;
    static const int CON_INDEX = 19;

    static const int HP_GROWTH_INDEX = 28;
    static const int STRENGTH_GROWTH_INDEX = 29;
    static const int SKILL_GROWTH_INDEX = 30;
    static const int SPEED_GROWTH_INDEX = 31;
    static const int DEFENSE_GROWTH_INDEX = 32;
    static const int RES_GROWTH_INDEX = 33;
    static const int LUCK_GROWTH_INDEX = 34;
};

#endif