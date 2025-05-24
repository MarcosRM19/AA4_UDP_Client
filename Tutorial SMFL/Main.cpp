#include "SceneManager.h"
#include "NetworkManager.h"


void main()
{
	srand(time(NULL));
	NETWORK.Init();
	
	if (NETWORK.ConnectToServer())
	{
		NETWORK.Start();
		SCENE.InitScenes(new RegisterScene());
		SCENE.Update();
	}
}