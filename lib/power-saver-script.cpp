#include "power-saver-script.h"

inline void pss::HelpMenu(){
    char* command = new char[]{""};
    sprintf(command, "cat %s", Constants::help_menu_file);
    int command_result = std::system(command);

    if (command_result){
        printf("Can't open help file!");
        exit(1);
    }
    delete[] command;
    return;
}

Arguments& Arguments::parse(int& argc, char** argv){
    if(argc <= 1){
        printf("Too few argumets! Use --help, if you don't know how to use this script!");
        exit(1);
    }

    auto generalOperation = [&](char* pointer){
        if (argc-1 > 0 && (*(argv+1))[0] != '-'){
            argv++;
            argc--;
            if (pointer != nullptr){
                delete[] pointer;
            }

            pointer = new char[strlen(*argv) + 1];
            strcpy(pointer, *argv);
            return true;
        }
        return false;
    };

    auto forStrigOperation = [&](std::string& str){
        if (argc-1 > 0 && (*(argv+1))[0] != '-'){
            argv++;
            argc--;

            str = *argv;
            
            return true;
        }
        return false;
    };

    while(argc > 1){
        argc--;
        argv++;
        if((*argv)[0] == '-'){
            if(!strcmp(*argv, "--help")){
                pss::HelpMenu();
                exit(0);
            }

            if (!strcmp(*argv, "--path") || !strcmp(*argv, "-p")){
                if (!forStrigOperation(this->path)){
                    printf("Use --path | -p <PATH>\n--help for more info");
                    fflush(stdout);
                }
            }

            if (!strcmp(*argv, "--scaling") || !strcmp(*argv, "-s")){
                if (!generalOperation(this->scaling)){
                    printf("Use --scaling | -s <SCALING>\n--help for more info\n");
                    fflush(stdout);
                }
            }

            if (!strcmp(*argv, "--size_frame") || !strcmp(*argv, "-sf")){
                if(!generalOperation(this->size_frame)){
                    printf("Use --size_frame | -sf <[Size]@[Ghz]>\n--help for more info\n");
                    fflush(stdout);
                }
            }

            if (!strcmp(*argv, "--power-daemon") || !strcmp(*argv, "-pd")){
                if(!generalOperation(this->power_daemon)){
                    printf("Use --power-daemon | -df <STATUS>\n--help for more info\n");
                    fflush(stdout);
                }
            } 
 
        } else {
            printf("Exepected argument: %s\n", *argv);
            fflush(stdout);
        }
        
    }
    return *this;
}

void Arguments::printArgs(){
    printf("Path: %s\nScaling: %s\nSize: %s\nPower-Daemon: %s\n", this->path.c_str(), this->scaling, this->size_frame, this->power_daemon);
    return;
}

void pss::EditConfigFile(Arguments& args){
    std::ofstream config_file(Constants::cacheFileName);
    std::fstream headerTemplate(Constants::monitors_config_template);

    if(config_file && headerTemplate){
        for(int lineFrmTempl = 0; lineFrmTempl < Constants::CountNeedLines; lineFrmTempl++){
            std::string line;
            getline(headerTemplate, line);
            config_file << line << '\n';
        }

        config_file << "monitor = ," << args.size_frame << ",auto," << args.scaling << '\n';

        config_file.close();
        headerTemplate.close();
        std::filesystem::rename(Constants::cacheFileName, args.path);
        
    } else {
        printf("Problem with files!");
        fflush(stdout);
    }

}

void pss::PowerSaver::operator()(Arguments& args){
    auto char_point_to_str = [&](char* c_string){
        std::string str;
        for(int i = 0; i < strlen(c_string); ++i){
            str += c_string[i];
        }
        return str;
    };

    std::string command_for_pd = "powerprofilesctl set " + char_point_to_str(args.power_daemon);
    int resultPowerDaemon = std::system(command_for_pd.c_str());

    if (resultPowerDaemon){
        printf("Problem with power profiles daemon");
        fflush(stdout);
        exit(1);
    }

    pss::EditConfigFile(args);
}