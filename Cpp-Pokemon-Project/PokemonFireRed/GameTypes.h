#pragma once

enum class GameState
{
	FadingIn,
	Intro_GameFreak,      // 1. GAME FREAK �ΰ� ���
	FadingOut,
	Intro_BattleAnim,     // 2. �հ� vs �ϵ己 ���� ���
	TitleScreen,          // 3. Ÿ��Ʋ ȭ��
	InBattle              // 4. �ΰ��� ���� ���
};