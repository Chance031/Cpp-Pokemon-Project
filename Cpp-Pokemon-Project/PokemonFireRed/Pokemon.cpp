#include "framework.h"
#include "Pokemon.h"
#include "DataManager.h"
#include "StringUtils.h"
#include <cmath> // floor 함수 사용

Pokemon::Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual)
	: species_(&species), individual_(individual)
{
	// 기술폭(learnset)을 확인하여 현재 레벨에서 배울 수 있는 기술을 자동으로 추가
	for (const auto& learnableMove : species_->learnset)
	{
		if (learnableMove.level > 0 && learnableMove.level <= individual_.level)
		{
			// 최대 4개까지만 기술을 배움 (가장 높은 레벨의 기술부터)
			if (moveset_.size() < 4)
			{
				LearnMove(DataManager::GetInstance().GetMoveData(learnableMove.moveId));
			}
		}
	}

	// 특성 설정 (숨겨진 특성은 일단 제외)
	ability_ = &DataManager::GetInstance().GetAbilityData(species_->ability1);

	// 모든 능력치 랭크를 0으로 초기화
	ResetStatStages();
	// 최초 능력치 계산
	RecalculateStats();
}

const std::string& Pokemon::GetName() const
{
	// 닉네임이 있으면 닉네임을, 없으면 종족의 한국어 이름을 반환
	return individual_.nickname.empty() ? species_->name_kr : individual_.nickname;
}

int Pokemon::GetLevel() const
{
	return individual_.level;
}

int Pokemon::GetCurrentHP() const
{
	return currentHP_;
}

int Pokemon::GetMaxHP() const
{
	return maxHP_;
}

StatusCondition Pokemon::GetPrimaryStatus() const
{
	return individual_.primaryStatus;
}

const std::vector<Move>& Pokemon::GetMoveset() const
{
	return moveset_;
}

std::vector<Move>& Pokemon::GetMovesetForModify()
{
	return moveset_;
}

const AbilityData* Pokemon::GetAbility() const
{
	return ability_;
}

Type Pokemon::GetType1() const
{
	return species_->type1;
}

Type Pokemon::GetType2() const
{
	return species_->type2;
}

bool Pokemon::IsFainted() const
{
	return currentHP_ <= 0;
}

void Pokemon::TakeDamage(int damage)
{
	currentHP_ -= damage;
	if (currentHP_ < 0)
	{
		currentHP_ = 0;
	}
}

LevelUpResult Pokemon::AddExp(int amount)
{
	// ... (경험치 및 레벨업 로직, 현재는 생략)
	return LevelUpResult();
}

void Pokemon::LearnMove(const MoveData& moveData)
{
	if (moveset_.size() < 4)
	{
		moveset_.emplace_back(moveData);
	}
}

void Pokemon::SetPrimaryStatus(StatusCondition newStatus)
{
	individual_.primaryStatus = newStatus;
}

void Pokemon::ApplyStatStageChange(Stat stat, int stages)
{
	statStages_[stat] += stages;
	// 랭크는 -6 ~ +6 범위를 벗어날 수 없음
	if (statStages_[stat] > 6) statStages_[stat] = 6;
	if (statStages_[stat] < -6) statStages_[stat] = -6;
}

void Pokemon::ResetStatStages()
{
	statStages_[Stat::ATTACK] = 0;
	statStages_[Stat::DEFENSE] = 0;
	statStages_[Stat::SPECIAL_ATTACK] = 0;
	statStages_[Stat::SPECIAL_DEFENSE] = 0;
	statStages_[Stat::SPEED] = 0;
	statStages_[Stat::ACCURACY] = 0;
	statStages_[Stat::EVASION] = 0;
}

int Pokemon::GetStat(Stat stat) const
{
	if (stat == Stat::HP)
	{
		return maxHP_;
	}
	// map의 at() 함수는 해당 키의 값을 반환. 없으면 예외 발생.
	return currentStats_.at(stat);
}

int Pokemon::GetStatStage(Stat stat) const
{
	return statStages_.at(stat);
}

void Pokemon::RecalculateStats()
{
	int level = individual_.level;

	// --- 1. HP 계산 ---
	// HP = floor( ( (종족값 * 2 + 개체값 + 노력치/4) * 레벨 ) / 100 ) + 레벨 + 10
	int hpBase = species_->baseStats.at(Stat::HP);
	int hpIV = individual_.ivs[Stat::HP];
	int hpEV = individual_.evs[Stat::HP];
	maxHP_ = static_cast<int>(floor(((hpBase * 2 + hpIV + floor(hpEV / 4.0)) * level) / 100.0)) + level + 10;
	currentHP_ = maxHP_; // 재계산 시 HP 완전 회복

	// --- 2. 나머지 능력치 (공격, 방어, 특공, 특방, 스피드) 계산 ---
	// Stat = floor( ( floor( ( (종족값*2 + 개체값 + 노력치/4) * 레벨 ) / 100 ) + 5 ) * 성격보정 )
	auto calculateOtherStat = [&](Stat stat) {
		int base = species_->baseStats.at(stat);
		int iv = individual_.ivs[stat];
		int ev = individual_.evs[stat];
		int calculatedStat = static_cast<int>(floor(((base * 2 + iv + floor(ev / 4.0)) * level) / 100.0)) + 5;

		// 성격 보정 적용
		auto natureMods = StringUtils::GetNatureStatMods(individual_.nature);
		if (natureMods.first == stat) { // 오르는 성격
			calculatedStat = static_cast<int>(floor(calculatedStat * 1.1));
		}
		if (natureMods.second == stat) { // 내려가는 성격
			calculatedStat = static_cast<int>(floor(calculatedStat * 0.9));
		}
		return calculatedStat;
		};

	currentStats_[Stat::ATTACK] = calculateOtherStat(Stat::ATTACK);
	currentStats_[Stat::DEFENSE] = calculateOtherStat(Stat::DEFENSE);
	currentStats_[Stat::SPECIAL_ATTACK] = calculateOtherStat(Stat::SPECIAL_ATTACK);
	currentStats_[Stat::SPECIAL_DEFENSE] = calculateOtherStat(Stat::SPECIAL_DEFENSE);
	currentStats_[Stat::SPEED] = calculateOtherStat(Stat::SPEED);
}