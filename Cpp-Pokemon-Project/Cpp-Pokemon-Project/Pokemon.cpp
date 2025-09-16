#include "Pokemon.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "DataManager.h"
#include "Move.h"
#include "StringUtils.h"

// =================================================================
// 생성자
// =================================================================

Pokemon::Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual)
	: species_(&species), individual_(individual)
{
	EnsureMapsAreInitialized();

	DetermineActiveAbility();	// 포켓몬의 특성을 결정
	RecalculateStats();			// 능력치 계산
	currentHp_ = maxHp_;		// 현재 HP를 최대 HP와 동일하게 설정

	// 모든 스탯 랭크를 0으로 초기화
	statStages_[Stat::ATTACK] = 0;
	statStages_[Stat::DEFENSE] = 0;
	statStages_[Stat::SPECIAL_ATTACK] = 0;
	statStages_[Stat::SPECIAL_DEFENSE] = 0;
	statStages_[Stat::SPEED] = 0;
	statStages_[Stat::ACCURACY] = 0;
	statStages_[Stat::EVASION] = 0;
}

// =================================================================
// 정보 조회 (Getters)
// =================================================================

const std::string& Pokemon::GetName() const
{
	// 닉네임이 비어있지 않으면 닉네임을, 비어있으면 종족의 한글 이름을 반환
	if (!individual_.nickname.empty())
	{
		return individual_.nickname;
	}
	return species_->name_kr;
}

int Pokemon::GetStat(Stat stat) const
{
	// 맵에서 해당 능력치를 찾아 반환. 없다면 0을 반환
	auto it = finalStats_.find(stat);

	if (it != finalStats_.end())
	{
		return it->second;
	}
	return 0;
}

const std::vector<Move>& Pokemon::GetMoveset() const
{
	return moveset_;
}

int Pokemon::GetStatStage(Stat stat) const
{
	if (statStages_.find(stat) != statStages_.end())
	{
		return statStages_.at(stat);
	}
	return 0; // 변화가 없으면 0
}

// =================================================================
// 상태 확인 (Checkers)
// =================================================================

bool Pokemon::HasVolatileStatus(VolatileStatus status) const
{
	// 일시적 상태이상 목록에 해당 상태가 있는지 확인
	auto it = std::find(individual_.volatileStatuses.begin(), individual_.volatileStatuses.end(), status);
	return it != individual_.volatileStatuses.end();
}

// =================================================================
// 상태 변경 (Setters & Modifiers)
// =================================================================

void Pokemon::TakeDamage(int damage)
{
	currentHp_ -= damage;
	if (currentHp_ < 0)
	{
		currentHp_ = 0;
	}
}

void Pokemon::Heal(int amount)
{
	currentHp_ += amount;
	if (currentHp_ > maxHp_)
	{
		currentHp_ = maxHp_;
	}
}

void Pokemon::LearnMove(const MoveData& moveData)
{
	if (moveset_.size() < 4)
	{
		moveset_.emplace_back(moveData);
		//std::cout << GetName() << "은(는) " << moveData.name_kr << "을(를) 배웠다!" << std::endl;
	}
	else
	{
		// TODO: 기술이 4개 꽉 찼을 때, 기존 기술을 잊고 배울지 선택하는 로직
		std::cout << GetName() << "은(는) 기술을 더 이상 배울 수 없다!" << std::endl;
	}
}

void Pokemon::Evolve(const PokemonSpecies& newSpecies)
{
	std::cout << "어라!? " << GetName() << "의 상태가...!" << std::endl;

	// 포켓몬의 종족을 새로운 종족으로 교체
	species_ = &newSpecies;

	std::cout << GetName() << "으(로) 진화했다!" << std::endl;

	RecalculateStats();	// 진화 후 능력치 재계산
	Heal(maxHp_);		// HP를 모두 회복
}

void Pokemon::SetPrimaryStatus(StatusCondition newStatus)
{
	individual_.primaryStatus = newStatus;
}

void Pokemon::AddVolatileStatus(VolatileStatus newStatus)
{
	// 이미 해당 상태에 걸려있지 않다면 추가
	if (!HasVolatileStatus(newStatus))
	{
		individual_.volatileStatuses.push_back(newStatus);
	}
}

void Pokemon::ClearVolatileStatuses()
{
	individual_.volatileStatuses.clear();
}

void Pokemon::UseRareCandy()
{
	std::cout << GetName() << "에게 이상한사탕을 사용했다!" << std::endl;
	if (individual_.level < 100)
	{
		// TODO: 다음 레벨 필요 경험치로 맞추는 로직
		LevelUpResult result = LevelUp();
		// TODO: result를 이용해 진화 및 기술 학습 처리 (main 함수와 동일한 로직)
	}
}

