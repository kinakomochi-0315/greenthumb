#pragma once

#include "humidity_data.h"
#include <Arduino.h>
#include <FS.h>
#include <SD.h>

/**
 * @brief 湿度データの記録・読み出しを行うインターフェース
 *
 * 湿度データの永続化（保存と復元）を担当します。
 */
class IHumidityRecorder
{
public:
    virtual ~IHumidityRecorder() = default;

    /**
     * @brief 湿度データを保存する
     *
     * @param data 保存する湿度データ
     * @return true 保存成功
     * @return false 保存失敗
     */
    virtual bool save(const HumidityData &data) = 0;

    /**
     * @brief 湿度データを読み込む
     *
     * @param[out] data 読み込んだデータを格納する湿度データ構造体
     * @return true 読み込み成功
     * @return false 読み込み失敗
     */
    virtual bool load(HumidityData &data) = 0;
};

/**
 * @brief SDカードを使用した湿度データレコーダーの実装
 *
 * SDカード上のファイルにバイナリ形式でデータを保存・読み込みします。
 */
class SDHumidityRecorder final : public IHumidityRecorder
{
public:
    /**
     * @brief コンストラクタ
     *
     * @param sd SDファイルシステムオブジェクトへの参照
     */
    explicit SDHumidityRecorder(fs::SDFS &sd) : sd(sd)
    {
    }

    /**
     * @brief SDカードへデータを保存する
     *
     * @param data 保存する湿度データ
     * @return true 保存成功
     * @return false 保存失敗
     */
    bool save(const HumidityData &data) override;

    /**
     * @brief SDカードからデータを読み込む
     *
     * @param[out] data 読み込んだデータを格納する湿度データ構造体
     * @return true 読み込み成功
     * @return false 読み込み失敗
     */
    bool load(HumidityData &data) override;

private:
    fs::SDFS &sd; ///< SDファイルシステムへの参照

    /**
     * @brief SDカードが利用可能かチェックする
     *
     * @return true 利用可能
     * @return false 利用不可
     */
    bool isSDAvailable();
};