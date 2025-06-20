#ifndef SCRIPT_POWER_H
#define SCRIPT_POWER_H

#include <stdio.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <cstring>

namespace Constants{
    static const int CountNeedLines = 6;
    static const char* help_menu_file = "../etc/help-menu.txt";
    static const char* monitors_config_template = "../etc/monitors-conf-template.txt";
    static const char* cacheFileName = "../cache/monitors-tmp.txt";
};

class Arguments{
public:
    ~Arguments(){
        delete[] this->path;
        delete[] this->size_frame;
        delete[] this->scaling;
        delete[] this->power_daemon;
    }
    Arguments& parse(int& argc, char** argv);

    void printArgs();

    char* scaling = new char[1]{'1'}; // Default scaling == 1
    char* path = new char[]{"/home/sarumo/.config/hypr/monitors.conf"}; // Path to file with monitor configuration
    char* size_frame = new char[9]{"prefered"}; // Size for yours monitor. Default is preferd size for your monitor(max size and freq)
    char* power_daemon = new char[9]{"balanced"}; // Parametr for additional packedge, default = balanced
};

namespace pss{
    inline void HelpMenu();

    class PowerSaver{
    public:
        void operator()(Arguments& args);
    };

    void EditConfigFile(Arguments& args);
}; //namespace pss

#endif //SCRIPT_POWER_H