#include "Player.h"
#include "Input\InputClass.h"

Player::Player()
{
	sprite = std::make_unique<Sprite>();
	state = 0;
	shaderType = ShaderId::defaultSpriteShader;
}
void  Player::Update()
{
	KeyBoardClass& kb = InputClass::Instance().GetKeyBoard();

	switch (state)
	{
	case 0:
		sprite->Update(position.x, position.y, 30, 30);
		if (kb.GetButtonState(KeyBoardClass::KBKEY::W))
		{
			position.y -= 10;
		}
		if (kb.GetButtonState(KeyBoardClass::KBKEY::S))
		{
			position.y += 10;
		}

		if (kb.GetButtonState(KeyBoardClass::KBKEY::A))
		{
			position.x -= 10;
		}

		if (kb.GetButtonState(KeyBoardClass::KBKEY::D))
		{
			position.x += 10;
		}

		if (kb.GetButtonDown(KeyBoardClass::KBKEY::Z))
		{
			state++;
		}
		break;
	case 1:
		shaderType = ShaderId::maskShader;
		break;
	}
}
void Player::Render()
{

}