/**
 * @file BluetoothLE.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides classes to interact with the ESP32 BLE stack.
 * @version 0.1
 * @date 2021-02-07
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__CONNECTIVITY_BLUETOOTHLE_HPP_
#define ESP32MODULES__CONNECTIVITY_BLUETOOTHLE_HPP_

// Standard header
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

// Platform header
#include <BLEServer.h>

namespace Esp32Modules::Connectivity::BluetoothLE
{
/**
 * @brief Sets up a BLE service to receive commands and dispatch them to callbacks registered by the
 * application.
 *
 * The created BLE service announces a single writable characteristic for external peers to send
 * commands to the device. A command consists of a token followed by a space and optionally
 * parameters. The raw string of parameters will be passed to the registered callback.
 *
 * Note that the BLE device runs asynchronously in the background and pushes received payloads to a
 * thread-safe queue. The application is responsible to regularily check for updates by calling the
 * ProcessPendingCommands() method.
 *
 */
class BleCommandReceiver
{
 public:
  /**
   * @brief Sets up the BLE device, the writable, receiving characteristic and starts announcing.
   *
   * @param deviceName Name of the BLE device (visible on the interface).
   * @param serviceUuid Uuid of the BLE service (visible on the interface).
   * @param rxUuid Uuid of the writable, receiving BLE characteristic (visible on the interface).
   */
  BleCommandReceiver(const std::string& deviceName, const std::string& serviceUuid,
                     const std::string& rxUuid);
  ~BleCommandReceiver() = default;

  /** Type of the receiving queue (not thread-safe). */
  using BleReceivingQueue = std::queue<std::string>;

  /** Type of a callback triggered when processing pending commands. */
  using BleCommandCallback = std::function<void(const std::string&)>;

  /**
   * @brief Registers a new callback for a given command token.
   *
   * @note Thread-safe.
   *
   * @param token Token by which the command is identified.
   * @param cb Callback to be called when processing pending commands for the token.
   * @return true if the callback was registered sucessfully, false otherwise (e.g. if the token was
   * already registered).
   */
  bool RegisterCallback(const std::string& token, const BleCommandCallback& cb);

  /**
   * @brief Process any pending commands received since the last call.
   *
   * @note Thread-safe.
   */
  void ProcessPendingCommands();

 private:
  std::unique_ptr<BLEServer> mBLEServer;  //!< Underlying BLE server providing the characteristic.
  std::mutex mMutex;  //!< Mutex to make the receiving queue and callbacks thread-safe.
  std::map<std::string, BleCommandCallback>
      mCallbacks;              //!< List of known callbacks per command token.
  BleReceivingQueue mRxQueue;  //!< Receiving queue.
};

}  // namespace Esp32Modules::Connectivity::BluetoothLE

#endif  // ESP32MODULES__CONNECTIVITY_BLUETOOTHLE_HPP_
