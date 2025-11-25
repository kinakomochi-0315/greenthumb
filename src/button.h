#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

/**
 * @brief ボタン入力を管理するクラス
 *
 * チャタリング除去（デバウンス）や長押し判定などの機能を提供します。
 */
class Button
{
private:
    uint8_t pin;                ///< ボタンが接続されているGPIOピン番号
    bool lastState;             ///< 前回のボタンの状態 (HIGH/LOW)
    bool currentState;          ///< 現在のボタンの状態 (HIGH/LOW)
    uint32_t pressStartTime;    ///< ボタンが押され始めた時刻 (millis)
    uint32_t longPressDuration; ///< 長押しと判定するまでの時間 (ms)
    bool longPressTriggered;    ///< 長押しイベントが既に発火したかどうか

public:
    /**
     * @brief コンストラクタ
     *
     * @param buttonPin ボタンが接続されているGPIOピン番号
     * @param longPressMs 長押しと判定するまでの時間（ミリ秒）。デフォルトは3000ms。
     */
    Button(uint8_t buttonPin, uint32_t longPressMs = 3000)
        : pin(buttonPin), lastState(HIGH), currentState(HIGH), pressStartTime(0), longPressDuration(longPressMs),
          longPressTriggered(false)
    {
    }

    /**
     * @brief ボタンの初期化
     *
     * ピンモードをINPUT_PULLUPに設定し、初期状態を読み取ります。
     * setup() 関数内で呼び出してください。
     */
    void begin()
    {
        pinMode(pin, INPUT_PULLUP);
        currentState = digitalRead(pin);
        lastState = currentState;
    }

    /**
     * @brief ボタンの状態を更新
     *
     * loop() 関数内で定期的に呼び出してください。
     * 現在のボタンの状態を読み取り、長押し判定などを行います。
     */
    void update()
    {
        lastState = currentState;
        currentState = digitalRead(pin);

        // ボタンが押され始めた
        if (currentState == LOW && lastState == HIGH)
        {
            pressStartTime = millis();
            longPressTriggered = false;
        }

        // 長押し判定（オーバーフロー対応の差分計算）
        if (currentState == LOW && !longPressTriggered)
        {
            uint32_t elapsed = millis() - pressStartTime;
            if (elapsed >= longPressDuration)
            {
                longPressTriggered = true;
            }
        }
    }

    /**
     * @brief ボタンが現在押されているかを確認
     *
     * @return true ボタンが押されている場合
     * @return false ボタンが離されている場合
     */
    bool isPressed() const
    {
        return currentState == LOW;
    }

    /**
     * @brief ボタンが押された瞬間かどうかを確認（立ち下がりエッジ）
     *
     * @return true ボタンが押された瞬間の場合
     * @return false それ以外の場合
     */
    bool wasPressed() const
    {
        return currentState == LOW && lastState == HIGH;
    }

    /**
     * @brief ボタンが離された瞬間かどうかを確認（立ち上がりエッジ）
     *
     * @return true ボタンが離された瞬間の場合
     * @return false それ以外の場合
     */
    bool wasReleased() const
    {
        return currentState == HIGH && lastState == LOW;
    }

    /**
     * @brief 長押しされたかどうかを確認
     *
     * 長押し判定時間を超えてボタンが押され続けている場合、一度だけtrueを返します。
     *
     * @return true 長押しが検知された場合
     * @return false それ以外の場合
     */
    bool wasLongPressed() const
    {
        return longPressTriggered && currentState == LOW;
    }

    /**
     * @brief 長押し判定時間を設定
     *
     * @param ms 長押しと判定するまでの時間（ミリ秒）
     */
    void setLongPressDuration(uint32_t ms)
    {
        longPressDuration = ms;
    }
};

#endif // BUTTON_H
