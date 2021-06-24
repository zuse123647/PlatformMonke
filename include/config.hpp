#pragma once

struct config_t {
    bool enabled = false;
};

extern config_t config;

void SaveConfig();
bool LoadConfig();