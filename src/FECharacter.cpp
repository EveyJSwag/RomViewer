#include "FECharacter.h"

FECharacter::FECharacter(unsigned char aRawCharacterData[CHARACTER_SIZE])
{
    name = aRawCharacterData[NAME_INDEX] | (aRawCharacterData[NAME_INDEX+1] << 8);
    details = aRawCharacterData[DETAIL_INDEX] | (aRawCharacterData[DETAIL_INDEX + 1] << 8);
    id = aRawCharacterData[ID_INDEX];
    level = aRawCharacterData[LEVEL_INDEX];
    hp = aRawCharacterData[HP_INDEX];
    strength = aRawCharacterData[STRENGTH_INDEX];
    skill = aRawCharacterData[SKILL_INDEX];
    speed = aRawCharacterData[SPEED_INDEX];
    defense = aRawCharacterData[DEFENSE_INDEX];
    res = aRawCharacterData[RES_INDEX];
    luck = aRawCharacterData[LUCK_INDEX];
    con = aRawCharacterData[CON_INDEX];

    growthHp = aRawCharacterData[HP_GROWTH_INDEX];
    growthSpeed = aRawCharacterData[STRENGTH_GROWTH_INDEX];
    growthSkill = aRawCharacterData[SKILL_GROWTH_INDEX];
    growthDefense = aRawCharacterData[DEFENSE_GROWTH_INDEX];
    growthRes = aRawCharacterData[RES_GROWTH_INDEX];
    growthLuck = aRawCharacterData[LUCK_GROWTH_INDEX];
}