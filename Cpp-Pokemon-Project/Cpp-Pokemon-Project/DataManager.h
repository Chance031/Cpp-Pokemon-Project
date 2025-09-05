#pragma once

#include <map>

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

private:
	// �����(private) �Լ� '����'
	void LoadPokemonSpecies(const std::string& filePath);
	void LoadMoveData(const std::string& filePath);

	// �ܺο��� ���� ����
	DataManager() = default;

	// �ε�� �����͸� ������ ������ ���̽�
	std::map<int, PokemonSpecies> speciesDatabase_;
	std::map<int, MoveData> moveDatabase_;
};