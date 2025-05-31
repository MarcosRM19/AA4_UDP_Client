#include "SceneManager.h"
#include "NetworkManager.h"
#include "PacketManager.h"

void main()
{

	CustomUDPPacket customPacket(UdpPacketType::NORMAL, ACK);
	int messageInt = 1;
	customPacket.WriteVariable(messageInt);

	PACKET_MANAGER.SendPacketToUDPServer(customPacket);


	/*NETWORK.Init();
	if (NETWORK.ConnectToServer())
	{
		NETWORK.Start();
		SCENE.InitScenes();
		SCENE.Update();
	}*/
}