#include "esp32-modules/connectivity/Wifi.hpp"

namespace Esp32Modules::Connectivity::Wifi
{

void Radio::Disable()
{
  WiFi.disconnect(true);  // Disconnect from the network
  WiFi.mode(WIFI_OFF);    // Switch WiFi off
}

void Radio::Enable(const WiFiMode_t mode)
{
  WiFi.disconnect(false);
  WiFi.mode(mode);
}

WifiConnection::WifiConnection(const std::string& ssid, const std::string& password,
                               const std::string& hostname)
{
  Radio::Enable(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.begin(ssid.c_str(), password.c_str());
  WiFi.setHostname(hostname.c_str());
}

WifiConnection::~WifiConnection() { Radio::Disable(); }

bool WifiConnection::IsConnected() const { return (WiFi.status() != WL_CONNECTED); }

WifiAccessPoint::WifiAccessPoint(const std::string& ssid, const std::string& password)
{
  Radio::Enable(WIFI_AP);
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

WifiAccessPoint::~WifiAccessPoint() { Radio::Disable(); }

IPAddress WifiAccessPoint::GetAssignedIP() const { return mIp; }

}  // namespace Esp32Modules::Connectivity::Wifi