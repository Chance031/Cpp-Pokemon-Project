#include "Pokemon.h"

#include <cmath>
#include <iostream>

// �ӽ� Nature ���� �Լ� (������ DataManager�� ���� Ŭ������ ���)
std::pair<Stat, Stat> GetNatureStatMods(Nature nature) {
	if (nature == Nature::MODEST) {
		return { Stat::SPECIAL_ATTACK, Stat::ATTACK };
	}
	return { Stat::HP, Stat::HP }; // ���� ����
}

// ������
Pokemon::Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual)
	: species_(species), individual_(individual)
{
	// ���� ������ ���� �ɷ�ġ�� ���
	RecalculateStats();
	// ���� HP�� �ִ� HP�� �����ϰ� ����
	currentHp_ = finalStats_.at(Stat::HP);

	std::cout << species_.name << " (Lv. " << individual_.level << ") ���� �Ϸ�!" << std::endl;
}

int Pokemon::GetStat(Stat stat) const
{
	// �ʿ��� �ش� �ɷ�ġ�� ã�� ��ȯ. ������ 0�� ��ȯ (������ġ)
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

// ����ġ ȹ�� �� ���� �� ó��
void Pokemon::AddExp(int amount)
{
	// TODO: �������� �ʿ��� ����ġ���� ����ϴ� ���� �߰�
	// (�� ������ ExpGroup�� ���� �޶�����, ���� ��ü ��Ģ�̹Ƿ� Pokemon Ŭ���� �ܺο� �ִ� ���� ����)
	individual_.currentExp += amount;
	std::cout << species_.name << "��(��) " << amount << "�� ����ġ�� �����!" << std::endl;
	// if (individual.currentExp >= requiredExpForNextLevel) { LevelUp(); }
}

void Pokemon::LevelUp()
{
	individual_.level++;
	std::cout << species_.name << "�� ������ " << individual_.level << "(��)�� �ö���!" << std::endl;
	RecalculateStats(); // ������ �ö����Ƿ� �ɷ�ġ�� �ٽ� ���
}

// ���� �ɷ�ġ ���
void Pokemon::RecalculateStats()
{
	// HP ���
	int baseHp = species_.baseStats.at(Stat::HP);	// ������
	int ivHp = individual_.ivs.at(Stat::HP);		// ��ü��
	int evHp = individual_.evs.at(Stat::HP);		// ��°�
	
	// HP �Ǽ�ġ ���
	finalStats_[Stat::HP] = floor(0.01 * (2 * baseHp + ivHp + floor(0.25 * evHp)) * individual_.level) + individual_.level + 10;

	// ���� ���� ���
	auto natureMods = GetNatureStatMods(individual_.nature);
	Stat raisedStat = natureMods.first;
	Stat loweredStat = natureMods.second;

	// ������ ���� ���

	for (auto const& [stat, baseValue] : species_.baseStats)
	{
		if (stat == Stat::HP) continue;

		double natureMode = 1.0;
		if (stat == raisedStat) natureMode = 1.1;
		if (stat == loweredStat) natureMode = 0.9;

		finalStats_[stat] = CalculateStatInternal(stat, natureMode);
	}
}


// HP �� �ɷ�ġ ��� ���� (���� �Լ�)
int Pokemon::CalculateStatInternal(Stat stat, double natureMod) const
{
	int base = species_.baseStats.at(stat);
	int iv = individual_.ivs.at(stat);
	int ev = individual_.evs.at(stat);

	return floor((floor(0.01 * (2 * base + iv + floor(0.25 * ev)) * individual_.level) + 5) * natureMod);
}

