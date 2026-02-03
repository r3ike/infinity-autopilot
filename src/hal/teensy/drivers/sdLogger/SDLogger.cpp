#include <drivers/sdLogger/SDLogger.hpp>
#include "SDLogger.hpp"

SDLogger::SDLogger(){}
SDLogger::~SDLogger(){}

bool SDLogger::init()
{
    return sd.begin(BUILTIN_SDCARD);
    file = sd.open('logs.log', FILE_WRITE);
}

void SDLogger::writeLog(String msg)
{
    

    if (file)
    {
        file.println(msg);
        file.close();
    }
    
}


