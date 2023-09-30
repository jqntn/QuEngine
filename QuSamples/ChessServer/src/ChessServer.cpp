#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define ENET_IMPLEMENTATION
#define PROTOCOL_I_

#include "Protocol.h"

#pragma warning(push)
#pragma warning(disable : 4477)

using namespace std::string_literals;

std::unordered_map<std::string, Room> Rooms;
std::unordered_map<std::string, std::string> UserToRoom;
std::unordered_map<std::string, ENetPeer*> UserToPeer;
std::unordered_map<ENetPeer*, std::string> PeerToUser;

Room
SwapRoom(const Room& room)
{
  auto swapped = Room(room);
  std::swap(swapped.P1, swapped.P2);
  return swapped;
}

void
DisconnectPlayer(ENetEvent event)
{
  {
    auto it = PeerToUser.find(event.peer);
    if (it == PeerToUser.end())
      return;
  }
  auto& user = PeerToUser.at(event.peer);
  auto& room = Rooms.at(UserToRoom.at(user));
  if (user == room.P1.UID) {
    room.P1 = Player();
    std::swap(room.P1, room.P2);
  } else if (user == room.P2.UID)
    room.P2 = Player();
  auto it = UserToPeer.find(room.P1.UID);
  auto otherPeer = it != UserToPeer.end() ? (*it).second : nullptr;

  if (otherPeer != nullptr)
    enet_peer_send(
      otherPeer,
      0,
      BuildPacket(nlohmann::json::to_msgpack(room), Opcode::RECEIVE_ROOM));

  if (room.P1.UID.empty() && room.P2.UID.empty())
    Rooms.erase(UserToRoom.at(user));
  UserToRoom.erase(user);
  UserToPeer.erase(user);
  PeerToUser.erase(event.peer);
}

void
HandleEvent(ENetEvent event, Opcode opcode, std::vector<uint8_t> data)
{
  using namespace nlohmann;

  switch (opcode) {
    case Opcode::CREATE_ROOM: {
      DisconnectPlayer(event);
      auto p1 = json::from_msgpack(data).get<struct Player>();
      auto room = Room{ p1.UID,
                        Player{ p1.UID, p1.Name, chess::Color::WHITE },
                        Player() };
      auto packet = BuildPacket(json::to_msgpack(room), Opcode::RECEIVE_ROOM);
      enet_peer_send(event.peer, 0, packet);
      Rooms.emplace(p1.UID, std::move(room));
      UserToRoom.emplace(p1.UID, p1.UID);
      UserToPeer.emplace(p1.UID, event.peer);
      PeerToUser.emplace(event.peer, p1.UID);
    } break;
    case Opcode::JOIN_ROOM: {
      auto joinRequest = json::from_msgpack(data).get<struct JoinRequest>();
      auto it = Rooms.find(joinRequest.UID);
      if (it == Rooms.end()) {
        enet_peer_send(
          event.peer, 0, BuildPacket("Room not found.", Opcode::SRV_MSG));
        return;
      }
      auto& room = (*it).second;
      if (!room.P2.UID.empty()) {
        enet_peer_send(
          event.peer, 0, BuildPacket("Room full.", Opcode::SRV_MSG));
        return;
      }
      DisconnectPlayer(event);
      room.P2 = joinRequest.P2;
      room.P2.Color = room.P1.Color == chess::Color::WHITE
                        ? chess::Color::BLACK
                        : chess::Color::WHITE;
      enet_peer_send(UserToPeer.at(room.P1.UID),
                     0,
                     BuildPacket(json::to_msgpack(room), Opcode::RECEIVE_ROOM));
      enet_peer_send(
        event.peer,
        0,
        BuildPacket(json::to_msgpack(SwapRoom(room)), Opcode::RECEIVE_ROOM));
      UserToRoom.emplace(joinRequest.P2.UID, room.UID);
      UserToPeer.emplace(joinRequest.P2.UID, event.peer);
      PeerToUser.emplace(event.peer, joinRequest.P2.UID);
    } break;
    case Opcode::MOVE: {
      auto& room = Rooms.at(UserToRoom.at(PeerToUser.at(event.peer)));
      auto rawMove = uint16_t();
      memcpy(&rawMove, &data[0], sizeof(rawMove));
      room.Moves.push_back(rawMove);
      if (rawMove == chess::MAX_MOVES)
        std::swap(room.P1.Color, room.P2.Color);
      {
        auto it = UserToPeer.find(room.P1.UID);
        if (it != UserToPeer.end())
          enet_peer_send((*it).second, 0, BuildPacket(data, Opcode::MOVE));
      }
      {
        auto it = UserToPeer.find(room.P2.UID);
        if (it != UserToPeer.end())
          enet_peer_send((*it).second, 0, BuildPacket(data, Opcode::MOVE));
      }
    } break;
  }
}

int
main()
{
  if (enet_initialize() < 0) {
    fprintf(stderr, "An error occurred while initializing ENet.\n");
    return EXIT_FAILURE;
  }

  auto address = ENetAddress{ ENET_HOST_ANY, 7777 };
  auto server = enet_host_create(&address, 64, 2, 0, 0);
  if (server == nullptr) {
    fprintf(stderr,
            "An error occurred while trying to create an ENet server host.\n");
    return EXIT_FAILURE;
  }

  auto event = ENetEvent();
  while (true)
    if (enet_host_service(server, &event, NETWORK_TICK) > 0)
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT: {
          fprintf(stdout,
                  "%hx:%hu - Peer Connected\n",
                  event.peer->address.host,
                  event.peer->address.port);
        } break;
        case ENET_EVENT_TYPE_DISCONNECT: {
          fprintf(stdout,
                  "%hx:%hu - Peer Disconnected\n",
                  event.peer->address.host,
                  event.peer->address.port);
          DisconnectPlayer(event);
        } break;
        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
          fprintf(stdout,
                  "%hx:%hu - Peer Disconnected (timeout)\n",
                  event.peer->address.host,
                  event.peer->address.port);
          DisconnectPlayer(event);
        } break;
        case ENET_EVENT_TYPE_RECEIVE: {
          auto contained = ExtractPacket(event.packet->data);
          HandleEvent(event, contained.first, contained.second);
          fprintf(stdout,
                  "%hx:%hu - Packet Received: size: %zi, opcode: %hhu\n",
                  event.peer->address.host,
                  event.peer->address.port,
                  event.packet->dataLength,
                  contained.first);
          enet_packet_destroy(event.packet);
        } break;
      }

  enet_host_destroy(server);
  enet_deinitialize();
  return EXIT_SUCCESS;
}