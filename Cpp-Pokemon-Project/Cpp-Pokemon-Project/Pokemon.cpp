#include "Pokemon.h"
#include "DataManager.h"

#include <iostream>
#include <cmath>

// (�ӽ�) ���ݿ� ���� �ɷ�ġ ���� �����͸� �������� �Լ�
// �����δ� DataManager�� ������ NatureManager�� �� ������ �ؾ� �մϴ�.
std::pair<Stat, Stat> GetNatureStatMods(Nature nature) {
    // ����: ����(MODEST) ������ Ư������(SPECIAL_ATTACK)�� �ø��� ����(ATTACK)�� ����
    if (nature == Nature::MODEST) {
        return { Stat::SPECIAL_ATTACK, Stat::ATTACK };
    }
    // ... �ٸ� ��� ���ݿ� ���� ������ �߰� ...
    // ������ ���� ����
    return { Stat::HP, Stat::HP }; // ������ �ߺ� �������� ���� ������ ǥ��
}

// (�ӽ�) ����ġ �׷쿡 ���� ���� ���� �ʿ� ����ġ�� ����ϴ� �Լ�
int GetRequiredExpForLevel(int level, ExpGroup group) {
    // GDD�� ����ġ � ��Ģ�� ���� ���
    if (group == ExpGroup::MEDIUM_FAST) {
        return level * level * level;
    }
    // ... �ٸ� ��� ����ġ �׷쿡 ���� ���� �߰� ...
    return level * level * level; // �⺻��
}

// ==========================================================
// Pokemon Ŭ���� ����
// ==========================================================

Pokemon::Pokemon(int speciesId, int level)
// --- ��� �̴ϼȶ����� ����Ʈ�� ����� ȿ������ �ʱ�ȭ ---
    : speciesId_(speciesId),
    level_(level),
    // DataManager���� �ش� ���ϸ��� '���� ������'�� ������ �ʱ�ȭ
    name_(DataManager::GetInstance()->GetPokemonSpeciesData(speciesId).name),
    type1_(DataManager::GetInstance()->GetPokemonSpeciesData(speciesId).type1),
    type2_(DataManager::GetInstance()->GetPokemonSpeciesData(speciesId).type2),
    // TODO: Ability, Item�� DataManager���� �����ͼ� �ʱ�ȭ
    ivs_({ {Stat::HP, 31}, {Stat::ATTACK, 31}, {Stat::DEFENSE, 31}, {Stat::SPECIAL_ATTACK, 31}, {Stat::SPECIAL_DEFENSE, 31}, {Stat::SPEED, 31} }),
    evs_({ {Stat::HP, 0}, {Stat::ATTACK, 0}, {Stat::DEFENSE, 0}, {Stat::SPECIAL_ATTACK, 0}, {Stat::SPECIAL_DEFENSE, 0}, {Stat::SPEED, 0} }),
    nature_(Nature::MODEST), // ���÷� '����' �������� ����
    currentExp_(0),
    friendship_(70), // �⺻ ģ�е�
    primaryStatus_(StatusCondition::NONE)
{
    // --- ������ ���������� ������ ó�� ---
    RecalculateStats();
    currentHp_ = maxHp_; // �ִ� HP�� ���� HP ����

    std::cout << name_ << " (Lv. " << level_ << ") ���� �Ϸ�!" << std::endl;
}

void Pokemon::TakeDamage(int damage) {
    currentHp_ -= damage;
    if (currentHp_ < 0)
        currentHp_ = 0;
}

void Pokemon::Heal(int amount) {
    currentHp_ += amount;
    if (currentHp_ > maxHp_)
        currentHp_ = maxHp_;
}

void Pokemon::AddExp(int amount) {
    currentExp_ += amount;
    std::cout << GetName() << "��(��) " << amount << "�� ����ġ�� �����!" << std::endl;

    while (true) {
        ExpGroup expGroup = DataManager::GetInstance()->GetPokemonSpeciesData(speciesId_).exp_group;
        int requiredExp = GetRequiredExpForLevel(level_ + 1, expGroup);

        if (currentExp_ >= requiredExp) {
            LevelUp();
        }
        else {
            break;
        }
    }
}

