#pragma once

#include "button.h"
#include <Arduino.h>

/**
 * @brief 湿度読み取りのインターフェース
 *
 * 土壌湿度センサーなどから湿度値を取得するための抽象基底クラスです。
 */
class IHumidityReader
{
public:
    virtual ~IHumidityReader() = default;

    /**
     * @brief 湿度を読み取るss
     *
     * @return float 湿度値（0.0 〜 100.0 %）
     */
    virtual float readHumidity() = 0;
};

/**
 * @brief GPIO（アナログ入力）を使用した湿度読み取りの実装
 *
 * アナログピンから値を読み取り、0〜100%の湿度値に変換します。
 */
class GPIOHumidityReader final : public IHumidityReader
{
public:
    /**
     * @brief コンストラクタ
     *
     * @param sensorPin センサーが接続されているアナログピン番号
     */
    explicit GPIOHumidityReader(int sensorPin) : sensorPin(sensorPin)
    {
    }

    /**
     * @brief アナログピンから値を読み取り、湿度を計算する
     *
     * 12bit ADC (0-4095) を想定して計算しています。
     *
     * @return float 湿度値（%）
     */
    float readHumidity() override
    {
        const int sensorValue = analogRead(sensorPin);
        return static_cast<float>(sensorValue) / 4096.0f * 100.0f;
    }

private:
    int sensorPin; ///< センサーピン番号
};

/**
 * @brief テスト用のモック湿度リーダー
 *
 * ランダムな湿度値を返します。センサーがない環境でのテストに使用します。
 */
class MockHumidityReader final : public IHumidityReader
{
public:
    /**
     * @brief ランダムな湿度値を生成して返す
     *
     * @return float ランダムな湿度値（0.0 〜 100.0 %）
     */
    float readHumidity() override
    {
        button.update();
        if (button.wasPressed())
        {
            currentHumidity += 10.0f;
            if (currentHumidity > 100.0f)
            {
                currentHumidity = 0.0f;
            }
        }
        return currentHumidity;
    }

private:
    Button button = Button(D1);    ///< 湿度変動をトリガーするためのボタン
    float currentHumidity = 50.0f; ///< 現在の湿度値
};