#include "SceneManager.h"
#include "NetworkManager.h"
#include "PacketManager.h"

void main()
{
	NETWORK.Init();
	if (NETWORK.ConnectToServer())
	{
		NETWORK.Start();
		SCENE.InitScenes();
		SCENE.Update();
	}
}