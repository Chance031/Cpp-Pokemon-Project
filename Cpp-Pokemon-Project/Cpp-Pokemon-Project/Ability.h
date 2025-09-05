#pragma once

#include <string>

enum class AbilityTrigger 
{
    ON_ENTRY,         // 등장 시 (예: 위협)
    ON_CONTACT,       // 접촉 시 (예: 정전기)
    PASSIVE           // 지속 효과 (예: 부유)
};

class Ability
{
public:
    // 생성자 선언
    Ability() : id_(0), name_(""), description_(""), triggerType_(AbilityTrigger::PASSIVE) {}
    Ability(int id, const std::string& name, const std::string& description);

    // Getter 함수
    int GetId() const { return id_; }
    std::string GetName() const { return name_; }
    std::string GetDescription() const { return description_; }

private:
    int id_;
    std::string name_;
    std::string description_;
    AbilityTrigger triggerType_;
};

