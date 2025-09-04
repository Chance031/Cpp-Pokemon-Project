#pragma once

#include <string>
#include <vector>
#include <map>

#include "Enums.h"
#include "Move.h"

class Item;
class Ability;

class Pokemon {
public:
    // ������
    Pokemon(int speciesId, int level);

    // --- ���� ��� �Լ� ---
    void TakeDamage(int damage);
    void Heal(int amount);
    void AddExp(int amount);
    bool LearnMove(const Move& move);

    // --- Getter �Լ� ---
    std::string GetNickname() const { return nickname_; }
    int GetLevel() const { return level_; }
    int GetCurrentHP() const { return currentHp_; }
    int GetMaxHP() const { return maxHp_; }
    int GetStat(Stat stat) const; // Ư�� ������ ���� ��갪�� ��ȯ
    const std::vector<Move>& GetMoveset() const { return moveset_; }
    // ... ��� �ʿ��� ��� Getter ...

private:
    // --- ����� ��� �Լ� ---
    void RecalculateStats();
    void LevelUp();
    void CheckForEvolution();
    void CheckForNewMoves();

    // --- ��� ���� (GDD ��� ������) ---

    // �⺻ ����
    int speciesId_;
    std::string nickname_;
    int level_;
    int currentExp_;
    int friendship_;

    // �ɷ�ġ '���'
    Nature nature_;
    std::map<Stat, int> ivs_;
    std::map<Stat, int> evs_;

    // ���� ���� �ɷ�ġ (�Ǽ�ġ)
    int maxHp_;
    int currentHp_;
    int attack_;
    int defense_;
    int specialAttack_;
    int specialDefense_;
    int speed_;

    // ��Ÿ �ý��� ������
    Type type1_;
    Type type2_;
    std::vector<Move> moveset_;
    std::map<Stat, int> statStages_;
    StatusCondition primaryStatus_;
    // Ability ability_;
    // Item heldItem_;
};