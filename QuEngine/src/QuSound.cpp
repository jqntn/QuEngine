#include <QuEngine/QuSound.h>
#include <SDL2/SDL_mixer.h>
#include <stdexcept>

QuSound::QuSound(Mix_Chunk* pSound)
{
  m_pSound = pSound;
}

QuSound::~QuSound()
{
  Mix_FreeChunk(m_pSound);
}

QuSound::QuSound(QuSound&& other) noexcept
{
  std::swap(m_pSound, other.m_pSound);
}

QuSound&
QuSound::operator=(QuSound&& other) noexcept
{
  std::swap(m_pSound, other.m_pSound);
  return *this;
}

uint32_t
QuSound::GetVolume() const
{
  return Mix_VolumeChunk(m_pSound, -1);
}

void
QuSound::Play(int8_t channel, int32_t loops)
{
  if (Mix_Playing(-1) >= Mix_AllocateChannels(-1))
    return;

  if (Mix_PlayChannel(channel, m_pSound, loops) < 0)
    throw std::runtime_error("Mix_PlayChannel: " + std::string(Mix_GetError()));
}

void
QuSound::SetVolume(uint8_t volume)
{
  Mix_VolumeChunk(m_pSound, volume);
}

uint32_t
QuSound::GetVolume(int8_t channel)
{
  return Mix_Volume(channel, -1);
}

bool
QuSound::IsPlaying(int8_t channel)
{
  return Mix_Playing(channel);
}

void
QuSound::SetVolume(int8_t channel, uint8_t volume)
{
  Mix_Volume(channel, volume);
}

QuSound
QuSound::LoadFromFile(std::string path)
{
  auto pSound = Mix_LoadWAV(path.c_str());

  if (pSound == nullptr)
    throw std::runtime_error("Mix_LoadWAV: " + std::string(Mix_GetError()));

  return QuSound(pSound);
}

QuSound
QuSound::LoadFromMemory(const std::byte* bytes, size_t size)
{
  auto pRWOp = SDL_RWFromConstMem(bytes, (int32_t)size);

  if (pRWOp == nullptr)
    throw std::runtime_error("SDL_RWFromConstMem: " +
                             std::string(SDL_GetError()));

  auto pSound = Mix_LoadWAV_RW(pRWOp, 1);

  if (pSound == nullptr)
    throw std::runtime_error("Mix_LoadWAV: " + std::string(Mix_GetError()));

  return QuSound(pSound);
}