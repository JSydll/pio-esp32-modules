/**
 * @file SdCard.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides a class to use an SD card using the SPI interface.
 * @version 0.1
 * @date 2021-04-13
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__FILESYSTEM_SDCARD_HPP_
#define ESP32MODULES__FILESYSTEM_SDCARD_HPP_

// Standard header
#include <cstdint>

// Platform header
#include <FS.h>
#include <SD.h>

#include "SPI.h"

namespace Esp32Modules::Filesystem
{

/**
 * @brief Represents and provides access to an SD card connected over SPI.
 *
 * Assumes the following wiring:
 *      SD Card | ESP32
 *      D2       -
 *      D3       SS
 *      CMD      MOSI
 *      VSS      GND
 *      VDD      3.3V
 *      CLK      SCK
 *      VSS      GND
 *      D0       MISO
 *      D1       -
 */
class SdCard
{
 public:
  /**
   * @brief Sets up the SD card for interactions.
   */
  SdCard();
  ~SdCard() = default;

  /**
   * @brief Indicates whether the SD card is available for reading/writing to it.
   *
   * @return true if an SD card was found, false if not.
   */
  bool IsAvailable() const;

  /**
   * @brief Retrieves the filesystem handle used to access files on the SD card.
   * 
   * @return fs::FS& Handle that can be used to open/read/write files.
   */
  fs::FS& GetFilesystemHandle();

  /**
   * @brief Provides the total size of the SD card in bytes.
   * 
   * @return Size of the SD card.
   */
  size_t GetByteSize() const;

  /**
   * @brief Shows the number of bytes left for writing data to the SD card.
   * 
   * @return Number of bytes left. 
   */
  size_t GetBytesAvailable() const;

 private:
  uint8_t mCardType;
};
}  // namespace Esp32Modules::Filesystem

#endif  // ESP32MODULES__FILESYSTEM_SDCARD_HPP_