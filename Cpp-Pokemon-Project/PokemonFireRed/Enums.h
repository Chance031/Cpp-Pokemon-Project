#pragma once

#include <string>
#include <vector>

// =================================================================
// ���ϸ� �ٽ� �Ӽ� (Core Pokemon Attributes)
// =================================================================

// ���ϸ� Ÿ��
enum class Type 
{
    NONE,
    NORMAL,     // �븻
    FIRE,       // ��
    WATER,      // ��
    GRASS,      // Ǯ
    ELECTRIC,   // ����
    ICE,        // ����
    FIGHTING,   // ����
    POISON,     // ��
    GROUND,     // ��
    FLYING,     // ����
    PSYCHIC,    // ������
    BUG,        // ����
    ROCK,       // ����
    GHOST,      // ��Ʈ
    DRAGON,     // �巡��
    DARK,       // ��
    STEEL,      // ��ö
    FAIRY       // ����
};

// ���ϸ��� �ɷ�ġ ����
enum class Stat 
{
    HP,                 // H
    ATTACK,             // A
    DEFENSE,            // B
    SPECIAL_ATTACK,     // C
    SPECIAL_DEFENSE,    // D
    SPEED,              // S
    ACCURACY,           // ���߷�
    EVASION             // ȸ����
};

// ���ϸ��� ����
enum class Nature 
{
    HARDY,      // ��� 
    LONELY,     // �ܷο�
    ADAMANT,    // ����
    NAUGHTY,    // ��������
    BRAVE,      // �밨
    BOLD,       // ���
    DOCILE,     // �¼�
    IMPISH,     // �峭�ٷ���
    LAX,        // �Ͷ�
    RELAXED,    // ��������
    MODEST,     // ����
    MILD,       // ����
    BASHFUL,    // ������
    RASH,       // ����
    QUIET,      // ����
    CALM,       // ����
    GENTLE,     // ����
    CAREFUL,    // ����
    QUIRKY,     // ����
    SASSY,      // �ǹ�
    TIMID,      // ������
    HASTY,      // ����
    JOLLY,      // ���
    NAIVE,      // õ������
    SERIOUS     // ����
};

// =================================================================
// ��� ���� (Move-related)
// =================================================================

// ��� Ÿ��
enum class MoveCategory 
{
    PHYSICAL,   // ����
    SPECIAL,    // Ư��
    STATUS      // ��ȭ
};

// ����� ��� ����
enum class MoveTarget 
{
    SELECTED_TARGET,        // ������ 1ü (�� �Ǵ� �Ʊ�)
    SELF,                   // �ڱ� �ڽ�
    ALLY,                   // �Ʊ� 1ü
    RANDOM_TARGET,          // ������ ��� 1ü
    ALL_ADJACENT_FOES,      // ������ ��� ��
    ALL_ADJACENT_POKEMON,   // ������ ��� ���ϸ�
    FIELD,                  // �ʵ� ��ü
    SPECIAL                 // Ư�� ���̽�
};

// ����� Ư�� �ൿ ���
enum class MoveExecutionType 
{
    NORMAL,             // �Ϲ� ����
    CHARGE,             // 1�� ������ (�ֶ��)
    TWO_TURN,           // 1�� ���� 2�� ���� (���߳���, �����ı�)
    RECHARGE            // ��� �� 1�� �ൿ�Ұ� (�ı�����)
};

// =================================================================
// ���ϸ� ���� �� ��ü ���� (Growth & Individual-related)
// =================================================================

// ����
enum class Gender
{
    MALE,           // ����
    FEMALE,         // ����
    GENDERLESS      // ����
};

// ����ġ �׷� (���� �ӵ�)
enum class ExpGroup
{
    ERRATIC,            // 600,000      (�ұ�Ģ)
    FAST,               // 800,000      (����)
    MEDIUM_FAST,        // 1,000,000    (�߰� ����)
    MEDIUM_SLOW,        // 1,059,860    (�߰� ����)
    SLOW,               // 1,250,000    (����)
    FLUCTUATING         // 1,640,000    (����)
};

// =================================================================
// ��Ʋ ���� �� ȯ�� ���� (Battle Status & Environment-related)
// =================================================================

// �ֿ� ���� �̻�
enum class StatusCondition
{
    NONE,
    POISON,     // ��
    TOXIC,      // �͵�
    BURN,       // ȭ��
    PARALYSIS,  // ����
    SLEEP,      // ���
    FREEZE      // ����
};

// �Ͻ����� ���� ����
enum class VolatileStatus
{
    NONE,
    CONFUSION,    // ȥ��
    FLINCH,       // Ǯ����
    LEECH_SEED,   // ���Ѹ���
    TRAPPED       // ����
};

