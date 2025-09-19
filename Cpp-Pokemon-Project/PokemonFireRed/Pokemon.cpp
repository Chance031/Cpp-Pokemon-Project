#include "framework.h"
#include "Pokemon.h"
#include "DataManager.h"
#include "StringUtils.h"
#include <cmath> // floor �Լ� ���

Pokemon::Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual)
	: species_(&species), individual_(individual)
{
	// �����(learnset)�� Ȯ���Ͽ� ���� �������� ��� �� �ִ� ����� �ڵ����� �߰�
	for (const auto& learnableMove : species_->learnset)
	{
		if (learnableMove.level > 0 && learnableMove.level <= individual_.level)
		{
			// �ִ� 4�������� ����� ��� (���� ���� ������ �������)
			if (moveset_.size() < 4)
			{
				LearnMove(DataManager::GetInstance().GetMoveData(learnableMove.moveId));
			}
		}
	}

	// Ư�� ���� (������ Ư���� �ϴ� ����)
	ability_ = &DataManager::GetInstance().GetAbilityData(species_->ability1);

	// ��� �ɷ�ġ ��ũ�� 0���� �ʱ�ȭ
	ResetStatStages();
	// ���� �ɷ�ġ ���
	RecalculateStats();
}

const std::string& Pokemon::GetName() const
{
	// �г����� ������ �г�����, ������ ������ �ѱ��� �̸��� ��ȯ
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
	// ... (����ġ �� ������ ����, ����� ����)
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
	// ��ũ�� -6 ~ +6 ������ ��� �� ����
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
	// map�� at() �Լ��� �ش� Ű�� ���� ��ȯ. ������ ���� �߻�.
	return currentStats_.at(stat);
}

int Pokemon::GetStatStage(Stat stat) const
{
	return statStages_.at(stat);
}

void Pokemon::RecalculateStats()
{
	int level = individual_.level;

	// --- 1. HP ��� ---
	// HP = floor( ( (������ * 2 + ��ü�� + ���ġ/4) * ���� ) / 100 ) + ���� + 10
	int hpBase = species_->baseStats.at(Stat::HP);
	int hpIV = individual_.ivs[Stat::HP];
	int hpEV = individual_.evs[Stat::HP];
	maxHP_ = static_cast<int>(floor(((hpBase * 2 + hpIV + floor(hpEV / 4.0)) * level) / 100.0)) + level + 10;
	currentHP_ = maxHP_; // ���� �� HP ���� ȸ��

	// --- 2. ������ �ɷ�ġ (����, ���, Ư��, Ư��, ���ǵ�) ��� ---
	// Stat = floor( ( floor( ( (������*2 + ��ü�� + ���ġ/4) * ���� ) / 100 ) + 5 ) * ���ݺ��� )
	auto calculateOtherStat = [&](Stat stat) {
		int base = species_->baseStats.at(stat);
		int iv = individual_.ivs[stat];
		int ev = individual_.evs[stat];
		int calculatedStat = static_cast<int>(floor(((base * 2 + iv + floor(ev / 4.0)) * level) / 100.0)) + 5;

		// ���� ���� ����
		auto natureMods = StringUtils::GetNatureStatMods(individual_.nature);
		if (natureMods.first == stat) { // ������ ����
			calculatedStat = static_cast<int>(floor(calculatedStat * 1.1));
		}
		if (natureMods.second == stat) { // �������� ����
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