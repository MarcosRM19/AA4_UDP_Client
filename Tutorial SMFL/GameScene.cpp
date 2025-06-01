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
        
        //este sistema de callback esta hecho con chatgpt
        player->SetShootCallback([this](const sf::Vector2f& position, const sf::Vector2f& direction) {
            CreateBullet(position, direction);
            });
		
        players.push_back(std::move(player));
	}
}

void GameScene::SetShootCallback(std::function<void(const sf::Vector2f&, const sf::Vector2f&)> callback)
{
    shootCallback = callback;  // Guarda la función callback
}

void GameScene::CreateBullet(const sf::Vector2f& bulletPos, const sf::Vector2f& bulletDir)
{
    std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>(bulletPos, bulletDir);
    bullets.push_back(newBullet);
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
		
    players[0]->PrepareMovement(deltaTime);

    // Separar los movimientos en X e Y
    sf::Vector2f originalPos = players[0]->GetPosition();

    // Intentar mover en X
    sf::Vector2f horizontalMove = { players[0]->GetVelocity().x * deltaTime, 0.f };
    sf::FloatRect nextXBounds = sf::FloatRect(originalPos + horizontalMove, players[0]->GetSize());

    if (!GAME.CollidesWithMap(nextXBounds))
    {
        players[0]->MoveHorizontally(horizontalMove.x);
    }

    sf::Vector2f verticalMove = { 0.f, players[0]->GetVelocity().y * deltaTime };
    sf::FloatRect nextYBounds = sf::FloatRect(players[0]->GetPosition() + verticalMove, players[0]->GetSize());

    if (!GAME.CollidesWithMap(nextYBounds))
    {
        players[0]->MoveVertically(verticalMove.y);
        players[0]->SetIsOnGround(false);
    }
    else
    {
        if (players[0]->GetVelocity().y > 0)
        {
            players[0]->SetIsOnGround(true);
        }
        else if (players[0]->GetVelocity().y < 0)
        {
            players[0]->SetIsOnGround(false);
            players[0]->StopVertical();
        }
    }

    players[0]->Update(deltaTime);

    // Tiene que ser asi por si se borran balas durante el for
    // Este codigo es de chatgpt
    for (auto it = bullets.begin(); it != bullets.end(); )
    {
        (*it)->Update(deltaTime);
        if (!(*it)->GetIsActive())
            it = bullets.erase(it);
        else
            ++it;
    }
}

void GameScene::Render(sf::RenderWindow& window)
{
	window.clear();
	GAME.Render(window);

	for (std::unique_ptr<Player>& player : players)
	{
		player->Render(window);
	}

    for (const std::shared_ptr<Bullet>& bullet : bullets)
    {
        bullet->Render(window);
    }
}

void GameScene::HandleEvent(const sf::Event& event)
{
	if (!players.empty())
		players[0]->HandleEvent(event);
}

void GameScene::DetectRectangle(sf::Vector2f mousePosition)
{
}


