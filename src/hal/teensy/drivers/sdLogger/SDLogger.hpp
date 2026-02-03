#pragma once
#include <config/board_configs.h>
#include <Arduino.h>
#include <SdFat.h>
#include <FS.h>

class SDLogger
{
private:
    SdFat sd;
    FsFile file;
public:
    SDLogger();
    ~SDLogger();

    bool init();

    void writeLog(String msg);
};


