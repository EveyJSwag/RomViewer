#include "FEClass.h"

FEClass::FEClass(unsigned char aRawClassData[CLASS_SIZE])
{
    classId = aRawClassData[ID_INDEX];
    hp = aRawClassData[HP_INDEX];
    strength = aRawClassData[STRENGTH_INDEX];
    skill = aRawClassData[SKILL_INDEX];
    speed = aRawClassData[SPEED_INDEX];
    defense = aRawClassData[DEFENSE_INDEX];
    res = aRawClassData[RES_INDEX];
    luck = aRawClassData[LUCK_INDEX];
    move = aRawClassData[MOVE_INDEX];
}