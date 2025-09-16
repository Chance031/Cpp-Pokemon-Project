#include "Pokemon.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "DataManager.h"
#include "Move.h"
#include "StringUtils.h"

// =================================================================
// ������
// =================================================================

Pokemon::Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual)
	: species_(&species), individual_(individual)
{
	EnsureMapsAreInitialized();

	DetermineActiveAbility();	// ���ϸ��� Ư���� ����
	RecalculateStats();			// �ɷ�ġ ���
	currentHp_ = maxHp_;		// ���� HP�� �ִ� HP�� �����ϰ� ����

	// ��� ���� ��ũ�� 0���� �ʱ�ȭ
	statStages_[Stat::ATTACK] = 0;
	statStages_[Stat::DEFENSE] = 0;
	statStages_[Stat::SPECIAL_ATTACK] = 0;
	statStages_[Stat::SPECIAL_DEFENSE] = 0;
	statStages_[Stat::SPEED] = 0;
	statStages_[Stat::ACCURACY] = 0;
	statStages_[Stat::EVASION] = 0;
}

// =================================================================
// ���� ��ȸ (Getters)
// =================================================================

const std::string& Pokemon::GetName() const
{
	// �г����� ������� ������ �г�����, ��������� ������ �ѱ� �̸��� ��ȯ
	if (!individual_.nickname.empty())
	{
		return individual_.nickname;
	}
	return species_->name_kr;
}

int Pokemon::GetStat(Stat stat) const
{
	// �ʿ��� �ش� �ɷ�ġ�� ã�� ��ȯ. ���ٸ� 0�� ��ȯ
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
	return 0; // ��ȭ�� ������ 0
}

// =================================================================
// ���� Ȯ�� (Checkers)
// =================================================================

bool Pokemon::HasVolatileStatus(VolatileStatus status) const
{
	// �Ͻ��� �����̻� ��Ͽ� �ش� ���°� �ִ��� Ȯ��
	auto it = std::find(individual_.volatileStatuses.begin(), individual_.volatileStatuses.end(), status);
	return it != individual_.volatileStatuses.end();
}

// =================================================================
// ���� ���� (Setters & Modifiers)
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
		//std::cout << GetName() << "��(��) " << moveData.name_kr << "��(��) �����!" << std::endl;
	}
	else
	{
		// TODO: ����� 4�� �� á�� ��, ���� ����� �ذ� ����� �����ϴ� ����
		std::cout << GetName() << "��(��) ����� �� �̻� ��� �� ����!" << std::endl;
	}
}

void Pokemon::Evolve(const PokemonSpecies& newSpecies)
{
	std::cout << "���!? " << GetName() << "�� ���°�...!" << std::endl;

	// ���ϸ��� ������ ���ο� �������� ��ü
	species_ = &newSpecies;

	std::cout << GetName() << "��(��) ��ȭ�ߴ�!" << std::endl;

	RecalculateStats();	// ��ȭ �� �ɷ�ġ ����
	Heal(maxHp_);		// HP�� ��� ȸ��
}

void Pokemon::SetPrimaryStatus(StatusCondition newStatus)
{
	individual_.primaryStatus = newStatus;
}

void Pokemon::AddVolatileStatus(VolatileStatus newStatus)
{
	// �̹� �ش� ���¿� �ɷ����� �ʴٸ� �߰�
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
	std::cout << GetName() << "���� �̻��ѻ����� ����ߴ�!" << std::endl;
	if (individual_.level < 100)
	{
		// TODO: ���� ���� �ʿ� ����ġ�� ���ߴ� ����
		LevelUpResult result = LevelUp();
		// TODO: result�� �̿��� ��ȭ �� ��� �н� ó�� (main �Լ��� ������ ����)
	}
}

