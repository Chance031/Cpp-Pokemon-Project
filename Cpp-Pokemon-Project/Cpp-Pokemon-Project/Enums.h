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

// ��� Ÿ��
enum class SkillCategory {
    NOEN,
    PHYSICAL,   // ����
    SPECIAL,    // Ư��
    STATUS      // ��ȭ
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

enum class SkillBehavior {
    NORMAL,
    TWO_TURN_INVULNERABLE,      // ���߳���, �����ı�ó�� 1��°�� ���ϴ� ���
    CHARGE_NO_INVULNERABLE,     // �ֶ��ó�� 1��°�� �⸦ ������ ���
    DIGGING,                    // ���ӿ� ���� ����
    FLYING,                     // ���߿� �ִ� ����
    // ... ���
};