#include "SceneManager.h"
#include "PacketManager.h"
#include "NetworkManager.h"
#include <fstream>
#include "EventManager.h"

void SceneManager::InitScenes()
{
	window = new sf::RenderWindow(sf::VideoMode({ WIDTH, HEIGHT }), "Tutorial SMFL",
		sf::Style::Titlebar | sf::Style::Close);

	if (!font.openFromFile("../Assets/Font/arialceb.ttf"))
	{
		std::cerr << "Error: no se pudo cargar la fuente\n";
	}

	PACKET_MANAGER.Init();
	PACKET_MANAGER.SendHandshake(" ");

	CustomPacket customPacket(ASK_MAP);
	EVENT_MANAGER.Emit(customPacket.type, customPacket);
}

void SceneManager::ChangeScene(Scene* scene)
{
	if(currentScene != nullptr)
		currentScene->Exit();
	currentScene = scene;
	currentScene->Enter(*window);
}

void SceneManager::Update()
{
	while (window->isOpen())
	{
		if (launcherFinished)
		{
			while (const std::optional event = window->pollEvent())
			{
				currentScene->Update(*window, *event);
			}
		}
	}

	delete window;
}
