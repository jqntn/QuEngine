#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class QuTexture;
class QuSound;

class QuResourceManager
{
public:
  ~QuResourceManager();

  QuResourceManager(const QuResourceManager&) = delete;
  QuResourceManager(QuResourceManager&&) = delete;

  QuResourceManager& operator=(const QuResourceManager&) = delete;
  QuResourceManager& operator=(QuResourceManager&&) = delete;

  std::shared_ptr<QuTexture> GetTexture(const std::string& path,
                                        const std::byte* bytes = nullptr,
                                        size_t size = -1);
  std::shared_ptr<QuSound> GetSound(const std::string& path,
                                    const std::byte* bytes = nullptr,
                                    size_t size = -1);

  void PurgeUnused();

  static QuResourceManager& Instance();

private:
  QuResourceManager() = default;

  std::string GetResDir() const;

  inline static QuResourceManager* s_Instance;

  std::unordered_map<std::string, std::shared_ptr<QuTexture>> m_Textures;
  std::unordered_map<std::string, std::shared_ptr<QuSound>> m_Sounds;
};