#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#pragma warning(push)
#pragma warning(disable : 4334)
#pragma warning(disable : 4715)
#include "chess.hpp"
#pragma warning(pop)

#include "enet.h"
#include "json_fwd.hpp"

constexpr auto NETWORK_TICK = 1000 / 20;
constexpr auto MAX_WAIT = 2000;

enum class Opcode : uint8_t
{
  CREATE_ROOM,
  JOIN_ROOM,
  RECEIVE_ROOM,
  SRV_MSG,
  MOVE,
};

struct Player
{
  std::string UID;
  std::string Name;
  chess::Color Color = chess::Color::NONE;
};
void
to_json(nlohmann::json& j, const Player& o);
void
from_json(const nlohmann::json& j, Player& o);

struct Room
{
  std::string UID = "Local";
  Player P1 = { "", "P1 - Local", chess::Color::WHITE };
  Player P2 = { "", "P2 - Local", chess::Color::BLACK };
  std::vector<uint16_t> Moves;
};
void
to_json(nlohmann::json& j, const Room& o);
void
from_json(const nlohmann::json& j, Room& o);

struct JoinRequest
{
  Player P2;
  std::string UID;
};
void
to_json(nlohmann::json& j, const JoinRequest& o);
void
from_json(const nlohmann::json& j, JoinRequest& o);

ENetPacket*
BuildPacket(std::vector<uint8_t> data, Opcode opcode);

ENetPacket*
BuildPacket(std::string data, Opcode opcode);

std::pair<Opcode, std::vector<uint8_t>>
ExtractPacket(uint8_t* bytes);

std::string
GenUID();

#endif

#ifdef PROTOCOL_I_

#include "json.hpp"

void
to_json(nlohmann::json& j, const Player& o)
{
  j = { { "UID", o.UID }, { "Name", o.Name }, { "Color", o.Color } };
}
void
from_json(const nlohmann::json& j, Player& o)
{
  j.at("UID").get_to(o.UID);
  j.at("Name").get_to(o.Name);
  j.at("Color").get_to(o.Color);
}

void
to_json(nlohmann::json& j, const Room& o)
{
  j = {
    { "UID", o.UID }, { "P1", o.P1 }, { "P2", o.P2 }, { "Moves", o.Moves }
  };
}
void
from_json(const nlohmann::json& j, Room& o)
{
  j.at("UID").get_to(o.UID);
  j.at("P1").get_to(o.P1);
  j.at("P2").get_to(o.P2);
  j.at("Moves").get_to(o.Moves);
}

void
to_json(nlohmann::json& j, const JoinRequest& o)
{
  j = { { "P2", o.P2 }, { "UID", o.UID } };
}
void
from_json(const nlohmann::json& j, JoinRequest& o)
{
  j.at("P2").get_to(o.P2);
  j.at("UID").get_to(o.UID);
}

ENetPacket*
BuildPacket(std::vector<uint8_t> data, Opcode opcode)
{
  auto dataSize = (uint8_t)data.size();
  auto packetSize = sizeof(opcode) + sizeof(dataSize) + dataSize;
  auto bytes = std::vector<std::byte>(packetSize);

  memcpy(&bytes[0], &opcode, sizeof(opcode));
  memcpy(&bytes[sizeof(opcode)], &dataSize, sizeof(dataSize));
  memcpy(&bytes[sizeof(opcode) + sizeof(dataSize)], &data[0], dataSize);

  return enet_packet_create(
    bytes.data(), bytes.size(), ENET_PACKET_FLAG_RELIABLE);
}

ENetPacket*
BuildPacket(std::string data, Opcode opcode)
{
  auto bytes = std::vector<uint8_t>(data.size());
  memcpy(&bytes[0], &data[0], data.size());
  return BuildPacket(bytes, opcode);
}

std::pair<Opcode, std::vector<uint8_t>>
ExtractPacket(uint8_t* bytes)
{
  auto opcode = Opcode();
  auto dataSize = uint8_t();
  memcpy(&opcode, &bytes[0], sizeof(opcode));
  memcpy(&dataSize, &bytes[sizeof(opcode)], sizeof(dataSize));
  auto data = std::vector<uint8_t>(dataSize);
  memcpy(&data[0], &bytes[sizeof(opcode) + sizeof(dataSize)], dataSize);
  return { opcode, data };
}

std::string
GenUID()
{
  std::srand((uint32_t)std::time(nullptr));

  auto min = 0;
  auto max = 9;
  auto range = max - min + 1;

  auto uid = std::string();
  for (auto i = 0; i < 4; i++)
    uid.append(std::to_string(std::rand() % range + min));

  return uid;
}

#endif