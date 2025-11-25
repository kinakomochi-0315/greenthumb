#include "humidity_recorder.h"

bool SDHumidityRecorder::isSDAvailable()
{
    if (!sd.begin())
    {
        return false;
    }
    return true;
}

bool SDHumidityRecorder::save(const HumidityData &data)
{
    if (!isSDAvailable())
        return false;

    // ファイルを開く
    fs::File file = sd.open("/humidity_log.txt", "w");
    if (!file)
        return false;

    // recordHead, recordSizeを書き込み
    file.println(data.head);
    file.println(HumidityData::RECORD_SIZE);

    // データを書き込み
    for (size_t i = 0; i < HumidityData::RECORD_SIZE; i++)
    {
        file.println(data.record[i]);
    }

    file.println();
    file.close();

    return true;
}

bool SDHumidityRecorder::load(HumidityData &data)
{
    if (!isSDAvailable())
        return false;

    // ファイルを開く
    fs::File file = sd.open("/humidity_log.txt", "r");
    if (!file)
        return false;

    // recordHead, recordSizeを読み込み
    data.head = file.parseInt();
    size_t savedRecordSize = file.parseInt();
    if (savedRecordSize < HumidityData::RECORD_SIZE)
    {
        // データサイズが不足している場合は失敗
        file.close();
        return false;
    }

    // データを読み込み
    for (size_t i = 0; i < HumidityData::RECORD_SIZE; i++)
    {
        data.record[i] = file.parseFloat();
    }

    file.close();
    return true;
}