// ���� ����
enum class Weather 
{
    NONE,
    HARSH_SUNLIGHT, // ��û
    RAIN,           // ��
    SANDSTORM,      // �𷡹ٶ�
    SNOW            // ����
};

// �ʵ� ����
enum class Terrain 
{
    NONE,
    ELECTRIC,           // �Ϸ�Ʈ���ʵ�
    GRASSY,             // �׷����ʵ�
    MISTY,              // �̽�Ʈ�ʵ�
    PSYCHIC             // �������ʵ�
};

// =================================================================
// ��� ȿ�� ���� (Move Effect-related)
// =================================================================

// ��� ȿ���� ī�װ�
enum class EffectCategory
{
    NONE,
    PRIMARY_STATUS,     // ȭ��, ���� �� �ֿ� �����̻�
    MULTI_STATUS,       // ���� �����̻� �� �ϳ��� �� (Ʈ���̾���)
    VOLATILE_STATUS,    // ȥ��, Ǯ���� �� �ֹ߼� ����
    STAT_CHANGE,        // ���� ���� ��ũ ��ȭ
    MULTI_STAT_CHANGE,  // ���� ���� ��ũ ��ȭ
    HEAL,               // ȸ��
    DAMAGE,             // ������ (���� ������, HP ��� ��)

    MULTI_HIT,          // ���� ����
    CHARGE_TURN,        // 1�� ������
    TWO_TURN_ATTACK,    // 2�� ���� (���߳��� ��)
    RECHARGE_TURN,      // ��� �� 1�� �޽� (�ı�����)
    RECOIL,             // �ݵ� ������
    DRAIN,              // HP ���
    INSTANT_EFFECT,     // �ϰ��ʻ� �� ��� �ߵ� ȿ��
    FIXED_DAMAGE,       // ���� ������
    POST_BATTLE         // ��Ʋ ���� �� ȿ�� (����̵��ޱ�)
};

// =================================================================
// Ư�� ���� (Ability-related)
// =================================================================

enum class AbilityTrigger
{
    // --- ���� ȿ�� ---
    PASSIVE,                // �ʵ忡 �ִ� ���� ��� �ߵ�

    // --- ���� / ���� �� ---
    ON_SWITCH_IN,           // ��ü�� �������� ��
    ON_SWITCH_OUT,          // ��ü�� ������ ��

    // --- �� ��� ---
    ON_END_TURN,            // ���� ���� ��

    // --- �ǰ� / ���� �� ---
    ON_CONTACT,             // ���� ������ �޾��� ��
    ON_HIT_BY_MOVE,         // ����� �¾��� �� (����/������ ����)
    ON_HIT_BY_TYPE,         // Ư�� Ÿ���� ����� �¾��� ��
    ON_HIT_PHYSICAL,        // ���� ����� �¾��� ��
    ON_CRITICAL_HIT,        // �޼ҿ� �¾��� ��
    ON_MOVE_TARGET,         // �ڽ��� ����� ����� �Ǿ��� �� (������)

    // --- �ൿ �� ---
    ON_MOVE_USE,            // ����� ����Ϸ��� �� �� (��ȯ����)
    ON_MOVE_LAST,           // �Ͽ��� ���� ���������� �ൿ�� ��

    // --- ���� ��ȭ �� ---
    ON_STATUS,              // Ư�� �����̻��� �Ǿ��� ��
    ON_STAT_LOWERED,        // �ɷ�ġ ��ũ�� �������� ��
    ON_FOE_STAT_UP,         // ����� �ɷ�ġ ��ũ�� �ö��� ��
    ON_FLINCH,              // Ǯ�׾��� ��
    ON_INTIMIDATED,         // ������ �޾��� ��

    // --- HP ��ȭ �� ---
    ON_LOW_HP,              // HP�� Ư�� ���� ������ ��

    // --- ����(Faint) ���� ---
    ON_KO,                  // ��븦 �����߷��� ��
    ON_FAINT,               // �ڽ��� �������� ��
    ON_FAINT_CONTACT,       // ���� �������� �ڽ��� �������� ��
    ON_ANY_FAINT,           // �ʵ��� �������� �������� ��
    ON_ALLY_FAINT,          // �Ʊ��� �������� ��

    // --- �ʵ� / ���� / ������ ���� ---
    ON_WEATHER,             // Ư�� ������ ��
    ON_TERRAIN,             // Ư�� �ʵ��� ��
    ON_TERRAIN_CHANGE,      // �ʵ尡 ����Ǿ��� ��
    ON_BERRY_EAT,           // �������Ÿ� �Ծ��� ��
    ON_ALLY_ITEM_USE,       // �Ʊ��� �������� ������� ��
    ON_OTHER_MOVE_USE,      // �ʵ��� �ٸ� ���ϸ��� Ư�� ����� ������� �� (����)
    ON_END_BATTLE,          // ��Ʋ ���� ��

