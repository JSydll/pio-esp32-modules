#include "esp32-modules/filesystem/Files.hpp"

// Standard header
#include <cstdlib>
#include <sstream>

namespace Esp32Modules::Filesystem
{
// -----------
// RegularFile
// -----------

RegularFile::RegularFile(fs::FS& fs, const std::string& path) : mFS{fs}, mPath{path} {}

bool RegularFile::ReadBytes(Bytestream& outBytes, const size_t numBytes)
{

  File file = mFS.open(mPath.c_str(), FILE_READ);
  if (not file)
  {
    return false;
  }
  const size_t bytesAvailable = file.available();
  const bool toEnd = (numBytes == 0) or (numBytes > bytesAvailable);
  const size_t bytesToRead = (toEnd ? bytesAvailable : numBytes);

  outBytes.resize(outBytes.size() + bytesToRead);
  const size_t bytesRead = file.read(outBytes.data(), bytesToRead);

  file.close();
  return (bytesRead == bytesToRead);
}

bool RegularFile::Read(std::string& contents)
{
  File file = mFS.open(mPath.c_str(), FILE_READ);
  if (not file or not file.available())
  {
    return false;
  }
  while (file.available())
  {
    auto result = file.read();
    if ((result == -1))
    {
      return false;
    }
    contents.push_back(result);
  }
  file.close();
  return true;
}

bool RegularFile::WriteBytes(const Bytestream& bytes, const size_t numBytes, const bool append)
{
  File file = mFS.open(mPath.c_str(), (append ? FILE_APPEND : FILE_WRITE));
  if (not file)
  {
    return false;
  }
  const size_t bytesAvailable = bytes.size();
  const bool toEnd = (numBytes == 0) or (numBytes > bytesAvailable);
  const size_t bytesToWrite = (toEnd ? bytesAvailable : numBytes);

  const auto bytesWritten = file.write(bytes.data(), bytesToWrite);

  file.close();
  return (bytesWritten == bytesToWrite);
}

bool RegularFile::Write(const std::string& data, const bool append)
{
  return WriteBytes(Bytestream{data.data(), data.data() + data.size()}, append);
}

bool RegularFile::Move(fs::FS& fs, const std::string& oldPath, const std::string& newPath)
{
  return fs.rename(oldPath.c_str(), newPath.c_str());
}

bool RegularFile::Delete(fs::FS& fs, const std::string& path) { return fs.remove(path.c_str()); }

// -----------
// IniFile
// -----------

namespace
{
std::vector<std::string> Split(const std::string& str, const char delimiter)
{
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream stream(str);
  while (std::getline(stream, token, delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}
}  // namespace

IniFile::IniFile(fs::FS& fs, const std::string& path)
{
  RegularFile file{fs, path};
  std::string contents;
  file.Read(contents);
  const auto lines = Split(contents, '\n');
  for (const auto& line : lines)
  {
    const auto tokens = Split(line, '=');
    if (tokens.size() != 2)
    {
      continue;
    }
    mKeyValuePairs[tokens[0]] = tokens[1];
  }
}

bool IniFile::GetValue(const std::string& key, uint16_t& value)
{
  const auto it = mKeyValuePairs.find(key);
  if (it == mKeyValuePairs.end())
  {
    return false;
  }
  value = std::atoi(it->second.c_str());
  return true;
}

bool IniFile::GetValue(const std::string& key, uint32_t& value)
{
  const auto it = mKeyValuePairs.find(key);
  if (it == mKeyValuePairs.end())
  {
    return false;
  }
  value = std::atol(it->second.c_str());
  return true;
}

bool IniFile::GetValue(const std::string& key, int16_t& value)
{
  uint16_t raw;
  if (!GetValue(key, raw))
  {
    return false;
  }
  value = static_cast<int16_t>(raw);
  return true;
}

bool IniFile::GetValue(const std::string& key, int32_t& value)
{
  uint32_t raw;
  if (!GetValue(key, raw))
  {
    return false;
  }
  value = static_cast<int32_t>(raw);
  return true;
}

bool IniFile::GetValue(const std::string& key, float& value)
{
  const auto it = mKeyValuePairs.find(key);
  if (it == mKeyValuePairs.end())
  {
    return false;
  }
  value = std::atof(it->second.c_str());
  return true;
}

bool IniFile::GetValue(const std::string& key, std::string& value)
{
  const auto it = mKeyValuePairs.find(key);
  if (it == mKeyValuePairs.end())
  {
    return false;
  }
  value = it->second;
  return true;
}

}  // namespace Esp32Modules::Filesystem