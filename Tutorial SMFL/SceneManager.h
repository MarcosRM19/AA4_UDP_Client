#pragma once
#include "RegisterScene.h"
#include "MatchMackingScene.h"
#include "GameScene.h"

#define SCENE SceneManager::Instance()
#define WIDTH 850
#define HEIGHT 850

class SceneManager
{
private:
	SceneManager() = default;

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator =(const SceneManager&) = delete;

	Scene* currentScene;

	sf::RenderWindow* window;
	sf::Font font;

public:
	inline static SceneManager& Instance()
	{
		static SceneManager manager;
		return manager;
	}

	void InitScenes(Scene* scene);
	void ChangeScene(Scene* scene);
	
	void Update();

	inline Scene* GetCurrentScene() { return currentScene; }
	inline sf::RenderWindow& GetWindow() { return *window; }
	inline const sf::Font& GetFont() const { return font; }
};

