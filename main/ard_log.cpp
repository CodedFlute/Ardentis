/**
 * @file ard_log.cpp
 * @author Roland Neill
 * @brief Logging library implementation for the Ardentis flight computer.
 * @version 0.1-alpha
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "ard_log.h"

#include <esp_log.h>

ardLog::ardLog(const char* tag) {
    TAG = tag;
}

void ardLog::logLevel(ard_logLevel_t level) {
    esp_log_level_set(TAG, (esp_log_level_t)level);
}

void ardLog::error(const char* message) {
    ESP_LOGE(TAG, "%s", message);
}

void ardLog::warn(const char* message) {
    ESP_LOGW(TAG, "%s", message);
}

void ardLog::info(const char* message) {
    ESP_LOGI(TAG, "%s", message);
}

void ardLog::debug(const char* message) {
    ESP_LOGD(TAG, "%s", message);
}

void ardLog::verb(const char* message) {
    ESP_LOGV(TAG, "%s", message);
}
