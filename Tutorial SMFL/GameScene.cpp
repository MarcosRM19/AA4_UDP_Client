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

	for (int i = 0; i < spawnPositions.size() - 1; i++)
	{
		std::shared_ptr<Player> player = std::make_shared<Player>(spawnPositions[i], playerColors[i]);
        
        //este sistema de callback esta hecho con chatgpt
        player->SetShootCallback([this](const sf::Vector2f& position, const sf::Vector2f& direction) {
            CreateBullet(position, direction);
            });
		
        players.push_back(player);
	}
}

void GameScene::SetShootCallback(std::function<void(const sf::Vector2f&, const sf::Vector2f&)> callback)
{
    shootCallback = callback;  // Guarda la funci�n callback
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
    if (GAME.GetReferencePlayer() == nullptr)
        return;

	float deltaTime = clock.restart().asSeconds();
    GAME.GetReferencePlayer()->PrepareMovement(deltaTime);

    // Separar los movimientos en X e Y
    sf::Vector2f originalPos = GAME.GetReferencePlayer()->GetPosition();

    // Intentar mover en X
    sf::Vector2f horizontalMove = { GAME.GetReferencePlayer()->GetVelocity().x * deltaTime, 0.f };
    sf::FloatRect nextXBounds = sf::FloatRect(originalPos + horizontalMove, GAME.GetReferencePlayer()->GetSize());

    if (!GAME.CollidesWithMap(nextXBounds))
    {
        GAME.GetReferencePlayer()->MoveHorizontally(horizontalMove.x);
    }

    sf::Vector2f verticalMove = { 0.f, GAME.GetReferencePlayer()->GetVelocity().y * deltaTime };
    sf::FloatRect nextYBounds = sf::FloatRect(GAME.GetReferencePlayer()->GetPosition() + verticalMove, GAME.GetReferencePlayer()->GetSize());

    if (!GAME.CollidesWithMap(nextYBounds))
    {
        GAME.GetReferencePlayer()->MoveVertically(verticalMove.y);
        GAME.GetReferencePlayer()->SetIsOnGround(false);
    }
    else
    {
        if (GAME.GetReferencePlayer()->GetVelocity().y > 0)
        {
            GAME.GetReferencePlayer()->SetIsOnGround(true);
        }
        else if (GAME.GetReferencePlayer()->GetVelocity().y < 0)
        {
            GAME.GetReferencePlayer()->SetIsOnGround(false);
            GAME.GetReferencePlayer()->StopVertical();
        }
    }

    GAME.GetReferencePlayer()->Update(deltaTime);

    for (auto it = bullets.begin(); it != bullets.end(); )
    {
        (*it)->Update(deltaTime);

        bool bulletDestroyed = false;

        for (std::shared_ptr<Player>& player : players)
        {
            if ((*it)->GetBounds().findIntersection(player->GetGlobalBounds()).has_value())
            {
                player->ReceiveDamage();

                it = bullets.erase(it);
                bulletDestroyed = true;
            }
        }

        //si se destruye la bala y no hay este if se saltaria una bala
        if (!bulletDestroyed)
        {
            it++;
        }
    }
}

void GameScene::Render(sf::RenderWindow& window)
{
	window.clear();
    if (GAME.GetReferencePlayer() == nullptr)
        return;

	GAME.Render(window);

	for (std::shared_ptr<Player>& player : players)
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
		GAME.GetReferencePlayer()->HandleEvent(event);
}

void GameScene::DetectRectangle(sf::Vector2f mousePosition)
{
}

void GameScene::SetCurrentPlayer(int id)
{
    if (id == 0)
    {
        GAME.SetReferencePlayer(players[0].get());
        GAME.SetEnemyPlayer(players[1].get());
    }
    else
    {
        GAME.SetReferencePlayer(players[1].get());
        GAME.SetEnemyPlayer(players[0].get());
    }

    GAME.GetReferencePlayer()->SetId(id);
    playerId = id;
    std::cout << id << std::endl;
    GAME.GetReferencePlayer()->SetColor(sf::Color::Blue);
    GAME.GetEnemyPlayer()->SetColor(sf::Color::Red);
}


