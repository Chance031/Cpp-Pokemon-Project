#pragma once

// ������ �ֻ��� '���'�� ����
enum class GameState
{
	Intro,
	TitleScreen,
	Battle,
	World
};

// ��Ʈ�� ����� ���� �ܰ踦 ����
enum class IntroState
{
	Copyright,
	GameFreakPresents,
	GengarVsNidorino,
	TitleScreen,
	Finished // ��Ʈ�ΰ� ��� �������� �˸��� ����
};

// ���̵� ���¸� ����
enum class FadeState
{
	None,
	FadingOut,
	FadingIn
};