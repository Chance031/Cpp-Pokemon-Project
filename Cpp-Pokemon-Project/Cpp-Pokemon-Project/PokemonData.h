#pragma once

#include <string>
#include <map>
#include <vector>

#include "Enums.h"

// --- �����(Learnset) ���� ����ü ---
struct LevelUpMove
{
	int level = 0;					// ����� ���� Ư�� ����
	int moveId = 0;					// ��� ����� ID
};

// --- ������ ��� ���� ����ü ---
struct LevelUpResult
{
	int evolutionId = 0;			// ��ȭ�� ���ϸ��� ���� ID
	std::vector<int> newMoveIds;	// �ش� ���������� ���� �� ��� ID ���
};

// --- ���ġ(EV) �����͸� ���� ����ü ---
struct EVYield
{
	Stat stat = Stat::HP;
	int amount = 0;
};

// =================================================================
// ���ϸ� '����'�� �Һ� ������ (CSV���� �ε�)
// =================================================================
struct PokemonSpecies
{
	// --- �⺻ ���� ---
	int id = 0;												// ���� ��ȣ
	std::string name_en = "";								// ���ϸ� �̸�(en)
	std::string name_kr = "";								// ���� �̸�(kr)

	// --- �ɷ�ġ ---
	std::map<Stat, int> baseStats{};						// H, A, B, C, D, S

	// --- ��ȭ ---
	int evolutionLevel = 0;									// ��ȭ ����
	int evolutionTargetId = 0;								// ��ȭ Ÿ��

	// --- Ÿ�� ---
	Type type1 = Type::NONE;								// ���ϸ� Ÿ��1
	Type type2 = Type::NONE;								// ���ϸ� Ÿ��2

	// --- ���� �� �� ---
	float male_ratio_pct = -1.0f;							// ����
	std::string egg_group_1 = "NONE";						// �˱׷�1
	std::string egg_group_2 = "NONE";						// �˱׷�2
	int hatch_counter = 0;									// ��ȭ ī��Ʈ

	// --- Ư�� ---
	std::string ability1 = "NONE";							// Ư��1
	std::string ability2 = "NONE";							// Ư��2
	std::string hidden_ability = "NONE";					// ������ Ư��

	// --- ���� ���� ������ ---
	ExpGroup exp_group = ExpGroup::MEDIUM_FAST;				// ����ġ �׷�
	int catch_rate = 0;										// ��ȹ��
	int base_exp_yield = 0;									// ���� ����ġ
	std::vector<EVYield> ev_yields{};						// �����߷��� �� �ִ� ���ġ ���
	int base_friendship = 0;								// ���� ģ�е�

	// --- ���� ���� ---
	std::string category_en = "";							// �з�(en)
	std::string category_kr = "";							// �з�(kr)
	float height_m = 0.0f;									// ����
	float weight_kg =0.0f;									// ü��
	std::string pokedex_entry_en = "";						// ���� ����(en)
	std::string pokedex_entry_kr = "";						// ���� ����(kr)

	// --- ����� ---
	std::vector<LevelUpMove> learnset{};					// ������ �� ���� ��� ���
};

// =================================================================
// ���ϸ� '��ü'�� ���� ������ (���� ������ ����)
// =================================================================
struct PokemonIndividual
{
	int level = 1;											// ���� ����
	Nature nature = Nature::HARDY;							// ����
	std::map<Stat, int> ivs{};								// ��ü��
	std::map<Stat, int> evs{};								// ���ġ
	int currentExp = 0;										// ���� �������� ���� ����ġ

	std::string nickname = "";								// �г���
	Gender gender = Gender::GENDERLESS;						// ����

	StatusCondition primaryStatus = StatusCondition::NONE;	// ��, ���� �� ������ ������ ���ӵǴ� �ֿ� ���� �̻� (�ϳ��� ����)
	std::vector<VolatileStatus> volatileStatuses{};			// ȥ��, ���Ѹ��� �� ���� �߿��� ����Ǵ� �Ͻ��� ���µ� (���� �� ����)
};