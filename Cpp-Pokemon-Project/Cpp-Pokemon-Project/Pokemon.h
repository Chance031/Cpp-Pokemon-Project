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
    // 생성자
    Pokemon(int speciesId, int level);

    // --- 공개 멤버 함수 ---
    void TakeDamage(int damage);
    void Heal(int amount);
    void AddExp(int amount);
    bool LearnMove(const Move& move);

    // --- Getter 함수 ---
    std::string GetNickname() const { return nickname_; }
    int GetLevel() const { return level_; }
    int GetCurrentHP() const { return currentHp_; }
    int GetMaxHP() const { return maxHp_; }
    int GetStat(Stat stat) const; // 특정 스탯의 최종 계산값을 반환
    const std::vector<Move>& GetMoveset() const { return moveset_; }
    // ... 등등 필요한 모든 Getter ...

private:
    // --- 비공개 멤버 함수 ---
    void RecalculateStats();
    void LevelUp();
    void CheckForEvolution();
    void CheckForNewMoves();

    // --- 멤버 변수 (GDD 기반 최종본) ---

    // 기본 정보
    int speciesId_;
    std::string nickname_;
    int level_;
    int currentExp_;
    int friendship_;

    // 능력치 '재료'
    Nature nature_;
    std::map<Stat, int> ivs_;
    std::map<Stat, int> evs_;

    // 계산된 최종 능력치 (실수치)
    int maxHp_;
    int currentHp_;
    int attack_;
    int defense_;
    int specialAttack_;
    int specialDefense_;
    int speed_;

    // 기타 시스템 데이터
    Type type1_;
    Type type2_;
    std::vector<Move> moveset_;
    std::map<Stat, int> statStages_;
    StatusCondition primaryStatus_;
    // Ability ability_;
    // Item heldItem_;
};