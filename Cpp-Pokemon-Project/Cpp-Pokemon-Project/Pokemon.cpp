#include "Pokemon.h"
#include "DataManager.h"

#include <iostream>
#include <cmath>

// (임시) 성격에 따른 능력치 보정 데이터를 가져오는 함수
// 실제로는 DataManager나 별도의 NatureManager가 이 역할을 해야 합니다.
std::pair<Stat, Stat> GetNatureStatMods(Nature nature) {
    // 예시: 조심(MODEST) 성격은 특수공격(SPECIAL_ATTACK)을 올리고 공격(ATTACK)을 내림
    if (nature == Nature::MODEST) {
        return { Stat::SPECIAL_ATTACK, Stat::ATTACK };
    }
    // ... 다른 모든 성격에 대한 데이터 추가 ...
    // 보정이 없는 성격
    return { Stat::HP, Stat::HP }; // 임의의 중복 스탯으로 보정 없음을 표시
}

// (임시) 경험치 그룹에 따라 다음 레벨 필요 경험치를 계산하는 함수
int GetRequiredExpForLevel(int level, ExpGroup group) {
    // GDD의 경험치 곡선 규칙에 따라 계산
    if (group == ExpGroup::MEDIUM_FAST) {
        return level * level * level;
    }
    // ... 다른 모든 경험치 그룹에 대한 공식 추가 ...
    return level * level * level; // 기본값
}

// ==========================================================
// Pokemon 클래스 구현
// ==========================================================

Pokemon::Pokemon(int speciesId, int level)
// --- 멤버 이니셜라이저 리스트를 사용한 효율적인 초기화 ---
    : speciesId_(speciesId),
    level_(level),
    // DataManager에서 해당 포켓몬의 '원형 데이터'를 가져와 초기화
    name_(DataManager::GetInstance()->GetPokemonSpeciesData(speciesId).name),
    type1_(DataManager::GetInstance()->GetPokemonSpeciesData(speciesId).type1),
    type2_(DataManager::GetInstance()->GetPokemonSpeciesData(speciesId).type2),
    // TODO: Ability, Item도 DataManager에서 가져와서 초기화
    ivs_({ {Stat::HP, 31}, {Stat::ATTACK, 31}, {Stat::DEFENSE, 31}, {Stat::SPECIAL_ATTACK, 31}, {Stat::SPECIAL_DEFENSE, 31}, {Stat::SPEED, 31} }),
    evs_({ {Stat::HP, 0}, {Stat::ATTACK, 0}, {Stat::DEFENSE, 0}, {Stat::SPECIAL_ATTACK, 0}, {Stat::SPECIAL_DEFENSE, 0}, {Stat::SPEED, 0} }),
    nature_(Nature::MODEST), // 예시로 '조심' 성격으로 고정
    currentExp_(0),
    friendship_(70), // 기본 친밀도
    primaryStatus_(StatusCondition::NONE)
{
    // --- 생성자 본문에서는 로직만 처리 ---
    RecalculateStats();
    currentHp_ = maxHp_; // 최대 HP로 현재 HP 설정

    std::cout << name_ << " (Lv. " << level_ << ") 생성 완료!" << std::endl;
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
    std::cout << GetName() << "은(는) " << amount << "의 경험치를 얻었다!" << std::endl;

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

    // HP 계산
    maxHp_ = floor(0.01 * (2 * speciesData.hp + ivs_.at(Stat::HP) + floor(0.25 * evs_.at(Stat::HP))) * level_) + level_ + 10;

    // 성격 보정치 계산 로직
    auto natureMods = GetNatureStatMods(nature_);
    Stat raisedStat = natureMods.first;
    Stat loweredStat = natureMods.second;

    // 각 능력치별로 성격 보정 계수(Modifier)를 계산
    double attackMod = (raisedStat == Stat::ATTACK) ? 1.1 : (loweredStat == Stat::ATTACK) ? 0.9 : 1.0;
    double defenseMod = (raisedStat == Stat::DEFENSE) ? 1.1 : (loweredStat == Stat::DEFENSE) ? 0.9 : 1.0;
    double spAttackMod = (raisedStat == Stat::SPECIAL_ATTACK) ? 1.1 : (loweredStat == Stat::SPECIAL_ATTACK) ? 0.9 : 1.0;
    double spDefenseMod = (raisedStat == Stat::SPECIAL_DEFENSE) ? 1.1 : (loweredStat == Stat::SPECIAL_DEFENSE) ? 0.9 : 1.0;
    double speedMod = (raisedStat == Stat::SPEED) ? 1.1 : (loweredStat == Stat::SPEED) ? 0.9 : 1.0;

    // 나머지 능력치 계산
    attack_ = floor((floor(0.01 * (2 * speciesData.attack + ivs_.at(Stat::ATTACK) + floor(0.25 * evs_.at(Stat::ATTACK))) * level_) + 5) * attackMod);
    defense_ = floor((floor(0.01 * (2 * speciesData.defense + ivs_.at(Stat::DEFENSE) + floor(0.25 * evs_.at(Stat::DEFENSE))) * level_) + 5) * defenseMod);
    specialAttack_ = floor((floor(0.01 * (2 * speciesData.sp_attack + ivs_.at(Stat::SPECIAL_ATTACK) + floor(0.25 * evs_.at(Stat::SPECIAL_ATTACK))) * level_) + 5) * spAttackMod);
    specialDefense_ = floor((floor(0.01 * (2 * speciesData.sp_defense + ivs_.at(Stat::SPECIAL_DEFENSE) + floor(0.25 * evs_.at(Stat::SPECIAL_DEFENSE))) * level_) + 5) * spDefenseMod);
    speed_ = floor((floor(0.01 * (2 * speciesData.speed + ivs_.at(Stat::SPEED) + floor(0.25 * evs_.at(Stat::SPEED))) * level_) + 5) * speedMod);
}

void Pokemon::LevelUp() {
    level_++;
    std::cout << GetName() << "의 레벨이 " << level_ << "(으)로 올랐다!" << std::endl;
    RecalculateStats();
    // CheckForNewMoves(); // TODO: DataManager와 연동하여 구현
    // CheckForEvolution(); // TODO: DataManager와 연동하여 구현
}

// 이 함수들은 DataManager가 완성된 후, 그곳의 데이터를 이용해 구현해야 합니다.
void Pokemon::CheckForEvolution() {}
void Pokemon::CheckForNewMoves() {}