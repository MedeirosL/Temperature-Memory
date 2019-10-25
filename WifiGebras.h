#ifndef WifiGebras_h
#define WifiGebras_h

class WifiGebras {
  private:
    static WifiGebras* instance;
    WifiGebras();
  public:
    static WifiGebras* getInstance();
    void Setup();
    void ChangeTo(String ssid_, String pass_);
    void SetData(String ssid_, String pass_);
    bool IsConnected();
    void Disconnect();
    void Connect();
    bool userDisconnected;
};
#endif
