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

// =================================================================
// 기술 효과 관련 (Move Effect-related)
// =================================================================

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
    DAMAGE,             // 데미지 (고정 데미지, HP 비례 등)

    MULTI_HIT,          // 연속 공격
    CHARGE_TURN,        // 1턴 모으기
    TWO_TURN_ATTACK,    // 2턴 공격 (공중날기 등)
    RECHARGE_TURN,      // 사용 후 1턴 휴식 (파괴광선)
    RECOIL,             // 반동 데미지
    DRAIN,              // HP 흡수
    INSTANT_EFFECT,     // 일격필살 등 즉시 발동 효과
    FIXED_DAMAGE,       // 고정 데미지
    POST_BATTLE         // 배틀 종료 후 효과 (고양이돈받기)
};

// =================================================================
// 특성 관련 (Ability-related)
// =================================================================

enum class AbilityTrigger
{
    // --- 지속 효과 ---
    PASSIVE,                // 필드에 있는 동안 계속 발동

    // --- 등장 / 퇴장 시 ---
    ON_SWITCH_IN,           // 교체로 등장했을 때
    ON_SWITCH_OUT,          // 교체로 퇴장할 때

    // --- 턴 기반 ---
    ON_END_TURN,            // 턴의 종료 시

    // --- 피격 / 공격 시 ---
    ON_CONTACT,             // 접촉 공격을 받았을 때
    ON_HIT_BY_MOVE,         // 기술에 맞았을 때 (접촉/비접촉 무관)
    ON_HIT_BY_TYPE,         // 특정 타입의 기술에 맞았을 때
    ON_HIT_PHYSICAL,        // 물리 기술에 맞았을 때
    ON_CRITICAL_HIT,        // 급소에 맞았을 때
    ON_MOVE_TARGET,         // 자신이 기술의 대상이 되었을 때 (프레셔)

    // --- 행동 시 ---
    ON_MOVE_USE,            // 기술을 사용하려고 할 때 (변환자재)
    ON_MOVE_LAST,           // 턴에서 가장 마지막으로 행동할 때

    // --- 상태 변화 시 ---
    ON_STATUS,              // 특정 상태이상이 되었을 때
    ON_STAT_LOWERED,        // 능력치 랭크가 떨어졌을 때
    ON_FOE_STAT_UP,         // 상대의 능력치 랭크가 올랐을 때
    ON_FLINCH,              // 풀죽었을 때
    ON_INTIMIDATED,         // 위협을 받았을 때

    // --- HP 변화 시 ---
    ON_LOW_HP,              // HP가 특정 비율 이하일 때

    // --- 기절(Faint) 관련 ---
    ON_KO,                  // 상대를 쓰러뜨렸을 때
    ON_FAINT,               // 자신이 쓰러졌을 때
    ON_FAINT_CONTACT,       // 접촉 공격으로 자신이 쓰러졌을 때
    ON_ANY_FAINT,           // 필드의 누군가가 쓰러졌을 때
    ON_ALLY_FAINT,          // 아군이 쓰러졌을 때

    // --- 필드 / 날씨 / 아이템 관련 ---
    ON_WEATHER,             // 특정 날씨일 때
    ON_TERRAIN,             // 특정 필드일 때
    ON_TERRAIN_CHANGE,      // 필드가 변경되었을 때
    ON_BERRY_EAT,           // 나무열매를 먹었을 때
    ON_ALLY_ITEM_USE,       // 아군이 아이템을 사용했을 때
    ON_OTHER_MOVE_USE,      // 필드의 다른 포켓몬이 특정 기술을 사용했을 때 (무희)
    ON_END_BATTLE,          // 배틀 종료 후

    // --- 특수 조건 ---
    SPECIAL_LOGIC           // 위 분류에 해당하지 않는 복합/고유 로직
};

