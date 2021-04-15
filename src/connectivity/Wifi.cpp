#include "esp32-modules/connectivity/Wifi.hpp"

namespace Esp32Modules::Connectivity::Wifi
{

WifiConnection::WifiConnection(const std::string& ssid, const std::string& password)
{
  WiFi.begin(ssid.c_str(), password.c_str());
}

WifiConnection::~WifiConnection()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

bool WifiConnection::IsConnected() const { return (WiFi.status() != WL_CONNECTED); }

WifiAccessPoint::WifiAccessPoint(const std::string& ssid, const std::string& password)
{
  if (password.empty())
  {
    WiFi.softAP(ssid.c_str());
  }
  else
  {
    WiFi.softAP(ssid.c_str(), password.c_str());
  }
  mIp = WiFi.softAPIP();
}

WifiAccessPoint::~WifiAccessPoint()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

IPAddress WifiAccessPoint::GetAssignedIP() const { return mIp; }

}  // namespace Esp32Modules::Connectivity::Wifi