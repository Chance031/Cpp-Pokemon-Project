#include "Move.h"

Move::Move(int id, const std::string& name, const std::string& description,
    Type type, MoveCategory category, int power, int accuracy, int pp,
    int priority, MoveTarget target, bool isContact, bool isSoundMove,
    bool isSpreadMove, bool isProtectable, bool canBeRedirected)
    : id_(id),
    name_(name),
    description_(description),
    type_(type),
    category_(category),
    power_(power),
    accuracy_(accuracy),
    maxPp_(pp),
    currentPp_(pp),
    priority_(priority),
    moveTarget_(target),
    isContact_(isContact),
    isSoundMove_(isSoundMove),
    isSpreadMove_(isSpreadMove),
    isProtectable_(isProtectable),
    canBeRedirected_(canBeRedirected),
    executionType_(MoveExecutionType::NORMAL) // 기본값으로 초기화
{
    // 생성자 본문은 비어있음
}

void Move::DecrementPp()
{
    if (currentPp_ > 0) {
        currentPp_--;
    }
}