    // --- Ư�� ���� ---
    SPECIAL_LOGIC           // �� �з��� �ش����� �ʴ� ����/���� ����
};

enum class AbilityCategory
{
    // --- �ɷ�ġ(Stat) ���� ---
    STAT_CHANGE,                // �ڽ���/����� ���� ��ũ�� ���� ���� (����, ����)
    STAT_MODIFIER,              // �ڽ��� ���� �Ǽ�ġ�� Ư�� ������ ���� (õ�����)
    STAT_MODIFIER_STATUS,       // �����̻� �� ���� �Ǽ�ġ�� ���� (�ټ�)
    STAT_MODIFIER_IMMUNITY,     // Ư�� ���� ��ũ�� �������� �ʰ� �� (��������)
    REVERSE_STAT_CHANGES,       // ���� ��ũ ��ȭ�� ������Ŵ (�ɼ��ٷ���)

    // --- ������ ��� ���� ---
    DAMAGE_MODIFIER,            // �ִ�/�޴� �������� ���� (��ũ�ϼ�, ����)
    DAMAGE_MODIFIER_TYPE,       // Ư�� Ÿ�� ����� �������� ���� (�β�������)
    DAMAGE_IMMUNITY,            // Ư�� ������ �������� ��ȿȭ (�Ұ����Ǻ���)
    DAMAGE,                     // ���� �������� �� (��ĥ���Ǻ�)

    // --- �����̻�(Status) ���� ---
    PRIMARY_STATUS,             // ��, ���� �� �ֿ� �����̻��� ��
    MULTI_STATUS,               // ���� �ֿ� �����̻� �� �ϳ��� ��
    VOLATILE_STATUS,            // ȥ�� �� �Ͻ��� �����̻��� ��
    STATUS_IMMUNITY,            // Ư�� �ֿ� �����̻��� ����
    STATUS_HEAL,                // �����̻��� ȸ����
    STATUS_TRANSFER,            // �����̻��� ��뿡�� �ű�

    // --- ���(Move) ���� ---
    MOVE_IMMUNITY,              // Ư�� ������ ����� ��ȿȭ (����)
    MOVE_MODIFIER,              // ����� �Ӽ��� ���� (Ʋ����)
    EFFECT_CHANCE_MOD,          // ����� �ΰ� ȿ�� �ߵ� Ȯ���� ���� (�ϴ�������)
    PP_USAGE_MODIFIER,          // PP �Ҹ��� ���� (������)
    ACCURACY_MODIFIER,          // ���߷��� ���� (����, �밡��)
    PRIORITY_MODIFIER,          // �켱���� ���� (����������)
    CRITICAL_HIT_MODIFIER,      // �޼� Ȯ���� ���� (���)

    // --- Ÿ��(Type) ���� ---
    TYPE_CHANGE,                // �ڽ��� Ÿ���� ����
    TYPE_IMMUNITY,              // Ư�� Ÿ�� ����� ��ȿȭ (����)
    TYPE_EFFECTIVENESS,         // Ÿ�� ���� ������ (��¯)

    // --- �ʵ�(Field) ���� ---
    WEATHER_CHANGE,             // ������ ����
    TERRAIN_CHANGE,             // �ʵ�(����)�� ����
    FIELD_EFFECT_NEGATE,        // �ʵ� ȿ���� ��ȿȭ

    // --- ���/ȸ�� ���� ---
    HEAL,                       // HP�� ȸ�� (����)
    ABSORB_BOOST,               // Ư�� Ÿ�� ����� ����ϰ� �ɷ�ġ ��ȭ (Ÿ�����ºҲ�)
    REDIRECT_BOOST,             // Ư�� Ÿ�� ����� �ڽſ��� �����ϰ� ��ȭ (�Ƿ�ħ)

    // --- ��Ÿ ȿ�� ---
    EFFECT_IMMUNITY,            // Ǯ����, �޼� �� Ư�� ȿ���� ���� (��������)
    SURVIVE_LETHAL,             // �ϰݿ� �������� ���� ���� (�˰���)
    TRAP,                       // ����� ��ü�� ���� (�׸��ڹ��)
    ITEM_ACTION,                // ������ ���� ȿ�� (����, ���尨)
    BATTLE_ACTION,              // ��Ʋ �ൿ ���� ȿ�� (����)
    TRANSFORM,                  // ����
    WEIGHT_MODIFIER,            // ���Ը� ����

    // --- Ư�� ���� ---
    SPECIAL_LOGIC               // �� �з��� �ش����� �ʴ� ����/���� ����
};