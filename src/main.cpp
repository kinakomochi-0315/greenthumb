/**
 * @file main.cpp
 * @brief GreenThumb アプリケーションのエントリーポイント
 *
 * Arduinoフレームワークを使用したセットアップとメインループを定義します。
 */

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <U8g2lib.h>

#include "greenthumb_app.h"
#include "humidity_reader.h"
#include "humidity_recorder.h"
#include "pump_controller.h"

constexpr uint8_t SENSOR_PIN = D0;       ///< 湿度センサーのアナログピン
constexpr uint8_t SD_CS_PIN = D2;        ///< SDカードモジュールのCSピン
constexpr uint8_t PUMP_CONTROL_PIN = D3; ///< ポンプ制御用GPIOピン

typedef U8G2_SSD1306_128X64_NONAME_F_HW_I2C U8G2_OLED;

U8G2_OLED oled(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
GPIOHumidityReader humidityReader(SENSOR_PIN);
SDHumidityRecorder humidityRecorder(SD);
GPIOPumpController pumpController(PUMP_CONTROL_PIN);

GreenThumbApp app(humidityReader, humidityRecorder, pumpController, oled);

/**
 * @brief 初期化処理
 *
 * シリアル通信、SDカード、アプリケーションの初期化を行います。
 */
void setup()
{
    Serial.begin(115200);

    // SDカードの初期化
    pinMode(SD_CS_PIN, OUTPUT);
    SD.begin(SD_CS_PIN);

    // アプリケーションの初期化
    app.begin();
}

/**
 * @brief メインループ
 *
 * アプリケーションの更新処理を継続的に呼び出します。
 */
void loop()
{
    app.update();
}