void Pokemon::ApplyStatStageChange(Stat stat, int stages)
{
	// statStages_ �ʿ� �ش� ������ ������ 0���� �ʱ�ȭ
	if (statStages_.find(stat) == statStages_.end())
	{
		statStages_[stat] = 0;
	}

	statStages_[stat] += stages;

	// �ɷ�ġ ��ũ�� -6 ~ +6 ���̸� ��� �� ����
	if (statStages_[stat] > 6) statStages_[stat] = 6;
	if (statStages_[stat] < -6) statStages_[stat] = -6;

	// TODO: ���⿡ "OO�� XX�� ��������/�ö󰬴�!" �޽��� ��� ���� �߰� ����
}

void Pokemon::ResetStatStages()
{
	// ��� ���� ��ũ�� 0���� �ʱ�ȭ
	statStages_[Stat::ATTACK] = 0;
	statStages_[Stat::DEFENSE] = 0;
	statStages_[Stat::SPECIAL_ATTACK] = 0;
	statStages_[Stat::SPECIAL_DEFENSE] = 0;
	statStages_[Stat::SPEED] = 0;
	statStages_[Stat::ACCURACY] = 0;
	statStages_[Stat::EVASION] = 0;
}

// =================================================================
// ���� (Growth)
// =================================================================

void Pokemon::AddExp(int amount)
{
	if (individual_.level >= 100) return;

	individual_.currentExp += amount;
	std::cout << GetName() << "��(��) " << amount << "�� ����ġ�� �����!" << std::endl;

	// TODO: �ʿ� ����ġ�� �����͸� DataManager���� �����ͼ� ������ ó��
	// while (individual_.currentExp >= GetRequiredExpForNextLevel(...)) { ... }
}

// =================================================================
// ����� ���� (Internal Logic)
// =================================================================

LevelUpResult Pokemon::LevelUp()
{
	LevelUpResult result;
	if (individual_.level >= 100) return result;

	individual_.level++;
	std::cout << GetName() << "�� ������ " << individual_.level << "(��)�� �ö���!" << std::endl;

	RecalculateStats();

	// ��ȭ ���� Ȯ��
	if (species_->evolutionLevel > 0 && individual_.level >= species_->evolutionLevel)
	{
		result.evolutionId = species_->evolutionTargetId;
	}

	// ��� ���� ���� Ȯ��
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
	// HP ���
	int baseHp = species_->baseStats.at(Stat::HP);
	int ivHp = individual_.ivs.at(Stat::HP);
	int evHp = individual_.evs.at(Stat::HP);
	maxHp_ = static_cast<int>(floor(0.01 * (2 * baseHp + ivHp + floor(0.25 * evHp)) * individual_.level) + individual_.level + 10);
	finalStats_[Stat::HP] = maxHp_;

	// TODO: ���� ���� ������ DataManager���� ���������� ����
	auto natureMods = StringUtils::GetNatureStatMods(individual_.nature);
	Stat raisedStat = natureMods.first;
	Stat loweredStat = natureMods.second;

	// ������ ���� ���
	for (auto const& [stat, baseValue] : species_->baseStats)
	{
		if (stat == Stat::HP) continue;

		double natureMod = 1.0;
		if (stat == raisedStat) natureMod = 1.1;
		if (stat == loweredStat) natureMod = 0.9;

		finalStats_[stat] = CalculateStatInternal(stat, natureMod);
	}

	// ü���� �ִ� ü���� ���� �ʵ��� ����
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
	// TODO: ������ Ư��, �Ϲ� Ư�� 2 ���� ����� ���� ���� �߰�

	// 1. ���� �����Ϳ��� Ư�� 'ID'�� ���� �����ɴϴ�.
	int abilityId = species_->ability1;
	if (abilityId == 0) // Ư���� ���� ��� (ID�� 0�� ���)
	{
		activeAbility_ = &DataManager::GetInstance().GetAbilityData(0);
		return;
	}

	// 2. DataManager�� ���� ID�� AbilityData�� ã�� �����͸� �����մϴ�.
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
		// .try_emplace()�� �ش� Ű�� ���� ���� ���� �߰��ϴ� ���� C++17 ����Դϴ�.
		individual_.ivs.try_emplace(stat, 0);
		individual_.evs.try_emplace(stat, 0);
	}
}
