#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <atomic>
#include "main.h"
#include "natives.h"
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "ws2_32.lib")

static SOCKET sendSocket = INVALID_SOCKET;
static SOCKET listenSocket = INVALID_SOCKET;
sockaddr_in sendAddr{};	  // Do wysyłania "8" na 2138
sockaddr_in listenAddr{}; // Do nasłuchiwania "9" na 2139
static std::string loopbackIP = "127.0.0.1";
const std::string deathMsg = "8";
const std::string killMsg = "9";

// Atomic Flags
std::atomic<bool> shouldKillPlayer{false};
std::atomic<bool> networkReady{false};
std::atomic<bool> killedByDeathlink{false};
std::atomic<bool> modEnabled{true};



void ShowNotification(const std::string text, const std::string type)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	if (type == "error")
	{
		std::string formattedText = "~o~~h~MyDeathLink~n~~h~~r~" + text;
		UI::_ADD_TEXT_COMPONENT_STRING((char *)formattedText.c_str());
	}
	if (type == "notify")
	{
		std::string formattedText = "~o~~h~MyDeathLink~n~~h~~p~" + text;
		UI::_ADD_TEXT_COMPONENT_STRING((char *)formattedText.c_str());
	}
	if (type == "success")
	{
		std::string formattedText = "~o~~h~MyDeathLink~n~~h~~g~" + text;
		UI::_ADD_TEXT_COMPONENT_STRING((char *)formattedText.c_str());
	}

	UI::_DRAW_NOTIFICATION(false, false);
}

void SetupNetworking()
{
	WAIT(1000);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		ShowNotification("WSAStartup FAILED!", "error");
		return;
	}

	// Socket do wysyłania "8" na port 2138
	sendSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (sendSocket == INVALID_SOCKET)
	{
		ShowNotification("sendSocket FAILED!", "error");
		return;
	}

	// Socket do nasłuchiwania "9" na port 2139
	listenSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		ShowNotification("listenSocket FAILED!", "error");
		return;
	}

	// Konfiguracja adresu do WYSYŁANIA (port 2138)
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(2138); // Wysyłaj na 2138
	sendAddr.sin_addr.s_addr = inet_addr(loopbackIP.c_str());

	// Konfiguracja adresu do NASŁUCHIWANIA (port 2139)
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_port = htons(2139); // Nasłuchuj na 2139
	listenAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind do portu 2139
	if (bind(listenSocket, (sockaddr *)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR)
	{
		ShowNotification("bind to 2139 FAILED!", "error");
		return;
	}

	networkReady = true;
	ShowNotification("Networking setup COMPLETE!", "success");
}

void CleanupNetworking()
{
	if (listenSocket != INVALID_SOCKET)
	{
		closesocket(listenSocket);
		listenSocket = INVALID_SOCKET;
	}
	if (sendSocket != INVALID_SOCKET)
	{
		closesocket(sendSocket);
		sendSocket = INVALID_SOCKET;
	}
	WSACleanup();
}

void SendDeathNotification()
{
	if (sendSocket != INVALID_SOCKET)
	{
		if (!killedByDeathlink)
		{
			sendto(sendSocket, deathMsg.c_str(), deathMsg.size(), 0, (sockaddr *)&sendAddr, sizeof(sendAddr));
		}
	}
}

void KillPlayer()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	ENTITY::SET_ENTITY_HEALTH(playerPed, 0);
}

void CheckPlayerDeath(bool &wasDead)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (ENTITY::IS_ENTITY_DEAD(playerPed) && !wasDead && modEnabled)
	{
		SendDeathNotification();
		wasDead = true;
		WAIT(3000);
	}
	else if (!ENTITY::IS_ENTITY_DEAD(playerPed))
	{
		wasDead = false;
	}
}

void Listener()
{
	while (true)
	{
		if (!networkReady)
		{
			Sleep(100);
			continue;
		}

		char buffer[1024];
		sockaddr_in from;
		int fromLen = sizeof(from);

		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(listenSocket, &readSet);
		timeval timeout{0, 100000}; // 100ms timeout

		int result = select(0, &readSet, NULL, NULL, &timeout);
		if (result > 0 && FD_ISSET(listenSocket, &readSet))
		{
			int bytes = recvfrom(listenSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&from, &fromLen);
			if (bytes > 0)
			{
				buffer[bytes] = '\0';
				std::string receivedMsg(buffer);

				if (receivedMsg == "9")
				{
					killedByDeathlink = true;
					shouldKillPlayer = true;
					Sleep(2000);
					killedByDeathlink = false;
				}
			}
		}
		Sleep(10);
	}
}

void ScriptMain()
{
	bool wasDead = false;

	WAIT(5000);
	ShowNotification("Mod started!", "success");

	std::thread listenerThread(Listener);
	listenerThread.detach();

	SetupNetworking();

	while (true)
	{
		if (modEnabled)
		{
			if (shouldKillPlayer)
			{
				KillPlayer();
				shouldKillPlayer = false;
			}

			CheckPlayerDeath(wasDead);
			WAIT(100);
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		break;
	case DLL_PROCESS_DETACH:
		CleanupNetworking();
		scriptUnregister(hInstance);
		break;
	}
	return TRUE;
}