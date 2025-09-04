#include "Pokemon.h"

#include <iostream>

Pokemon::Pokemon(int speciesId, int level)
	: speciesId_(speciesId), level_(level)
{
    // 개체값(IV)은 0~31 사이의 난수로 설정합니다. (지금은 예시로 31로 고정)
    ivs_[Stat::HP] = 31;
    ivs_[Stat::ATTACK] = 31;
    ivs_[Stat::DEFENSE] = 31;
    ivs_[Stat::SPECIAL_ATTACK] = 31;
    ivs_[Stat::SPECIAL_DEFENSE] = 31;
    ivs_[Stat::SPEED] = 31;

    // 노력치(EV)는 처음에는 모두 0입니다.
    evs_[Stat::HP] = 0;
    evs_[Stat::ATTACK] = 0;
    evs_[Stat::DEFENSE] = 0;
    evs_[Stat::SPECIAL_ATTACK] = 0;
    evs_[Stat::SPECIAL_DEFENSE] = 0;
    evs_[Stat::SPEED] = 0;

    // 성격(Nature)은 25가지 중 하나로 랜덤하게 설정합니다. (지금은 예시로 고정)
    nature_ = Nature::MODEST; // 조심 성격

    // 기타 변수 초기화
    currentExp_ = 0; // 경험치는 0부터 시작
    primaryStatus_ = StatusCondition::NONE; // 상태 이상 없음
    // ...

    // --- 마지막으로, 설정된 재료들로 첫 능력치를 계산합니다 ---
    RecalculateStats();

    // HP는 처음에는 가득 차 있습니다.
    currentHp_ = maxHp_;

    std::cout << "레벨 " << level_ << " 포켓몬 생성 완료!" << std::endl;
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
    // TODO: GDD의 '경험치 공식'에 따라 amount를 조절하는 로직 추가
    // (예: 상대 레벨, 트레이너전 보너스 등)
    currentExp_ += amount;
    std::cout << GetName() << "은(는) " << amount << "의 경험치를 얻었다!" << std::endl;

    // --- 레벨 업 체크 ---
    // 여러 번 레벨업 할 수 있으므로 while 루프로 체크합니다.
    while (true)
    {
        // DataManager에서 다음 레벨에 필요한 총 경험치를 가져온다고 가정
        // int requiredExp = DataManager::GetInstance()->GetTotalExpForLevel(level_ + 1, expGroup_);

        // 임시 필요 경험치 (실제로는 경험치 곡선 함수 사용)
        int requiredExp = (level_ + 1) * (level_ + 1) * (level_ + 1);

        if (currentExp_ >= requiredExp)
        {
            LevelUp(); // 필요 경험치를 넘었으면 레벨 업!
        }
        else
        {
            break; // 넘지 못하면 루프 종료
        }
    }
}

bool Pokemon::LearnMove(const Move& move)
{
    if (moveset_.size() < 4)
    {
        moveset_.push_back(move);
        return true; // 배우기 성공
    }

    // TODO: 4개를 초과했을 때, 기존 기술을 잊고 배울지 물어보는 UI 로직 호출

    return false; // 배우기 실패 (자리가 없음)
}

int Pokemon::GetStat(Stat stat) const
{
    switch (stat)
    {
    case Stat::HP: return maxHp_; // 현재 HP가 아닌 최대 HP를 반환하는 것이 일반적
    case Stat::ATTACK: return attack_;
    case Stat::DEFENSE: return defense_;
    case Stat::SPECIAL_ATTACK: return specialAttack_;
    case Stat::SPECIAL_DEFENSE: return specialDefense_;
    case Stat::SPEED: return speed_;
    default: return 0; // 예외 처리
    }
}

void Pokemon::RecalculateStats()
{
    // DataManager에서 이 포켓몬의 종족값(Base Stat)을 가져왔다고 가정합니다.
    // int baseHp = DataManager::GetInstance()->GetSpecies(speciesId_)->GetBaseStat(Stat::HP);
    // int baseAttack = ...;

    // --- 임시 종족값 (예: 이상해씨) ---
    int baseHp = 45;
    int baseAttack = 49;
    int baseDefense = 49;
    int baseSpecialAttack = 65;
    int baseSpecialDefense = 65;
    int baseSpeed = 45;

    // --- HP 계산 (GDD 공식) ---
    maxHp_ = floor(0.01 * (2 * baseHp + ivs_[Stat::HP] + floor(0.25 * evs_[Stat::HP])) * level_) + level_ + 10;

    // --- 나머지 능력치 계산 (GDD 공식) ---
    // 실제 구현에서는 Nature의 보정치(1.1, 1.0, 0.9)를 가져오는 로직이 필요합니다.
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
    std::cout << GetName() << "의 레벨이 " << level_ << "(으)로 올랐다!" << std::endl;

    // 1. 능력치 재계산 (가장 중요)
    RecalculateStats();

    // 2. 새로운 기술을 배우는지 확인
    CheckForNewMoves();

    // 3. 진화하는지 확인
    CheckForEvolution();
}

void Pokemon::CheckForEvolution()
{
}

void Pokemon::CheckForNewMoves()
{
}


