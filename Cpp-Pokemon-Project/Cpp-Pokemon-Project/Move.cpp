#include "Move.h"

Move::Move(const MoveData& data)
	: data_(data), currentPp_(data.maxPp)
{
	//
}

void Move::DecrementPp(int amount)
{
	currentPp_ -= amount;
	if (currentPp_ < 0)
		currentPp_ = 0;
}

void Move::RestorePp(int amount)
{
	currentPp_ += amount;
	if (currentPp_ > data_.maxPp)
		currentPp_ = data_.maxPp;
}

void Move::SetPp(int value)
{
	currentPp_ = value;
	if (currentPp_ < 0) currentPp_ = 0;
	if (currentPp_ > data_.maxPp) currentPp_ = data_.maxPp;
}
