#pragma once

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
    SPEED               // S
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
    SELF,               // ����� �ڽ�
    SELECTED_TARGET,    // ������ ��� 1ü (�� �Ǵ� �Ʊ�)
    ALL_ADJACENT_FOES,  // ������ ��� ��
    ALL_ALLIES,         // �Ʊ� ��ü (����� ����)
    ALL_ADJACENT        // ������ ��� ���ϸ� (����� ����, �Ʊ� ����)
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