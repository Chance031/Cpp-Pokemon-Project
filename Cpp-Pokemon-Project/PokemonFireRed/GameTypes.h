#pragma once

// 게임의 최상위 '장면'을 정의
enum class GameState
{
	Intro,
	TitleScreen,
	Battle,
	World
};

// 인트로 장면의 세부 단계를 정의
enum class IntroState
{
	Copyright,
	GameFreakPresents,
	GengarVsNidorino,
	TitleScreen,
	Finished // 인트로가 모두 끝났음을 알리는 상태
};

// 페이드 상태를 관리
enum class FadeState
{
	None,
	FadingOut,
	FadingIn
};