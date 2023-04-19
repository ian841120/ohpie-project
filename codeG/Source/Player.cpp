#include "Player.h"
#include "Input\InputClass.h"

Player::Player()
{
	sprite = std::make_unique<Sprite>();
	playerTexture= std::make_unique<Texture>("./Data/Image/golem.png");
	sprite->SetShaderResourceView(playerTexture->GetSRV().Get(), playerTexture->GetWidth(), playerTexture->GetHeight());
	state = 0;
	speed = 10;
	width = 64;
	height = 64;
	flip = 1;
	shaderType = ShaderId::defaultSpriteShader;
	maskTexture = std::make_unique<Texture>("./Data/Image/febucci-dissolve-texture-example.jpg");
	maskData.maskTexture = maskTexture->GetSRV().Get();
	maskData.dissolveThreshold = 1.0f;
	maskData.edgeThreshold = 0.25f;
	maskData.edgeColor = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
}
void  Player::Update(float elapsedTime)
{
	KeyBoardClass& kb = InputClass::Instance().GetKeyBoard();
	sprite->SetPovit(povit);

	switch (state)
	{
	case 0:
		sprite->Update(position.x, position.y, width*flip, height);
		if (kb.GetButtonState(KeyBoardClass::KBKEY::W))
		{
			position.y -= speed;
			
		}
		if (kb.GetButtonState(KeyBoardClass::KBKEY::S))
		{
			position.y += speed;
			
		}

		if (kb.GetButtonState(KeyBoardClass::KBKEY::A))
		{
			position.x -= speed;
			flip = -1;
		}

		if (kb.GetButtonState(KeyBoardClass::KBKEY::D))
		{
			position.x += speed;
			flip = 1;
		}

		if (kb.GetButtonDown(KeyBoardClass::KBKEY::Z))
		{
			state++;
		}
		break;
	case 1:
		shaderType = ShaderId::maskShader;
		maskData.dissolveThreshold -= 0.01f;
		break;
	}
}
void Player::Render(RenderContext&rc)
{
	Graphics& graphics = Graphics::GetInstance();
	SpriteShader* shader = graphics.GetShader(shaderType);
	rc.maskData.maskTexture = maskData.maskTexture;
	rc.maskData.dissolveThreshold = maskData.dissolveThreshold;
	rc.maskData.edgeThreshold = maskData.edgeThreshold;
	rc.maskData.edgeColor = maskData.edgeColor;
	shader->Begin(rc);
	shader->Render(rc, sprite.get());
	shader->End(rc);

}
void Player::DebugGUI()
{
	ImGui::Begin("PlayerParameter");

	ImGui::DragFloat2("PlayerPos", &position.x);
	ImGui::DragFloat("PlayerWidth", &width);
	ImGui::DragFloat("PlayerHeight", &height);
	ImGui::DragInt("PlayerSpeed", &speed);
	ImGui::SliderInt("Povit", reinterpret_cast<int*>(&povit), 0, 8);
	ImGui::Text("Texture");
	ImGui::Image(playerTexture->GetSRV().Get(), { 128,128 });

	ImGui::Text("Mask Texture");
	ImGui::Image(maskTexture->GetSRV().Get(), { 128,128 });
	ImGui::SliderFloat("Dissolve Threshold", &maskData.dissolveThreshold, 0.0f, 1.0f);
	ImGui::SliderFloat("Edge Threshold", &maskData.edgeThreshold, 0.0f, 1.0f);
	ImGui::ColorEdit4("Edge Color", &maskData.edgeColor.x);

	ImGui::End();
}