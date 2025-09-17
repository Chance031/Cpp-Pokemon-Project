#include "Move.h"
#include "MoveData.h"

// 생성자
Move::Move(const MoveData& data)
// 멤버 변수 초기화
	: data_(&data),
	currentPp_(data.pp),    // 현재 PP를 최대 PP로 초기화
	maxPp_(data.pp),        // 현재 '최대 PP'도 기본 PP로 초기화
	ppUpStages_(0)          // PP 강화 횟수는 0으로 시작
{
}

// PP를 감소시키는 함수
void Move::DecrementPp(int amount)
{
	currentPp_ -= amount;
	// PP가 0보다 아래로 내려가지 않도록 보정
	if (currentPp_ < 0)
	{
		currentPp_ = 0;
	}
}

// PP를 회복시키는 함수
void Move::RestorePp(int amount)
{
	currentPp_ += amount;
	// 현재 PP가 최대 PP를 넘지 않도록 보정
	if (currentPp_ > maxPp_)
	{
		currentPp_ = maxPp_;
	}
}

// PP를 특정 값으로 설정하는 함수
void Move::SetPp(int value)
{
	currentPp_ = value;
	// 설정된 값이 0과 최대 PP 사이를 벗어나지 않도록 보정
	if (currentPp_ < 0) currentPp_ = 0;
	if (currentPp_ > maxPp_) currentPp_ = maxPp_;
}