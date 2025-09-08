#pragma once

#include "MoveData.h"

class Move
{
public:
	// ������
	// explicit Ű����� ���ʿ��� �Ͻ��� ����ȯ�� �����ݴϴ�.
	explicit Move(const MoveData& data);

	// Getter �Լ�
	// MoveData�� �ִ� �Һ� �������� �״�� �ܺο� �����ϴ� ���Ҹ� �մϴ�.
	int GetId() const { return data_.id; }
	const std::string& GetName() const { return data_.name_kr; }
	Type GetType() const { return data_.type; }
	MoveCategory GetCategory() const { return data_.category; }
	int GetPower() const { return data_.power; }
	int GetAccuracy() const { return data_.accuracy; }
	int GetMaxPp() const { return data_.pp; }
	int GetPriority() const { return data_.priority; }
	bool IsContactMove() const { return data_.is_contact; }

    // --- �ν��Ͻ� ���� ���� �Լ� ---
    // �� Move ��ü���� ������ ������ ����(currentPp)�� �ٷ�ϴ�.
    int GetCurrentPp() const { return currentPp_; }
    bool IsUsable() const { return currentPp_ > 0; }

    void DecrementPp(int amount = 1);
    void RestorePp(int amount);
    void SetPp(int value);

private:
	// ��� ����
	const MoveData& data_;
	int currentPp_;
	int maxPp_;           
	int ppUpStages_ = 0;  
};