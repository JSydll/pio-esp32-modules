#include "esp32-modules/connectivity/BluetoothLE.hpp"

// Standard includes
#include <cstdint>

// Platform includes
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>

namespace Esp32Modules::Connectivity::BluetoothLE
{
namespace
{
/** */
struct BleCommand
{
  std::string token;       //!<
  std::string parameters;  //!<
};

/**
 * @brief Implements the callbacks for the BLE server itself.
 *
 */
class ServerFunctor : public BLEServerCallbacks
{
 public:
  ServerFunctor() = default;
  ~ServerFunctor() = default;

 private:
  void onConnect(BLEServer*) override
  {
    // Do something on connect
  }

  void onDisconnect(BLEServer* server) override
  {
    // Give the bluetooth stack the chance to get things ready
    delay(500);
    // Restart advertising
    server->startAdvertising();
  }
};

/**
 * @brief Implements the callbacks for the writable BLE characteristic
 *
 */
class ReceivingFunctor : public BLECharacteristicCallbacks
{
 public:
  /**
   * @brief Construct a new Receiving Functor object
   *
   * @param mutex
   * @param rxQueue
   */
  ReceivingFunctor(std::mutex& mutex, BleCommandReceiver::BleReceivingQueue& rxQueue)
      : mInterfaceMutex{mutex}, mRxQueue{rxQueue}
  {
  }
  ~ReceivingFunctor() = default;

 private:
  std::mutex& mInterfaceMutex;
  BleCommandReceiver::BleReceivingQueue& mRxQueue;

  void onWrite(BLECharacteristic* charac) override
  {
    std::string rxValue{charac->getValue()};
    if (rxValue.length() > 0)
    {
      std::lock_guard<std::mutex>{mInterfaceMutex};
      mRxQueue.emplace(std::move(rxValue));
    }
  }
};
}  // namespace

BleCommandReceiver::BleCommandReceiver(const std::string& deviceName,
                                       const std::string& serviceUuid, const std::string& rxUuid)
    : mBLEServer{}, mCallbacks{}, mRxQueue{}
{
  BLEDevice::init(deviceName);
  mBLEServer.reset(BLEDevice::createServer());
  mBLEServer->setCallbacks(new ServerFunctor());
  auto bleService = mBLEServer->createService(serviceUuid);
  auto rxCharacteristic =
      bleService->createCharacteristic(rxUuid, BLECharacteristic::PROPERTY_WRITE);
  rxCharacteristic->setCallbacks(new ReceivingFunctor(mMutex, mRxQueue));
  bleService->start();
  mBLEServer->getAdvertising()->addServiceUUID(bleService->getUUID());
  mBLEServer->getAdvertising()->start();
}

bool BleCommandReceiver::RegisterCallback(const std::string& token, const BleCommandCallback& cb)
{
  std::lock_guard<std::mutex>{mMutex};
  const auto res = mCallbacks.emplace(token, cb);
  return res.second;
}

namespace
{
/** Maps the @p cmd to the BleCommand structure. */
BleCommand GetBleCommand(const std::string& cmd)
{
  const auto splitPos = cmd.find(' ');
  return {cmd.substr(0, splitPos), cmd.substr(splitPos + 1, cmd.size())};
}
}  // namespace

void BleCommandReceiver::ProcessPendingCommands()
{
  std::queue<std::string> receivedCommands;
  {
    std::lock_guard<std::mutex>{mMutex};
    if (mRxQueue.empty())
    {
      return;
    }
    receivedCommands.swap(mRxQueue);
  }
  while (not receivedCommands.empty())
  {
    const auto cmd = receivedCommands.front();
    receivedCommands.pop();
    const auto bleCmd = GetBleCommand(cmd);
    const auto match = mCallbacks.find(bleCmd.token);
    if (match == mCallbacks.end())
    {
      continue;
    }
    match->second(bleCmd.parameters);
  }
}

}  // namespace Esp32Modules::Connectivity::BluetoothLE