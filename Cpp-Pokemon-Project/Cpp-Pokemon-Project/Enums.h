#pragma once

#include <string>
#include <vector>

// =================================================================
// 포켓몬 핵심 속성 (Core Pokemon Attributes)
// =================================================================

// 포켓몬 타입
enum class Type 
{
    NONE,
    NORMAL,     // 노말
    FIRE,       // 불
    WATER,      // 물
    GRASS,      // 풀
    ELECTRIC,   // 전기
    ICE,        // 얼음
    FIGHTING,   // 격투
    POISON,     // 독
    GROUND,     // 땅
    FLYING,     // 비행
    PSYCHIC,    // 에스퍼
    BUG,        // 벌레
    ROCK,       // 바위
    GHOST,      // 고스트
    DRAGON,     // 드래곤
    DARK,       // 악
    STEEL,      // 강철
    FAIRY       // 요정
};

// 포켓몬의 능력치 종류
enum class Stat 
{
    HP,                 // H
    ATTACK,             // A
    DEFENSE,            // B
    SPECIAL_ATTACK,     // C
    SPECIAL_DEFENSE,    // D
    SPEED,              // S
    ACCURACY,           // 명중률
    EVASION             // 회피율
};

// 포켓몬의 성격
enum class Nature 
{
    HARDY,      // 노력 
    LONELY,     // 외로움
    ADAMANT,    // 고집
    NAUGHTY,    // 개구쟁이
    BRAVE,      // 용감
    BOLD,       // 대담
    DOCILE,     // 온순
    IMPISH,     // 장난꾸러기
    LAX,        // 촐랑
    RELAXED,    // 무사태평
    MODEST,     // 조심
    MILD,       // 의젓
    BASHFUL,    // 수줍음
    RASH,       // 덜렁
    QUIET,      // 냉정
    CALM,       // 차분
    GENTLE,     // 얌전
    CAREFUL,    // 신중
    QUIRKY,     // 변덕
    SASSY,      // 건방
    TIMID,      // 겁쟁이
    HASTY,      // 성급
    JOLLY,      // 명랑
    NAIVE,      // 천진난만
    SERIOUS     // 성실
};

// =================================================================
// 기술 관련 (Move-related)
// =================================================================

// 기술 타입
enum class MoveCategory 
{
    PHYSICAL,   // 물리
    SPECIAL,    // 특수
    STATUS      // 변화
};

// 기술의 대상 범위
enum class MoveTarget 
{
    SELECTED_TARGET,        // 선택한 1체 (적 또는 아군)
    SELF,                   // 자기 자신
    ALLY,                   // 아군 1체
    RANDOM_TARGET,          // 랜덤한 상대 1체
    ALL_ADJACENT_FOES,      // 인접한 모든 적
    ALL_ADJACENT_POKEMON,   // 인접한 모든 포켓몬
    FIELD,                  // 필드 전체
    SPECIAL                 // 특수 케이스
};

// 기술의 특수 행동 방식
enum class MoveExecutionType 
{
    NORMAL,             // 일반 실행
    CHARGE,             // 1턴 모으기 (솔라빔)
    TWO_TURN,           // 1턴 숨고 2턴 공격 (공중날기, 구멍파기)
    RECHARGE            // 사용 후 1턴 행동불가 (파괴광선)
};

// =================================================================
// 포켓몬 성장 및 개체 관련 (Growth & Individual-related)
// =================================================================

// 성별
enum class Gender
{
    MALE,           // 수컷
    FEMALE,         // 암컷
    GENDERLESS      // 무성
};

// 경험치 그룹 (성장 속도)
enum class ExpGroup
{
    ERRATIC,            // 600,000      (불규칙)
    FAST,               // 800,000      (빠름)
    MEDIUM_FAST,        // 1,000,000    (중간 빠름)
    MEDIUM_SLOW,        // 1,059,860    (중간 느림)
    SLOW,               // 1,250,000    (느림)
    FLUCTUATING         // 1,640,000    (변동)
};

// =================================================================
// 배틀 상태 및 환경 관련 (Battle Status & Environment-related)
// =================================================================

// 주요 상태 이상
enum class StatusCondition
{
    NONE,
    POISON,     // 독
    TOXIC,      // 맹독
    BURN,       // 화상
    PARALYSIS,  // 마비
    SLEEP,      // 잠듦
    FREEZE      // 얼음
};

// 일시적인 전투 상태
enum class VolatileStatus
{
    NONE,
    CONFUSION,    // 혼란
    FLINCH,       // 풀죽음
    LEECH_SEED,   // 씨뿌리기
    TRAPPED       // 함정
};

// 날씨 상태
enum class Weather 
{
    NONE,
    HARSH_SUNLIGHT, // 쾌청
    RAIN,           // 비
    SANDSTORM,      // 모래바람
    SNOW            // 설경
};

// 필드 상태
enum class Terrain 
{
    NONE,
    ELECTRIC,           // 일렉트릭필드
    GRASSY,             // 그래스필드
    MISTY,              // 미스트필드
    PSYCHIC             // 사이코필드
};

// 기술 효과의 카테고리
enum class EffectCategory
{
    NONE,
    PRIMARY_STATUS,     // 화상, 마비 등 주요 상태이상
    MULTI_STATUS,       // 여러 상태이상 중 하나를 검 (트라이어택)
    VOLATILE_STATUS,    // 혼란, 풀죽음 등 휘발성 상태
    STAT_CHANGE,        // 단일 스탯 랭크 변화
    MULTI_STAT_CHANGE,  // 복수 스탯 랭크 변화
    HEAL,               // 회복
    DAMAGE              // 데미지 (고정 데미지, HP 비례 등)
};

// =================================================================
// 기술 효과 관련 (Move Effect-related)
// =================================================================

// 스탯 랭크 변화 정보를 담는 구조체
struct StatChange
{
    Stat stat;      // 대상 스탯 (예: ATTACK)
    int stages;     // 변화량 (예: +1, -2)
};

// move_effects.csv의 한 줄을 파싱하여 저장할 최종 구조체
struct MoveEffectData
{
    int id = 0;
    std::string identifier = "";
    EffectCategory category = EffectCategory::NONE;
    MoveTarget target = MoveTarget::SELECTED_TARGET;

    // 카테고리에 따라 파싱될 데이터
    StatusCondition primaryStatus = StatusCondition::NONE;
    VolatileStatus volatileStatus = VolatileStatus::NONE;
    std::vector<StatusCondition> multiStatusOptions{};
    std::vector<StatChange> statChanges{};
};