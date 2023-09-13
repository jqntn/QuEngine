#pragma once

#define WIN32_LEAN_AND_MEAN

#include <filesystem>
#include <string>

void
HelloQuEngine();

class QuEvent;

class QuEngine
{
public:
  QuEngine();
  ~QuEngine();

  void StartCounters();
  void UpdateCounters();
  bool PollEvent(const QuEvent& event);
  const float& GetDeltaTime() const { return m_DeltaTime; }

private:
  uint64_t m_Then = 0;
  uint64_t m_Now = 0;
  float m_DeltaTime = 0;
};

void
LogError(const std::string& what);

void
SaveLogs();

std::filesystem::path
GetExePath();