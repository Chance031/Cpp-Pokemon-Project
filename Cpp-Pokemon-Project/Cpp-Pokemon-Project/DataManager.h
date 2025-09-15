#pragma once

#include <map>
#include <vector>
#include <string>

#include "AbilityData.h"
#include "PokemonData.h"
#include "MoveData.h"
#include "Enums.h"

class DataManager
{
public:
	// �̱��� �ν��Ͻ��� �����ɴϴ�.
	static DataManager& GetInstance();

	// ��� CSV �����͸� �ε��մϴ�.
	void LoadAllData();

	// ID�� ���� ���� �����͸� �������� �Լ���
	const PokemonSpecies& GetPokemonSpecies(int id) const;
	const MoveData& GetMoveData(int id) const;
	const MoveEffectData& GetMoveEffectData(int id) const;
	const float GetTypeMatchup(Type attackingType, Type defendingType) const;
	const AbilityData& GetAbilityData(int id) const;

private:
	// �ܺο��� ���� ����
	DataManager() = default;

	// =================================================================
	// �����(private) �ε� �Լ� ����
	// =================================================================
	void LoadPokemonSpecies(const std::string& filePath);
	void LoadMoves(const std::string& filePath);
	void LoadMoveEffects(const std::string& filePath);
	void LoadTypeMatchups(const std::string& filePath);
	void LoadAbilities(const std::string& filePath);

	// �ε�� �����͸� ������ ������ ���̽�
	std::map<int, PokemonSpecies> speciesDatabase_;
	std::map<int, MoveData> moveDatabase_;
	std::map<int, MoveEffectData> moveEffectDatabase_;
	std::map<Type, std::map<Type, float>> typeMatchups_;
	std::map<int, AbilityData> abilityDatabase_;
};