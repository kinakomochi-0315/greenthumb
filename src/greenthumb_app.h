#pragma once

#include "button.h"
#include "humidity_data.h"
#include "humidity_reader.h"
#include "humidity_recorder.h"
#include "pump_controller.h"
#include <Arduino.h>
#include <U8g2lib.h>

/**
 * @brief GreenThumb アプリケーションのメインクラス
 *
 * センサーからの読み取り、データの記録、OLEDディスプレイへの表示、
 * ユーザー入力（ボタン）の処理など、アプリケーション全体のロジックを統括します。
 */
class GreenThumbApp final
{
public:
    /**
     * @brief コンストラクタ
     *
     * @param reader 湿度リーダーへの参照
     * @param recorder 湿度レコーダーへの参照
     * @param oled OLEDディスプレイオブジェクトへの参照
     */
    GreenThumbApp(IHumidityReader &reader, IHumidityRecorder &recorder, IPumpController &pumpController, U8G2 &oled)
        : reader(reader), recorder(recorder), pumpController(pumpController), oled(oled), button(USR_BTN_PIN), data()
    {
    }

    ~GreenThumbApp() = default;

    /**
     * @brief アプリケーションの初期化
     *
     * 依存コンポーネントの初期化や、保存されたデータの読み込みを行います。
     * setup() 関数内で呼び出してください。
     */
    void begin();

    /**
     * @brief アプリケーションの更新処理
     *
     * センサー読み取り、画面更新、ボタン入力処理などを定期的に行います。
     * loop() 関数内で呼び出してください。
     */
    void update();

private:
    constexpr static uint8_t USR_BTN_PIN = D1;                 ///< ユーザーボタンのピン番号
    constexpr static uint32_t RECORD_INTERVAL = 5 * 60 * 1000; ///< データ記録間隔（5分）
    constexpr static uint32_t DISPLAY_INTERVAL = 2000;         ///< ディスプレイ更新間隔（2秒）
    constexpr static uint32_t PUMP_MIN_INTERVAL = 3 * 24 * 60 * 60 * 1000; ///< ポンプ再稼働までの最短クールタイム（3日）
    constexpr static float PUMP_ON_THRESHOLD = 5.0f;           ///< ポンプを作動させる湿度閾値 (%)
    constexpr static float PUMP_OFF_THRESHOLD = 75.0f;         ///< ポンプを停止させる湿度閾値 (%)

    IHumidityReader &reader;         ///< 湿度リーダー
    IHumidityRecorder &recorder;     ///< 湿度レコーダー
    IPumpController &pumpController; ///< ポンプコントローラー
    U8G2 &oled;                      ///< OLEDディスプレイ
    Button button;                   ///< ボタンコントローラー
    HumidityData data;               ///< 湿度データ

    uint32_t lastWateringTime = 0; ///< 最後にポンプを作動させた時間（ミリ秒）

    /**
     * @brief ポンプを稼働開始させるかどうか判定する
     */
    inline bool showldStartWatering(float humidity) const;

    /**
     * @brief ポンプを稼働停止させるかどうか判定する
     */
    inline bool showldStopWatering(float humidity) const;

    /**
     * @brief 現在の湿度値を画面に描画する
     *
     * @param x 描画開始X座標
     * @param y 描画開始Y座標
     * @param humidity 湿度値
     */
    void drawHumidityValue(const int x, const int y, const float humidity);

    /**
     * @brief 湿度の履歴グラフを描画する
     *
     * @param x 描画領域の左上X座標
     * @param y 描画領域の左上Y座標
     * @param w 描画領域の幅
     * @param h 描画領域の高さ
     */
    void drawHumidityGraph(const int x, const int y, const int w, const int h);

    /**
     * @brief ポンプ作動中の画面を描画する
     *
     * @param x 描画領域の左上X座標
     * @param y 描画領域の左上Y座標
     * @param w 描画領域の幅
     * @param h 描画領域の高さ
     */
    void drawWateringView(const int x, const int y, const int w, const int h);

    /**
     * @brief 湿度データをリセットする
     *
     * メモリ上のデータとSDカード上のデータをクリアします。
     */
    void resetHumidityData();
};