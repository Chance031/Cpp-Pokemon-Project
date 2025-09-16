#pragma once

#include "MoveData.h"

class Move
{
public:
	// 생성자
	explicit Move(const MoveData& data);

	// Getter 함수
	int GetId() const { return data_->id; }
	const std::string& GetName() const { return data_->name_kr; }
	Type GetType() const { return data_->type; }
	MoveCategory GetCategory() const { return data_->category; }
	int GetPower() const { return data_->power; }
	int GetAccuracy() const { return data_->accuracy; }
	int GetMaxPp() const { return data_->pp; }
	int GetEffectId() const { return data_->effect_id; }
	int GetEffectChance() const { return data_->effect_chance; }
	int GetPriority() const { return data_->priority; }
	bool IsContactMove() const { return data_->is_contact; }

	// --- 인스턴스 상태 관련 함수 ---
	int GetCurrentPp() const { return currentPp_; }
	bool IsUsable() const { return currentPp_ > 0; }

	void DecrementPp(int amount = 1);
	void RestorePp(int amount);
	void SetPp(int value);

private:
	// 멤버 변수 (포인터* 사용)
	const MoveData* data_;
	int currentPp_;
	int maxPp_;
	int ppUpStages_ = 0;
};