bool Pokemon::LearnMove(const Move& move) {
    if (moveset_.size() < 4) {
        moveset_.push_back(move);
        return true;
    }
    return false;
}

int Pokemon::GetStat(Stat stat) const {
    switch (stat) {
    case Stat::HP: return maxHp_;
    case Stat::ATTACK: return attack_;
    case Stat::DEFENSE: return defense_;
    case Stat::SPECIAL_ATTACK: return specialAttack_;
    case Stat::SPECIAL_DEFENSE: return specialDefense_;
    case Stat::SPEED: return speed_;
    default: return 0;
    }
}

void Pokemon::RecalculateStats() {
    const PokemonSpeciesData& speciesData = DataManager::GetInstance()->GetPokemonSpeciesData(speciesId_);

    // HP ���
    maxHp_ = floor(0.01 * (2 * speciesData.hp + ivs_.at(Stat::HP) + floor(0.25 * evs_.at(Stat::HP))) * level_) + level_ + 10;

    // ���� ����ġ ��� ����
    auto natureMods = GetNatureStatMods(nature_);
    Stat raisedStat = natureMods.first;
    Stat loweredStat = natureMods.second;

    // �� �ɷ�ġ���� ���� ���� ���(Modifier)�� ���
    double attackMod = (raisedStat == Stat::ATTACK) ? 1.1 : (loweredStat == Stat::ATTACK) ? 0.9 : 1.0;
    double defenseMod = (raisedStat == Stat::DEFENSE) ? 1.1 : (loweredStat == Stat::DEFENSE) ? 0.9 : 1.0;
    double spAttackMod = (raisedStat == Stat::SPECIAL_ATTACK) ? 1.1 : (loweredStat == Stat::SPECIAL_ATTACK) ? 0.9 : 1.0;
    double spDefenseMod = (raisedStat == Stat::SPECIAL_DEFENSE) ? 1.1 : (loweredStat == Stat::SPECIAL_DEFENSE) ? 0.9 : 1.0;
    double speedMod = (raisedStat == Stat::SPEED) ? 1.1 : (loweredStat == Stat::SPEED) ? 0.9 : 1.0;

    // ������ �ɷ�ġ ���
    attack_ = floor((floor(0.01 * (2 * speciesData.attack + ivs_.at(Stat::ATTACK) + floor(0.25 * evs_.at(Stat::ATTACK))) * level_) + 5) * attackMod);
    defense_ = floor((floor(0.01 * (2 * speciesData.defense + ivs_.at(Stat::DEFENSE) + floor(0.25 * evs_.at(Stat::DEFENSE))) * level_) + 5) * defenseMod);
    specialAttack_ = floor((floor(0.01 * (2 * speciesData.sp_attack + ivs_.at(Stat::SPECIAL_ATTACK) + floor(0.25 * evs_.at(Stat::SPECIAL_ATTACK))) * level_) + 5) * spAttackMod);
    specialDefense_ = floor((floor(0.01 * (2 * speciesData.sp_defense + ivs_.at(Stat::SPECIAL_DEFENSE) + floor(0.25 * evs_.at(Stat::SPECIAL_DEFENSE))) * level_) + 5) * spDefenseMod);
    speed_ = floor((floor(0.01 * (2 * speciesData.speed + ivs_.at(Stat::SPEED) + floor(0.25 * evs_.at(Stat::SPEED))) * level_) + 5) * speedMod);
}

void Pokemon::LevelUp() {
    level_++;
    std::cout << GetName() << "�� ������ " << level_ << "(��)�� �ö���!" << std::endl;
    RecalculateStats();
    // CheckForNewMoves(); // TODO: DataManager�� �����Ͽ� ����
    // CheckForEvolution(); // TODO: DataManager�� �����Ͽ� ����
}

// �� �Լ����� DataManager�� �ϼ��� ��, �װ��� �����͸� �̿��� �����ؾ� �մϴ�.
void Pokemon::CheckForEvolution() {}
void Pokemon::CheckForNewMoves() {}