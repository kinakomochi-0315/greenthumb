#include "greenthumb_app.h"

void GreenThumbApp::begin()
{
    // ボタンの初期化
    button.begin();

    // OLEDの初期化
    oled.initDisplay();
    oled.setPowerSave(0);
    oled.clearDisplay();

    // 過去のログを読み込み
    recorder.load(data);
}

void GreenThumbApp::update()
{
    static uint32_t lastRecordTime = 0;
    static uint32_t lastDisplayTime = 0;

    float humidity = reader.readHumidity();

    // ポンプ制御
    if (showldStartWatering(humidity))
    {
        // ポンプの稼働を開始
        pumpController.turnOn();
    }
    else if (showldStopWatering(humidity))
    {
        // ポンプの稼働を終了
        pumpController.turnOff();
        lastWateringTime = millis();
    }

    // ボタンの状態更新
    button.update();

    // 長押し検知でリセット
    if (button.wasLongPressed())
    {
        resetHumidityData();
    }

    // 記録処理（オーバーフロー対応の差分計算）
    uint32_t currentTime = millis();
    if (currentTime - lastRecordTime >= RECORD_INTERVAL)
    {
        data.record[data.head] = humidity;
        data.head = (data.head + 1) % HumidityData::RECORD_SIZE;

        // ログの保存
        recorder.save(data);

        lastRecordTime = currentTime;
    }

    // 表示処理（オーバーフロー対応の差分計算）
    if (currentTime - lastDisplayTime >= DISPLAY_INTERVAL)
    {
        oled.clearBuffer();

        int w = oled.getDisplayWidth();
        int h = oled.getDisplayHeight();
        drawHumidityValue(0, 22, humidity);

        if (pumpController.isOn())
        {
            drawWateringView(0, 25, w, h - 25);
        }
        else
        {
            drawHumidityGraph(0, 25, w, h - 25);
        }

        oled.sendBuffer();

        lastDisplayTime = currentTime;
    }
}

inline bool GreenThumbApp::showldStartWatering(float humidity) const
{
    return humidity < PUMP_ON_THRESHOLD &&                   // ポンプ起動閾値よりも現在の土壌水分が少ない
           !pumpController.isOn() &&                         // ポンプが起動していない
           millis() - lastWateringTime >= PUMP_MIN_INTERVAL; // 最後に水やりした時間から一定時間経過している
}

inline bool GreenThumbApp::showldStopWatering(float humidity) const
{
    return humidity >= PUMP_OFF_THRESHOLD && pumpController.isOn();
}

void GreenThumbApp::drawHumidityValue(const int x, const int y, const float humidity)
{
    char humStr[16];
    sprintf(humStr, "%.1f", humidity);

    // 大きいフォントで湿度値、小さいフォントで%を表示
    oled.setFont(u8g2_font_logisoso22_tn);
    oled.drawStr(x, y, humStr);

    const int humW = oled.getStrWidth(humStr);
    oled.setFont(u8g2_font_logisoso16_tr);
    oled.drawStr(x + humW + 2, y, "%");
}

void GreenThumbApp::drawHumidityGraph(const int x, const int y, const int w, const int h)
{
    // 表示範囲内の最大値・最小値を取得
    float minVal = 100.0f;
    float maxVal = 0.0f;
    for (int i = 0; i < w; i++)
    {
        float value = data[i];

        if (value > maxVal)
        {
            maxVal = value;
        }
        if (value < minVal)
        {
            minVal = value;
        }
    }

    float range = maxVal - minVal;

    // グラフの描画
    int prevX, prevY;

    for (int i = 0; i < w; i++)
    {
        float value = data[i];
        int currentX = w - 1 - i;
        int currentY;

        if (range == 0)
        {
            currentY = y + h / 2;
        }
        else
        {
            float normalized = (value - minVal) / range;
            currentY = y + (h - 1) - (int)(normalized * (h - 1));
        }

        if (i != 0)
        {
            oled.drawLine(prevX, prevY, currentX, currentY);
        }
        prevX = currentX;
        prevY = currentY;
    }
}

void GreenThumbApp::drawWateringView(const int x, const int y, const int w, const int h)
{
    oled.setFont(u8g2_font_profont17_tf);
    const char *text = "watering...";
    int textWidth = oled.getStrWidth(text);
    int centerX = x + (w - textWidth) / 2;
    oled.drawStr(centerX, y + h / 2, text);
}

void GreenThumbApp::resetHumidityData()
{
    // データをリセット
    data.clear();
    // ログの保存
    recorder.save(data);
}