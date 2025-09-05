#pragma once

// ============================================================
// 1. 포켓몬의 고유 속성 (Pokemon's Intrinsic Properties) 
// ============================================================

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
    SPEED               // S
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

// ============================================================
// 2. 기술 관련 속성 (Move-related Properties) 
// ============================================================

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
    SINGLE_OPPONENT,    // 상대 하나
    ALL_OPPONENTS,      // 인접한 모든 상대
    SELF,               // 자기 자신
    ALL_ALLIES,         // 모든 아군
    ENTIRE_FIELD        // 필드 전체
};


// 기술의 특수 행동 방식
enum class MoveExecutionType 
{
    NORMAL,             // 일반 실행
    CHARGE,             // 1턴 모으기 (솔라빔)
    TWO_TURN,           // 1턴 숨고 2턴 공격 (공중날기, 구멍파기)
    RECHARGE            // 사용 후 1턴 행동불가 (파괴광선)
};

// ============================================================
// 3. 상태 관련 속성 (Status-related Properties)
// ============================================================

// 지속되는 주요 상태 이상
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

// ============================================================
// 4. 환경 관련 속성 (Environment-related Properties)
// ============================================================
 
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