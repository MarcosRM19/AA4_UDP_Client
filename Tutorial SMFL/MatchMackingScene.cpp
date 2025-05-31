#include "MatchMackingScene.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "EventManager.h"

MatchMackingScene::MatchMackingScene() : Scene()
{
}

void MatchMackingScene::Enter(sf::RenderWindow& window)
{
	std::cout << "Enter MatchMaking Scene" << std::endl;

	searchingGame = false;

	CreateCancelButtons(window);
	CreateButtons(window);
}

void MatchMackingScene::Exit()
{
	std::cout << "Exit MatchMaking Scene" << std::endl;
}

void MatchMackingScene::Update()
{

}

void MatchMackingScene::Render(sf::RenderWindow& window)
{
	sf::Color brown(238, 208, 157);

	window.clear(brown);

	if (NETWORK.GetNetworkState() == NetworkState::CONNECTED_TO_SERVER_TCP)
	{
		if (searchingGame)
		{
			for (sf::RectangleShape buton : cancelButtons)
				window.draw(buton);
			for (sf::Text text : cancelButtonsTexts)
				window.draw(text);
		}
		else
		{
			for (sf::RectangleShape buton : buttons)
				window.draw(buton);
			for (sf::Text text : buttonsTexts)
				window.draw(text);
		}
	}

	window.display();
}

void MatchMackingScene::DetectRectangle(sf::Vector2f mousePosition)
{
	if (NETWORK.GetNetworkState() != NetworkState::CONNECTED_TO_SERVER_TCP)
		return;

	for (size_t i = 0; i < cancelButtons.size(); i++) {
		if (cancelButtons[i].getGlobalBounds().contains(mousePosition) && searchingGame) {

			CustomTCPPacket customPacket(CANCEL_QUEUE);
			EVENT_MANAGER.TCPEmit(customPacket.type, customPacket);
			searchingGame = false;
		}
	}

	for (size_t i = 0; i < buttons.size(); i++) {
		if (buttons[i].getGlobalBounds().contains(mousePosition) && !searchingGame) {

			CustomTCPPacket customPacket(START_QUEUE);
			EVENT_MANAGER.TCPEmit(customPacket.type, customPacket);
			searchingGame = true;
		}
	}
}


void MatchMackingScene::CreateButtons(sf::RenderWindow& window)
{
	buttons.push_back(sf::RectangleShape());
	buttonsTexts.push_back(sf::Text(SCENE.GetFont()));

	sf::Vector2f position(
		window.getSize().x / 2.f - 150,
		window.getSize().y / 2.f
	);
	std::string text;
	text = "Join Friendly Game";

	buttons[0].setSize({ 300.f, 75.f });
	buttons[0].setFillColor(sf::Color::Green);
	buttons[0].setOrigin(buttons[0].getSize() / 2.f);
	buttons[0].setPosition(position);

	buttonsTexts[0].setString(text);
	buttonsTexts[0].setFillColor(sf::Color::Black);

	sf::FloatRect textBounds = buttonsTexts[0].getLocalBounds();
	buttonsTexts[0].setOrigin(sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.f,
		textBounds.position.y + textBounds.size.y / 2.f
	));

	buttonsTexts[0].setPosition(position);
}

void MatchMackingScene::CreateCancelButtons(sf::RenderWindow& window)
{
	cancelButtons.push_back(sf::RectangleShape());
	cancelButtonsTexts.push_back(sf::Text(SCENE.GetFont()));

	sf::Vector2f position(
		window.getSize().x / 2.f + 150,
		window.getSize().y / 2.f
	);

	std::string text;
	text = "Cancel Queue";

	cancelButtons[0].setSize({ 300.f, 75.f });
	cancelButtons[0].setFillColor(sf::Color::Green);
	cancelButtons[0].setOrigin(cancelButtons[0].getSize() / 2.f);
	cancelButtons[0].setPosition(position);

	cancelButtonsTexts[0].setString(text);
	cancelButtonsTexts[0].setFillColor(sf::Color::Black);

	sf::FloatRect textBounds = cancelButtonsTexts[0].getLocalBounds();
	cancelButtonsTexts[0].setOrigin(sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.f,
		textBounds.position.y + textBounds.size.y / 2.f
	));

	cancelButtonsTexts[0].setPosition(position);
}
