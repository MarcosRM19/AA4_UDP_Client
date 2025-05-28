#include "SceneManager.h"
#include "NetworkManager.h"


void main()
{
	NETWORK.Init();
	if (NETWORK.ConnectToServer())
	{
		NETWORK.Start();
		SCENE.Update();
	}
}