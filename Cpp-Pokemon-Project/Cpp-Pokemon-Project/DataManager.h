#pragma once

#include <map>
#include <vector>
#include <string>

#include "PokemonData.h"
#include "MoveData.h"

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
	const float GetTypeMatchup(Type attackingType, Type defendingType) const;

private:
	// �ܺο��� ���� ����
	DataManager() = default;

	// �����(private) �Լ� '����'
	void LoadPokemonSpecies(const std::string& filePath);
	void LoadMoves(const std::string& filePath);
	void LoadTypeMatchups(const std::string& filePath);

	// �ε�� �����͸� ������ ������ ���̽�
	std::map<int, PokemonSpecies> speciesDatabase_;
	std::map<int, MoveData> moveDatabase_;
	std::map<Type, std::map<Type, float>> typeMatchups_;
};