#include "SceneManager.h"
#include "PacketManager.h"
#include "NetworkManager.h"
#include <fstream>

void SceneManager::InitScenes(Scene* scene)
{
	window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Tutorial SMFL",
		sf::Style::Titlebar | sf::Style::Close);

	if (!font.openFromFile("../Assets/Font/arialceb.ttf"))
	{
		std::cerr << "Error: no se pudo cargar la fuente\n";
	}

	currentScene = scene;
	currentScene->Enter(*window);

	PACKET_MANAGER.Init();
	PACKET_MANAGER.SendHandshake(" ");

	launcherFinished = true;
}

void SceneManager::ReadMap(std::string jsonContent)
{
	std::string folderPath = "../Assets/Maps/";
	std::string savePath = folderPath + "map1.json";

	std::filesystem::create_directories(folderPath);

	std::ofstream file(savePath);
	if (file.is_open()) 
	{
		file << jsonContent;
		file.close();
		std::cout << "Map saved to " << savePath << std::endl;
	}
	else 
	{
		std::cerr << "Failed to open file for writing: " << savePath << std::endl;
	}
}

void SceneManager::ChangeScene(Scene* scene)
{
	currentScene->Exit();
	currentScene = scene;
	currentScene->Enter(*window);
}

void SceneManager::Update()
{
	if (!launcherFinished)
		return;

	while (window->isOpen())
	{
		while (const std::optional event = window->pollEvent())
		{
			currentScene->Update(*window, *event);
		}
	}

	delete window;
}
