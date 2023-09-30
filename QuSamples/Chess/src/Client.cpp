#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define ENET_IMPLEMENTATION

#include <Chess/Client.h>
#include <Chess/json/json.hpp>
#include <Windows.h>
#include <lmcons.h>

using namespace std::string_literals;

Client::Client()
{
  MakeUsername();

  if (enet_initialize() < 0)
    throw std::runtime_error("An error occurred while initializing ENet.");

  m_pClient = enet_host_create(nullptr, 1, 2, 0, 0);
  if (m_pClient == nullptr)
    throw std::runtime_error(
      "An error occurred while trying to create an ENet client host.");
}

Client::~Client()
{
  if (m_IsListening) {
    m_IsListening = false;
    m_pThread->join();
    delete m_pThread;
  }

  if (m_IsConnected && m_pPeer != nullptr) {
    enet_peer_disconnect(m_pPeer, 0);

    auto disconnected = false;
    while (!disconnected &&
           enet_host_service(m_pClient, &m_Event, MAX_WAIT) > 0)
      if (m_Event.type == ENET_EVENT_TYPE_DISCONNECT)
        disconnected = true;

    enet_peer_reset(m_pPeer);
  }

  enet_host_destroy(m_pClient);
  enet_deinitialize();
}

void
Client::CreateRoom()
{
  if (!m_IsConnected)
    Connect();

  if (Room.UID == Username.first) {
    LastError = "Already in room.";
    return;
  }

  auto player = Player{ Username.first, GetFormattedUsername() };
  enet_peer_send(
    m_pPeer,
    0,
    BuildPacket(nlohmann::json::to_msgpack(player), Opcode::CREATE_ROOM));
}

void
Client::JoinRoom()
{
  if (!m_IsConnected)
    Connect();

  if (Room.UID == RoomToJoin) {
    LastError = "Already in room.";
    return;
  }

  auto joinRequest =
    JoinRequest{ Player{ Username.first, GetFormattedUsername() }, RoomToJoin };
  enet_peer_send(
    m_pPeer,
    0,
    BuildPacket(nlohmann::json::to_msgpack(joinRequest), Opcode::JOIN_ROOM));
}

void
Client::SendMove(const chess::Move& move)
{
  if (Room.UID == "Local") {
    MoveReceived(move);
    return;
  }

  auto rawMove = move.move();
  auto data = std::vector<uint8_t>(sizeof(rawMove));
  memcpy(&data[0], &rawMove, sizeof(rawMove));

  enet_peer_send(m_pPeer, 0, BuildPacket(data, Opcode::MOVE));
}

void
Client::MakeUsername()
{
  constexpr auto max_len = UNLEN + 1;
  unsigned long un_len = max_len;
  char un[max_len];
  GetUserNameA(un, &un_len);
  memcpy(Username.second, un, sizeof(Username.second));
  Username.first = GenUID();
}

std::string
Client::GetFormattedUsername()
{
  return std::format("{}#{}", Username.second, Username.first);
}

void
Client::Connect()
{
  auto address = ENetAddress{};
  enet_address_set_host(&address, "89.156.185.224");
  address.port = 7777;

  auto peer = enet_host_connect(m_pClient, &address, 2, 0);
  if (peer == nullptr) {
    LastError = "No available peers.";
    return;
  }
  m_pPeer = peer;

  if (enet_host_service(m_pClient, &m_Event, MAX_WAIT) > 0 &&
      m_Event.type == ENET_EVENT_TYPE_CONNECT) {
    m_IsConnected = true;
    LastError = "";
    Listen();
  } else {
    LastError = "Connection failed.";
    enet_peer_reset(m_pPeer);
  }
}

void
Client::Listen()
{
  m_IsListening = true;

  m_pThread = new std::thread([&]() {
    while (m_IsListening)
      if (enet_host_service(m_pClient, &m_Event, NETWORK_TICK) > 0)
        switch (m_Event.type) {
          case ENET_EVENT_TYPE_RECEIVE: {
            auto contained = ExtractPacket(m_Event.packet->data);
            HandleEvent(m_Event, contained.first, contained.second);
            enet_packet_destroy(m_Event.packet);
          } break;
        }
  });
}

void
Client::HandleEvent(ENetEvent event, Opcode opcode, std::vector<uint8_t> data)
{
  switch (opcode) {
    case Opcode::RECEIVE_ROOM: {
      Room = nlohmann::json::from_msgpack(data).get<struct Room>();
      RoomReceived();
      LastError = "";
    } break;
    case Opcode::SRV_MSG: {
      auto msg = std::string(data.size(), '0');
      memcpy(&msg[0], &data[0], data.size());
      LastError = msg;
    } break;
    case Opcode::MOVE: {
      auto rawMove = uint16_t();
      memcpy(&rawMove, &data[0], sizeof(rawMove));
      MoveReceived(chess::Move(rawMove));
      LastError = "";
    } break;
  }
}

#define PROTOCOL_I_
#include "../../ChessServer/src/Protocol.h"