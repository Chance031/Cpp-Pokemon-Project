#pragma once

enum class GameState
{
	FadingIn,
	Intro_GameFreak,      // 1. GAME FREAK 로고 장면
	FadingOut,
	Intro_BattleAnim,     // 2. 겐가 vs 니드런 전투 장면
	TitleScreen,          // 3. 타이틀 화면
	InBattle              // 4. 인게임 전투 장면
};