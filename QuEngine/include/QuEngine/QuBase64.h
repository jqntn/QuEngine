#pragma once

#include <array>
#include <string>
#include <vector>

std::array<int32_t, 0xFF>
b64_table()
{
  auto table = std::array<int32_t, 0xFF>();
  for (auto i = 0; i < 0xFF; i++)
    table[i] = -1;
  for (auto i = 0; i < 64; i++)
    table
      ["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] =
        i;
  return table;
}

constexpr size_t
b64_len_comptime(const char* b64)
{
  auto len = std::char_traits<char>::length(b64);
  if (b64[len - 2] == '=')
    return (len / 4) * 3 - 2;
  else if (b64[len - 1] == '=')
    return (len / 4) * 3 - 1;
  else
    return (len / 4) * 3;
}

template<size_t len>
constexpr std::array<std::byte, len>
b46_dec_comptime(const char* b64)
{
  auto table = b64_table();
  auto out = std::array<std::byte, len>();
  auto off = -8;
  for (auto i = 0, val = 0, pos = 0;
       i < std::char_traits<char>::length(b64) && table[b64[i]] > -1;
       i++) {
    val = (val << 6) + table[b64[i]];
    off += 6;
    if (off > -1) {
      out[pos++] = std::byte((val >> off) & 0xFF);
      off -= 8;
    }
  }
  return out;
}

std::vector<std::byte>
b46_dec_runtime(const char* b64)
{
  auto table = b64_table();
  auto out = std::vector<std::byte>();
  auto off = -8;
  for (auto i = 0, val = 0;
       i < std::char_traits<char>::length(b64) && table[b64[i]] > -1;
       i++) {
    val = (val << 6) + table[b64[i]];
    off += 6;
    if (off > -1) {
      out.push_back(std::byte((val >> off) & 0xFF));
      off -= 8;
    }
  }
  return out;
}

#define b64_bin_comptime(x) b46_dec_comptime<b64_len_comptime(x)>(x)
#define b64_bin_runtime(x) b46_dec_runtime(x)