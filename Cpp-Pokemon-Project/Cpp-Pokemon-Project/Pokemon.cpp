#include "Pokemon.h"

#include <cmath>
#include <iostream>

// 임시 Nature 보정 함수 (원래는 DataManager나 별도 클래스가 담당)
std::pair<Stat, Stat> GetNatureStatMods(Nature nature) {
	if (nature == Nature::MODEST) {
		return { Stat::SPECIAL_ATTACK, Stat::ATTACK };
	}
	return { Stat::HP, Stat::HP }; // 보정 없음
}

// 생성자
Pokemon::Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual)
	: species_(species), individual_(individual)
{
	// 생성 시점에 최종 능력치를 계산
	RecalculateStats();
	// 현재 HP를 최대 HP와 동일하게 설정
	currentHp_ = finalStats_.at(Stat::HP);

	std::cout << species_.name << " (Lv. " << individual_.level << ") 생성 완료!" << std::endl;
}

int Pokemon::GetStat(Stat stat) const
{
	// 맵에서 해당 능력치를 찾아 반환. 없으면 0을 반환 (안전장치)
	auto it = finalStats_.find(stat);

	return (it != finalStats_.end()) ? it->second : 0;
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
	int maxHp = finalStats_.at(Stat::HP);
	if (currentHp_ > maxHp)
		currentHp_ = maxHp;
}

// 경험치 획득 및 레벨 업 처리
void Pokemon::AddExp(int amount)
{
	// TODO: 레벨업에 필요한 경험치량을 계산하는 로직 추가
	// (이 로직은 ExpGroup에 따라 달라지며, 게임 전체 규칙이므로 Pokemon 클래스 외부에 있는 것이 좋음)
	individual_.currentExp += amount;
	std::cout << species_.name << "은(는) " << amount << "의 경험치를 얻었다!" << std::endl;
	// if (individual.currentExp >= requiredExpForNextLevel) { LevelUp(); }
}

void Pokemon::LevelUp()
{
	individual_.level++;
	std::cout << species_.name << "의 레벨이 " << individual_.level << "(으)로 올랐다!" << std::endl;
	RecalculateStats(); // 레벨이 올랐으므로 능력치를 다시 계산
}

// 최종 능력치 계산
void Pokemon::RecalculateStats()
{
	// HP 계산
	int baseHp = species_.baseStats.at(Stat::HP);	// 종족값
	int ivHp = individual_.ivs.at(Stat::HP);		// 개체값
	int evHp = individual_.evs.at(Stat::HP);		// 노력값
	
	// HP 실수치 계산
	finalStats_[Stat::HP] = floor(0.01 * (2 * baseHp + ivHp + floor(0.25 * evHp)) * individual_.level) + individual_.level + 10;

	// 성격 보정 계산
	auto natureMods = GetNatureStatMods(individual_.nature);
	Stat raisedStat = natureMods.first;
	Stat loweredStat = natureMods.second;

	// 나머지 스탯 계산

	for (auto const& [stat, baseValue] : species_.baseStats)
	{
		if (stat == Stat::HP) continue;

		double natureMode = 1.0;
		if (stat == raisedStat) natureMode = 1.1;
		if (stat == loweredStat) natureMode = 0.9;

		finalStats_[stat] = CalculateStatInternal(stat, natureMode);
	}
}


// HP 외 능력치 계산 공식 (헬퍼 함수)
int Pokemon::CalculateStatInternal(Stat stat, double natureMod) const
{
	int base = species_.baseStats.at(stat);
	int iv = individual_.ivs.at(stat);
	int ev = individual_.evs.at(stat);

	return floor((floor(0.01 * (2 * base + iv + floor(0.25 * ev)) * individual_.level) + 5) * natureMod);
}

