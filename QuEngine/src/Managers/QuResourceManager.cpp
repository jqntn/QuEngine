#include <QuEngine/Managers/QuResourceManager.h>
#include <QuEngine/QuEngine.h>
#include <QuEngine/QuSound.h>
#include <QuEngine/QuTexture.h>

QuResourceManager::~QuResourceManager()
{
  s_Instance = nullptr;
}

std::shared_ptr<QuTexture>
QuResourceManager::GetTexture(const std::string& path,
                              const std::byte* bytes,
                              size_t size)
{
  auto it = m_Textures.find(path);
  if (it != m_Textures.end())
    return it->second;

  auto texture = std::make_shared<QuTexture>(
    bytes != nullptr ? QuTexture::LoadFromMemory(bytes, size)
                     : QuTexture::LoadFromFile(GetResDir() + path));

  it = m_Textures.emplace(path, std::move(texture)).first;
  return it->second;
}

std::shared_ptr<QuSound>
QuResourceManager::GetSound(const std::string& path,
                            const std::byte* bytes,
                            size_t size)
{
  auto it = m_Sounds.find(path);
  if (it != m_Sounds.end())
    return it->second;

  auto sound = std::make_shared<QuSound>(
    bytes != nullptr ? QuSound::LoadFromMemory(bytes, size)
                     : QuSound::LoadFromFile(GetResDir() + path));

  it = m_Sounds.emplace(path, std::move(sound)).first;
  return it->second;
}

void
QuResourceManager::PurgeUnused()
{
  for (auto it = m_Textures.begin(); it != m_Textures.end();)
    it = it->second.use_count() > 1 ? ++it : m_Textures.erase(it);
  for (auto it = m_Sounds.begin(); it != m_Sounds.end();)
    it = it->second.use_count() > 1 ? ++it : m_Sounds.erase(it);
}

QuResourceManager&
QuResourceManager::Instance()
{
  if (s_Instance == nullptr)
    s_Instance = new QuResourceManager();
  return *s_Instance;
}

std::string
QuResourceManager::GetResDir() const
{
  return GetExePath().parent_path().string() + "/res/";
}