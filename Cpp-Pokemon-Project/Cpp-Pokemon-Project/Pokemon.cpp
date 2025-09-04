#include "Pokemon.h"

#include <iostream>

Pokemon::Pokemon(int speciesId, int level)
	: speciesId_(speciesId), level_(level)
{
    // ��ü��(IV)�� 0~31 ������ ������ �����մϴ�. (������ ���÷� 31�� ����)
    ivs_[Stat::HP] = 31;
    ivs_[Stat::ATTACK] = 31;
    ivs_[Stat::DEFENSE] = 31;
    ivs_[Stat::SPECIAL_ATTACK] = 31;
    ivs_[Stat::SPECIAL_DEFENSE] = 31;
    ivs_[Stat::SPEED] = 31;

    // ���ġ(EV)�� ó������ ��� 0�Դϴ�.
    evs_[Stat::HP] = 0;
    evs_[Stat::ATTACK] = 0;
    evs_[Stat::DEFENSE] = 0;
    evs_[Stat::SPECIAL_ATTACK] = 0;
    evs_[Stat::SPECIAL_DEFENSE] = 0;
    evs_[Stat::SPEED] = 0;

    // ����(Nature)�� 25���� �� �ϳ��� �����ϰ� �����մϴ�. (������ ���÷� ����)
    nature_ = Nature::MODEST; // ���� ����

    // ��Ÿ ���� �ʱ�ȭ
    currentExp_ = 0; // ����ġ�� 0���� ����
    primaryStatus_ = StatusCondition::NONE; // ���� �̻� ����
    // ...

    // --- ����������, ������ ����� ù �ɷ�ġ�� ����մϴ� ---
    RecalculateStats();

    // HP�� ó������ ���� �� �ֽ��ϴ�.
    currentHp_ = maxHp_;

    std::cout << "���� " << level_ << " ���ϸ� ���� �Ϸ�!" << std::endl;
}

void Pokemon::TakeDamage(int damage)
{
    currentHp_ -= damage;
    if (currentHp_ < 0)
        currentHp_ = 0;
}

void Pokemon::Heal(int amount)
{
    currentHp_ += amount;
    if (currentHp_ > maxHp_)
        currentHp_ = maxHp_;
}

void Pokemon::AddExp(int amount)
{
    // TODO: GDD�� '����ġ ����'�� ���� amount�� �����ϴ� ���� �߰�
    // (��: ��� ����, Ʈ���̳��� ���ʽ� ��)
    currentExp_ += amount;
    std::cout << GetName() << "��(��) " << amount << "�� ����ġ�� �����!" << std::endl;

    // --- ���� �� üũ ---
    // ���� �� ������ �� �� �����Ƿ� while ������ üũ�մϴ�.
    while (true)
    {
        // DataManager���� ���� ������ �ʿ��� �� ����ġ�� �����´ٰ� ����
        // int requiredExp = DataManager::GetInstance()->GetTotalExpForLevel(level_ + 1, expGroup_);

        // �ӽ� �ʿ� ����ġ (�����δ� ����ġ � �Լ� ���)
        int requiredExp = (level_ + 1) * (level_ + 1) * (level_ + 1);

        if (currentExp_ >= requiredExp)
        {
            LevelUp(); // �ʿ� ����ġ�� �Ѿ����� ���� ��!
        }
        else
        {
            break; // ���� ���ϸ� ���� ����
        }
    }
}

bool Pokemon::LearnMove(const Move& move)
{
    if (moveset_.size() < 4)
    {
        moveset_.push_back(move);
        return true; // ���� ����
    }

    // TODO: 4���� �ʰ����� ��, ���� ����� �ذ� ����� ����� UI ���� ȣ��

    return false; // ���� ���� (�ڸ��� ����)
}

int Pokemon::GetStat(Stat stat) const
{
    switch (stat)
    {
    case Stat::HP: return maxHp_; // ���� HP�� �ƴ� �ִ� HP�� ��ȯ�ϴ� ���� �Ϲ���
    case Stat::ATTACK: return attack_;
    case Stat::DEFENSE: return defense_;
    case Stat::SPECIAL_ATTACK: return specialAttack_;
    case Stat::SPECIAL_DEFENSE: return specialDefense_;
    case Stat::SPEED: return speed_;
    default: return 0; // ���� ó��
    }
}

void Pokemon::RecalculateStats()
{
    // DataManager���� �� ���ϸ��� ������(Base Stat)�� �����Դٰ� �����մϴ�.
    // int baseHp = DataManager::GetInstance()->GetSpecies(speciesId_)->GetBaseStat(Stat::HP);
    // int baseAttack = ...;

    // --- �ӽ� ������ (��: �̻��ؾ�) ---
    int baseHp = 45;
    int baseAttack = 49;
    int baseDefense = 49;
    int baseSpecialAttack = 65;
    int baseSpecialDefense = 65;
    int baseSpeed = 45;

    // --- HP ��� (GDD ����) ---
    maxHp_ = floor(0.01 * (2 * baseHp + ivs_[Stat::HP] + floor(0.25 * evs_[Stat::HP])) * level_) + level_ + 10;

    // --- ������ �ɷ�ġ ��� (GDD ����) ---
    // ���� ���������� Nature�� ����ġ(1.1, 1.0, 0.9)�� �������� ������ �ʿ��մϴ�.
    double natureModifier = 1.0;

    attack_ = floor((floor(0.01 * (2 * baseAttack + ivs_[Stat::ATTACK] + floor(0.25 * evs_[Stat::ATTACK])) * level_) + 5) * natureModifier);
    defense_ = floor((floor(0.01 * (2 * baseDefense + ivs_[Stat::DEFENSE] + floor(0.25 * evs_[Stat::DEFENSE])) * level_) + 5) * natureModifier);
    specialAttack_ = floor((floor(0.01 * (2 * baseSpecialAttack + ivs_[Stat::SPECIAL_ATTACK] + floor(0.25 * evs_[Stat::SPECIAL_ATTACK])) * level_) + 5) * natureModifier);
    specialDefense_ = floor((floor(0.01 * (2 * baseSpecialDefense + ivs_[Stat::SPECIAL_DEFENSE] + floor(0.25 * evs_[Stat::SPECIAL_DEFENSE])) * level_) + 5) * natureModifier);
    speed_ = floor((floor(0.01 * (2 * baseSpeed + ivs_[Stat::SPEED] + floor(0.25 * evs_[Stat::SPEED])) * level_) + 5) * natureModifier);
}

void Pokemon::LevelUp()
{
    level_++;
    std::cout << GetName() << "�� ������ " << level_ << "(��)�� �ö���!" << std::endl;

    // 1. �ɷ�ġ ���� (���� �߿�)
    RecalculateStats();

    // 2. ���ο� ����� ������ Ȯ��
    CheckForNewMoves();

    // 3. ��ȭ�ϴ��� Ȯ��
    CheckForEvolution();
}

void Pokemon::CheckForEvolution()
{
}

void Pokemon::CheckForNewMoves()
{
}