void Pokemon::ApplyStatStageChange(Stat stat, int stages)
{
	// statStages_ 맵에 해당 스탯이 없으면 0으로 초기화
	if (statStages_.find(stat) == statStages_.end())
	{
		statStages_[stat] = 0;
	}

	statStages_[stat] += stages;

	// 능력치 랭크는 -6 ~ +6 사이를 벗어날 수 없음
	if (statStages_[stat] > 6) statStages_[stat] = 6;
	if (statStages_[stat] < -6) statStages_[stat] = -6;

	// TODO: 여기에 "OO의 XX가 떨어졌다/올라갔다!" 메시지 출력 로직 추가 가능
}

void Pokemon::ResetStatStages()
{
	// 모든 스탯 랭크를 0으로 초기화
	statStages_[Stat::ATTACK] = 0;
	statStages_[Stat::DEFENSE] = 0;
	statStages_[Stat::SPECIAL_ATTACK] = 0;
	statStages_[Stat::SPECIAL_DEFENSE] = 0;
	statStages_[Stat::SPEED] = 0;
	statStages_[Stat::ACCURACY] = 0;
	statStages_[Stat::EVASION] = 0;
}

// =================================================================
// 성장 (Growth)
// =================================================================

void Pokemon::AddExp(int amount)
{
	if (individual_.level >= 100) return;

	individual_.currentExp += amount;
	std::cout << GetName() << "은(는) " << amount << "의 경험치를 얻었다!" << std::endl;

	// TODO: 필요 경험치량 데이터를 DataManager에서 가져와서 레벨업 처리
	// while (individual_.currentExp >= GetRequiredExpForNextLevel(...)) { ... }
}

// =================================================================
// 비공개 로직 (Internal Logic)
// =================================================================

LevelUpResult Pokemon::LevelUp()
{
	LevelUpResult result;
	if (individual_.level >= 100) return result;

	individual_.level++;
	std::cout << GetName() << "의 레벨이 " << individual_.level << "(으)로 올랐다!" << std::endl;

	RecalculateStats();

	// 진화 조건 확인
	if (species_->evolutionLevel > 0 && individual_.level >= species_->evolutionLevel)
	{
		result.evolutionId = species_->evolutionTargetId;
	}

	// 기술 습득 조건 확인
	for (const auto& moveInfo : species_->learnset)
	{
		if (moveInfo.level == individual_.level)
		{
			result.newMoveIds.push_back(moveInfo.moveId);
		}
	}
	return result;
}

void Pokemon::RecalculateStats()
{
	// HP 계산
	int baseHp = species_->baseStats.at(Stat::HP);
	int ivHp = individual_.ivs.at(Stat::HP);
	int evHp = individual_.evs.at(Stat::HP);
	maxHp_ = static_cast<int>(floor(0.01 * (2 * baseHp + ivHp + floor(0.25 * evHp)) * individual_.level) + individual_.level + 10);
	finalStats_[Stat::HP] = maxHp_;

	// TODO: 성격 보정 로직을 DataManager에서 가져오도록 수정
	auto natureMods = StringUtils::GetNatureStatMods(individual_.nature);
	Stat raisedStat = natureMods.first;
	Stat loweredStat = natureMods.second;

	// 나머지 스탯 계산
	for (auto const& [stat, baseValue] : species_->baseStats)
	{
		if (stat == Stat::HP) continue;

		double natureMod = 1.0;
		if (stat == raisedStat) natureMod = 1.1;
		if (stat == loweredStat) natureMod = 0.9;

		finalStats_[stat] = CalculateStatInternal(stat, natureMod);
	}

	// 체력이 최대 체력을 넘지 않도록 조정
	if (currentHp_ > maxHp_)
	{
		currentHp_ = maxHp_;
	}
}

int Pokemon::CalculateStatInternal(Stat stat, double natureMod) const
{
	int base = species_->baseStats.at(stat);
	int iv = individual_.ivs.at(stat);
	int ev = individual_.evs.at(stat);

	return static_cast<int>(floor((floor(0.01 * (2 * base + iv + floor(0.25 * ev)) * individual_.level) + 5) * natureMod));
}

void Pokemon::DetermineActiveAbility()
{
	// TODO: 숨겨진 특성, 일반 특성 2 등을 고려한 랜덤 로직 추가

	// 1. 종족 데이터에서 특성 'ID'를 직접 가져옵니다.
	int abilityId = species_->ability1;
	if (abilityId == 0) // 특성이 없는 경우 (ID가 0일 경우)
	{
		activeAbility_ = &DataManager::GetInstance().GetAbilityData(0);
		return;
	}

	// 2. DataManager를 통해 ID로 AbilityData를 찾아 포인터를 저장합니다.
	activeAbility_ = &DataManager::GetInstance().GetAbilityData(abilityId);
}

void Pokemon::EnsureMapsAreInitialized()
{
	const std::vector<Stat> coreStats = {
		Stat::HP, Stat::ATTACK, Stat::DEFENSE,
		Stat::SPECIAL_ATTACK, Stat::SPECIAL_DEFENSE, Stat::SPEED
	};

	for (const auto& stat : coreStats)
	{
		// .try_emplace()는 해당 키가 없을 때만 값을 추가하는 편리한 C++17 기능입니다.
		individual_.ivs.try_emplace(stat, 0);
		individual_.evs.try_emplace(stat, 0);
	}
}
