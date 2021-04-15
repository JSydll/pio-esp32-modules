/**
 * @file Files.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides classes to interact with files.
 * @version 0.1
 * @date 2021-04-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__FILESYSTEM_FILES_HPP_
#define ESP32MODULES__FILESYSTEM_FILES_HPP_

// Standard header
#include <cstdint>
#include <map>
#include <string>
#include <vector>

// Platform header
#include <FS.h>

namespace Esp32Modules::Filesystem
{
/** @brief Represents a sequence of raw bytes. */
using Bytestream = std::vector<uint8_t>;

/**
 * @brief Represents a regular file on the filesystem and enables interactions with it.
 */
class RegularFile
{
 public:
  /**
   * @brief Prepares the regular file for interactions.
   *
   * @param fs Filesystem on which the file can be found.
   * @param path Full path name of the file.
   */
  RegularFile(fs::FS& fs, const std::string& path);
  ~RegularFile() = default;

  /**
   * @brief Reads (a number of) bytes from the file.
   *
   * @param outBytes Output from the read operation (appends to it).
   * @param numBytes Number of bytes to be read - if set to 0, all available bytes will be read.
   * @return true if the operation was successful, false otherwise.
   */
  bool ReadBytes(Bytestream& outBytes, const size_t numBytes = 0);

  /**
   * @brief Reads the contents of the file into the string buffer.
   *
   * @param contents Contents from the file.
   * @return true if the operation was successful, false otherwise.
   */
  bool Read(std::string& contents);

  /**
   * @brief Writes bytes to the file.
   *
   * @note If the file does not exist, a new file will be created.
   *
   * @param bytes Buffer containing the bytes to be written.
   * @param numBytes Number of bytes to be written - if set to 0, all available bytes will be
   * written. If the provided size is larger than the bytes available in the buffer, only the
   * available bytes will be written.
   * @param append Flag indicating whether the data shall be appended to the file - if set to false,
   * the existing data will be discarded.
   * @return true if the operation was successful, false otherwise.
   */
  bool WriteBytes(const Bytestream& bytes, const size_t numBytes = 0, const bool append = true);

  /**
   * @brief Writes the data to the file.
   *
   * @note If the file does not exist, a new file will be created. 
   * 
   * @param content File contents to be written.
   * @param append Flag indicating whether the data shall be appended to the file - if set to false,
   * the existing data will be discarded.
   * @return true if the operation was successful, false otherwise.
   */
  bool Write(const std::string& content, const bool append = true);

  /**
   * @brief Moves a file to another location (i.e. renames it).
   *
   * @param fs Filesystem on which the file can be found.
   * @param oldPath Full current path name of the file.
   * @param newPath Full desired path name of the file
   * @return true if the operation was successful, false otherwise.
   */
  static bool Move(fs::FS& fs, const std::string& oldPath, const std::string& newPath);

  /**
   * @brief Deletes the file (if existing).
   *
   * @param fs Filesystem on which the file can be found.
   * @param path Full desired path name of the file.
   * @return true if the operation was successful, false otherwise.
   */
  static bool Delete(fs::FS& fs, const std::string& path);

 private:
  fs::FS& mFS;
  std::string mPath;
};

/**
 * @brief Represents a configuration file in the INI format (one key=value per line).
 */
class IniFile
{
 public:
  /**
   * @brief Interprets the file located at @p path as a config file and populates the internal map
   * of key-value pairs.
   *
   * @param fs Filesystem on which the file can be found.
   * @param path Full desired path name of the file.
   */
  IniFile(fs::FS& fs, const std::string& path);
  ~IniFile() = default;

  /** @{ */
  /** @brief Attempts to get a typed value from the key-value-map.
   *
   * @param key
   * @param value
   * @return true if the key-value-pair was found and successfully converted, false otherwise.
   */
  bool GetValue(const std::string& key, uint16_t& value);
  bool GetValue(const std::string& key, uint32_t& value);
  bool GetValue(const std::string& key, int16_t& value);
  bool GetValue(const std::string& key, int32_t& value);
  bool GetValue(const std::string& key, float& value);
  bool GetValue(const std::string& key, std::string& value);
  /** @} */

 private:
  std::map<std::string, std::string> mKeyValuePairs;
};

}  // namespace Esp32Modules::Filesystem

#endif  // ESP32MODULES__FILESYSTEM_FILES_HPP_