#pragma once

#include <string>

struct Mix_Chunk;

class QuSound
{
public:
  ~QuSound();

  QuSound(QuSound&&) noexcept;
  QuSound(const QuSound&) = default;
  QuSound& operator=(QuSound&&) noexcept;
  QuSound& operator=(const QuSound&) = default;

  Mix_Chunk* GetRaw() const { return m_pSound; }
  uint32_t GetVolume() const;

  void Play(int8_t channel = -1, int32_t loops = 0);
  void SetVolume(uint8_t volume);

  static uint32_t GetVolume(int8_t channel);

  static bool IsPlaying(int8_t channel);
  static void SetVolume(int8_t channel, uint8_t volume);
  static QuSound LoadFromFile(std::string path);
  static QuSound LoadFromMemory(const std::byte* bytes, size_t size);

private:
  QuSound(Mix_Chunk* pSound);

  Mix_Chunk* m_pSound = nullptr;
};