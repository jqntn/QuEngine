#pragma once

#include "../../../ChessServer/src/Protocol.h"
#include <thread>

class Client
{
public:
  Client();
  ~Client();

  void CreateRoom();
  void JoinRoom();
  void SendMove(const chess::Move& move);

  std::string LastError;

  std::pair<std::string, char[10 + 1]> Username;
  char RoomToJoin[4 + 1] = "0000";
  Room Room;

  std::function<void(void)> RoomReceived;
  std::function<void(const chess::Move&)> MoveReceived;

private:
  void MakeUsername();
  std::string GetFormattedUsername();
  void Connect();
  void Listen();
  void HandleEvent(ENetEvent event, Opcode opcode, std::vector<uint8_t> data);

  ENetHost* m_pClient = nullptr;
  ENetPeer* m_pPeer = nullptr;
  std::thread* m_pThread = nullptr;

  ENetEvent m_Event{};

  bool m_IsConnected = false;
  bool m_IsListening = false;
};