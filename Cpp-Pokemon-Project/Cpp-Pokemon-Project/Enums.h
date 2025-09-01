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

// 기술 타입
enum class SkillCategory {
    NOEN,
    PHYSICAL,   // 물리
    SPECIAL,    // 특수
    STATUS      // 변화
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

enum class SkillBehavior {
    NORMAL,
    TWO_TURN_INVULNERABLE,      // 공중날기, 구멍파기처럼 1턴째에 피하는 기술
    CHARGE_NO_INVULNERABLE,     // 솔라빔처럼 1턴째에 기를 모으는 기술
    DIGGING,                    // 땅속에 숨은 상태
    FLYING,                     // 공중에 있는 상태
    // ... 등등
};