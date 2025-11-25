#pragma once

#include <Arduino.h>

/**
 * @brief ポンプ制御のインターフェース
 *
 * ポンプのON/OFF操作と状態確認を行うための抽象基底クラスです。
 */
class IPumpController
{
public:
    virtual ~IPumpController() = default;

    /**
     * @brief ポンプを作動させる
     */
    virtual void turnOn() = 0;

    /**
     * @brief ポンプを停止させる
     */
    virtual void turnOff() = 0;

    /**
     * @brief ポンプが作動中かどうかを確認する
     *
     * @return true 作動中
     * @return false 停止中
     */
    virtual bool isOn() = 0;
};

/**
 * @brief GPIOを使用したポンプ制御の実装
 *
 * 指定されたGPIOピンのHIGH/LOWを切り替えることでポンプを制御します。
 */
class GPIOPumpController final : public IPumpController
{
public:
    /**
     * @brief コンストラクタ
     *
     * @param controlPin ポンプ制御に使用するGPIOピン番号
     */
    explicit GPIOPumpController(int controlPin) : controlPin(controlPin), state(false)
    {
        pinMode(controlPin, OUTPUT);
        digitalWrite(controlPin, LOW);
    }

    /**
     * @brief ポンプを作動させる（ピンをHIGHにする）
     */
    void turnOn() override
    {
        digitalWrite(controlPin, HIGH);
        state = true;
    }

    /**
     * @brief ポンプを停止させる（ピンをLOWにする）
     */
    void turnOff() override
    {
        digitalWrite(controlPin, LOW);
        state = false;
    }

    /**
     * @brief ポンプの状態を取得する
     *
     * @return true HIGH（作動中）
     * @return false LOW（停止中）
     */
    bool isOn() override
    {
        return state;
    }

private:
    int controlPin; ///< 制御ピン番号
    bool state;     ///< 現在の状態キャッシュ
};