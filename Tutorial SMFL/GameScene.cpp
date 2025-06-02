#include "GameScene.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "EventManager.h"

void GameScene::GenerateColisions()
{

}

void GameScene::InitPlayers()
{
	std::vector<sf::Vector2f> spawnPositions = GAME.GetSpawnPositions();

	std::vector<sf::Color> playerColors = { sf::Color::Blue, sf::Color::Red };

	for (int i = 0; i < spawnPositions.size(); i++)
	{
		std::unique_ptr<Player> player = std::make_unique<Player>(spawnPositions[i], playerColors[i]);
		players.push_back(std::move(player));
	}
}

void GameScene::Enter(sf::RenderWindow& window)
{
	std::cout << "Enter Game Scene" << std::endl;
	GAME.Init(window);
	InitPlayers();
}

void GameScene::Exit()
{
	std::cout << "Exit Game Scene" << std::endl;
}

void GameScene::Update()
{
	float deltaTime = clock.restart().asSeconds();
    GAME.GetPlayer()->PrepareMovement(deltaTime);

    // Separar los movimientos en X e Y
    sf::Vector2f originalPos = GAME.GetPlayer()->GetPosition();

    // Intentar mover en X
    sf::Vector2f horizontalMove = { GAME.GetPlayer()->GetVelocity().x * deltaTime, 0.f };
    sf::FloatRect nextXBounds = sf::FloatRect(originalPos + horizontalMove, GAME.GetPlayer()->GetSize());

    if (!GAME.CollidesWithMap(nextXBounds))
    {
        GAME.GetPlayer()->MoveHorizontally(horizontalMove.x);
    }

    sf::Vector2f verticalMove = { 0.f, GAME.GetPlayer()->GetVelocity().y * deltaTime };
    sf::FloatRect nextYBounds = sf::FloatRect(GAME.GetPlayer()->GetPosition() + verticalMove, GAME.GetPlayer()->GetSize());

    if (!GAME.CollidesWithMap(nextYBounds))
    {
        GAME.GetPlayer()->MoveVertically(verticalMove.y);
        GAME.GetPlayer()->SetIsOnGround(false);
    }
    else
    {
        if (GAME.GetPlayer()->GetVelocity().y > 0)
        {
            GAME.GetPlayer()->SetIsOnGround(true);
        }
        else if (GAME.GetPlayer()->GetVelocity().y < 0)
        {
            GAME.GetPlayer()->SetIsOnGround(false);
            GAME.GetPlayer()->StopVertical();
        }
    }

    GAME.GetPlayer()->Update(deltaTime);
}

void GameScene::Render(sf::RenderWindow& window)
{
	window.clear();
	GAME.Render(window);

	for (std::unique_ptr<Player>& player : players)
	{
		player->Render(window);
	}
}

void GameScene::HandleEvent(const sf::Event& event)
{
	if (!players.empty())
		GAME.GetPlayer()->HandleEvent(event);
}

void GameScene::DetectRectangle(sf::Vector2f mousePosition)
{
}

void GameScene::SetCurrentPlayer(int id)
{
    GAME.SetReferencePlayer(players[id].get());
    GAME.GetPlayer()->SetId(id);
    playerId = id;
    std::cout << id << std::endl;
    for (std::unique_ptr<Player>& player : players)
    {
        if (player->GetIdPlayer() == GAME.GetPlayer()->GetIdPlayer())
            player->SetColor(sf::Color::Blue);
        else
            player->SetColor(sf::Color::Red);
    }
}


