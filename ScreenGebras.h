#ifndef ScreenGebras_h
#define ScreenGebras_h

class ScreenGebras {
  private:
    static ScreenGebras* instance;
    ScreenGebras();
    String simplemsg;
    String date;
    String time;
    double current;
    double pulse;
    double slider;
    float output;
    float outputFrequency;
    String title;
    String bottom;
    String middle;
    String top;
    String msg;
  public:
    static ScreenGebras* getInstance();
    void Setup();
    void Clear();
    void Reboot();
    void Teste(long millis);
    void Display(String simplemsg_);
    void Current(String date_, String time_, double current_);
    void Pulse(String date_, String time_, double pulse_);
    void Pulse2(String date_, String time_, double pulse_, int pulseCount);
    void Output(String date_, String time_, float output_, float outputFrequency);
    void Slider(String date_, String time_, int slider_);
    void Menu(String title_, String bottom_, String middle_, String top_);
    void P(int p1, bool isStart);
};
#endif
