#pragma once

#include <cstring>

/**
 * @brief 湿度データの保持構造体
 *
 * リングバッファとして使用される配列と、現在の書き込み位置（ヘッド）を管理します。
 */
struct HumidityData
{
    constexpr static size_t RECORD_SIZE = 16384; ///< 記録可能な最大データ数

    float record[RECORD_SIZE]; ///< 湿度データ配列
    int head;                  ///< 現在の書き込み位置（リングバッファのヘッド）

    /**
     * @brief コンストラクタ
     *
     * データをゼロ初期化します。
     */
    HumidityData() : record{}, head(0)
    {
    }

    /**
     * @brief インデックス演算子オーバーロード
     *
     * リングバッファとして動作するようにインデックスを計算します。
     *
     * @param index アクセスするデータのインデックス
     * @return float& 指定インデックスの湿度データへの参照
     */
    const float &operator[](size_t index) const
    {
        return record[(head - 1 - index + RECORD_SIZE) % RECORD_SIZE];
    }

    /**
     * @brief 湿度データを追加
     *
     * @param humidity 追加する湿度値
     */
    void push(float humidity)
    {
        record[head] = humidity;
        head = (head + 1) % RECORD_SIZE;
    }

    /**
     * @brief 湿度データをクリア
     */
    void clear()
    {
        memset(record, 0, sizeof(record));
        head = 0;
    }
};