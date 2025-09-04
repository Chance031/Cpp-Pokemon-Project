#pragma once

// 포켓몬 타입
enum class Type {
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
enum class Stat {
    HP,
    ATTACK,
    DEFENSE,
    SPECIAL_ATTACK,
    SPECIAL_DEFENSE,
    SPEED
};

// 포켓몬의 성격
enum class Nature {
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
enum class ExpGroup {
    ERRATIC,            // 600,000      (불규칙)
    FAST,               // 800,000      (빠름)
    MEDIUM_FAST,        // 1,000,000    (중간 빠름)
    MEDIUM_SLOW,        // 1,059,860    (중간 느림)
    SLOW,               // 1,250,000    (느림)
    FLUCTUATING         // 1,640,000    (변동)
};

// 기술 타입
enum class SkillCategory {
    PHYSICAL,   // 물리
    SPECIAL,    // 특수
    STATUS      // 변화
};

// 기술의 대상 범위
enum class TargetType {
    SINGLE_OPPONENT,    // 상대 하나
    ALL_OPPONENTS,      // 인접한 모든 상대
    SELF,               // 자기 자신
    ALL_ALLIES,         // 모든 아군
    ENTIRE_FIELD        // 필드 전체
};


// 기술의 특수 행동 방식
enum class SkillBehavior {
    NORMAL,
    TWO_TURN_INVULNERABLE,      // 공중날기, 구멍파기처럼 1턴째에 피하는 기술
    CHARGE_NO_INVULNERABLE,     // 솔라빔처럼 1턴째에 기를 모으는 기술
    DIGGING,                    // 땅속에 숨은 상태
    FLYING,                     // 공중에 있는 상태
    // ... 등등
};

// 상태 이상
enum class StatusCondition {
    NONE,
    POISON,     // 독
    TOXIC,      // 맹독
    BURN,       // 화상
    PARALYSIS,  // 마비
    SLEEP,      // 잠듦
    FREEZE      // 얼음
};

// 상태 변화
enum class VolatileStatus {
    NONE,
    CONFUSION,    // 혼란
    INFATUATION,  // 헤롱헤롱
    LEECH_SEED,   // 씨뿌리기
    FLINCH        // 풀죽음
};

// 날씨 상태
enum class Weather {
    NONE,
    HARSH_SUNLIGHT,     // 쾌청
    RAIN,               // 비
    SANDSTORM,          // 모래바람
    SNOW                // 설경
};

// 필드 상태
enum class Terrain {
    NONE,
    ELECTRIC,           // 일렉트릭필드
    GRASSY,             // 그래스필드
    MISTY,              // 미스트필드
    PSYCHIC             // 사이코필드
};