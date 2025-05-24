#include "SceneManager.h"
#include "NetworkManager.h"


void main()
{
	//NETWORK.Init();
	SCENE.InitScenes(new MatchMackingScene());
	SCENE.Update();

	
	/*if (NETWORK.ConnectToServer())
	{
		NETWORK.Start();
		SCENE.InitScenes(new RegisterScene());
		
	}*/
}