enum class AbilityCategory
{
    // --- 능력치(Stat) 관련 ---
    STAT_CHANGE,                // 자신의/상대의 스탯 랭크를 직접 변경 (가속, 위협)
    STAT_MODIFIER,              // 자신의 스탯 실수치를 특정 배율로 조절 (천하장사)
    STAT_MODIFIER_STATUS,       // 상태이상 시 스탯 실수치를 조절 (근성)
    STAT_MODIFIER_IMMUNITY,     // 특정 스탯 랭크가 떨어지지 않게 함 (괴력집게)
    REVERSE_STAT_CHANGES,       // 스탯 랭크 변화를 반전시킴 (심술꾸러기)

    // --- 데미지 계산 관련 ---
    DAMAGE_MODIFIER,            // 주는/받는 데미지를 조절 (테크니션, 필터)
    DAMAGE_MODIFIER_TYPE,       // 특정 타입 기술의 데미지를 조절 (두꺼운지방)
    DAMAGE_IMMUNITY,            // 특정 조건의 데미지를 무효화 (불가사의부적)
    DAMAGE,                     // 직접 데미지를 줌 (까칠한피부)

    // --- 상태이상(Status) 관련 ---
    PRIMARY_STATUS,             // 독, 마비 등 주요 상태이상을 검
    MULTI_STATUS,               // 여러 주요 상태이상 중 하나를 검
    VOLATILE_STATUS,            // 혼란 등 일시적 상태이상을 검
    STATUS_IMMUNITY,            // 특정 주요 상태이상을 막음
    STATUS_HEAL,                // 상태이상을 회복함
    STATUS_TRANSFER,            // 상태이상을 상대에게 옮김

    // --- 기술(Move) 관련 ---
    MOVE_IMMUNITY,              // 특정 종류의 기술을 무효화 (방음)
    MOVE_MODIFIER,              // 기술의 속성을 변경 (틀깨기)
    EFFECT_CHANCE_MOD,          // 기술의 부가 효과 발동 확률을 조절 (하늘의은총)
    PP_USAGE_MODIFIER,          // PP 소모량을 조절 (프레셔)
    ACCURACY_MODIFIER,          // 명중률을 조절 (복안, 노가드)
    PRIORITY_MODIFIER,          // 우선도를 조절 (짓궂은마음)
    CRITICAL_HIT_MODIFIER,      // 급소 확률을 조절 (대운)

    // --- 타입(Type) 관련 ---
    TYPE_CHANGE,                // 자신의 타입을 변경
    TYPE_IMMUNITY,              // 특정 타입 기술을 무효화 (부유)
    TYPE_EFFECTIVENESS,         // 타입 상성을 무시함 (배짱)

    // --- 필드(Field) 관련 ---
    WEATHER_CHANGE,             // 날씨를 변경
    TERRAIN_CHANGE,             // 필드(지형)를 변경
    FIELD_EFFECT_NEGATE,        // 필드 효과를 무효화

    // --- 흡수/회복 관련 ---
    HEAL,                       // HP를 회복 (저수)
    ABSORB_BOOST,               // 특정 타입 기술을 흡수하고 능력치 강화 (타오르는불꽃)
    REDIRECT_BOOST,             // 특정 타입 기술을 자신에게 유인하고 강화 (피뢰침)

    // --- 기타 효과 ---
    EFFECT_IMMUNITY,            // 풀죽음, 급소 등 특정 효과를 막음 (전투무장)
    SURVIVE_LETHAL,             // 일격에 쓰러지는 것을 막음 (옹골참)
    TRAP,                       // 상대의 교체를 막음 (그림자밟기)
    ITEM_ACTION,                // 아이템 관련 효과 (점착, 긴장감)
    BATTLE_ACTION,              // 배틀 행동 관련 효과 (도주)
    TRANSFORM,                  // 변신
    WEIGHT_MODIFIER,            // 무게를 조절

    // --- 특수 조건 ---
    SPECIAL_LOGIC               // 위 분류에 해당하지 않는 복합/고유 로직
};