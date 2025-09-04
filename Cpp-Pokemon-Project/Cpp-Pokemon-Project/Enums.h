#pragma once

// ���ϸ� Ÿ��
enum class Type {
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
enum class Stat {
    HP,
    ATTACK,
    DEFENSE,
    SPECIAL_ATTACK,
    SPECIAL_DEFENSE,
    SPEED
};

// ���ϸ��� ����
enum class Nature {
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

// ����ġ �׷� (���� �ӵ�)
enum class ExpGroup {
    ERRATIC,            // 600,000      (�ұ�Ģ)
    FAST,               // 800,000      (����)
    MEDIUM_FAST,        // 1,000,000    (�߰� ����)
    MEDIUM_SLOW,        // 1,059,860    (�߰� ����)
    SLOW,               // 1,250,000    (����)
    FLUCTUATING         // 1,640,000    (����)
};

// ��� Ÿ��
enum class SkillCategory {
    PHYSICAL,   // ����
    SPECIAL,    // Ư��
    STATUS      // ��ȭ
};

// ����� ��� ����
enum class TargetType {
    SINGLE_OPPONENT,    // ��� �ϳ�
    ALL_OPPONENTS,      // ������ ��� ���
    SELF,               // �ڱ� �ڽ�
    ALL_ALLIES,         // ��� �Ʊ�
    ENTIRE_FIELD        // �ʵ� ��ü
};


// ����� Ư�� �ൿ ���
enum class SkillBehavior {
    NORMAL,
    TWO_TURN_INVULNERABLE,      // ���߳���, �����ı�ó�� 1��°�� ���ϴ� ���
    CHARGE_NO_INVULNERABLE,     // �ֶ��ó�� 1��°�� �⸦ ������ ���
    DIGGING,                    // ���ӿ� ���� ����
    FLYING,                     // ���߿� �ִ� ����
    // ... ���
};

// ���� �̻�
enum class StatusCondition {
    NONE,
    POISON,     // ��
    TOXIC,      // �͵�
    BURN,       // ȭ��
    PARALYSIS,  // ����
    SLEEP,      // ���
    FREEZE      // ����
};

// ���� ��ȭ
enum class VolatileStatus {
    NONE,
    CONFUSION,    // ȥ��
    INFATUATION,  // ������
    LEECH_SEED,   // ���Ѹ���
    FLINCH        // Ǯ����
};

// ���� ����
enum class Weather {
    NONE,
    HARSH_SUNLIGHT,     // ��û
    RAIN,               // ��
    SANDSTORM,          // �𷡹ٶ�
    SNOW                // ����
};

// �ʵ� ����
enum class Terrain {
    NONE,
    ELECTRIC,           // �Ϸ�Ʈ���ʵ�
    GRASSY,             // �׷����ʵ�
    MISTY,              // �̽�Ʈ�ʵ�
    PSYCHIC             // �������ʵ�
};