#include <stdio.h>
#include <cstring>
#include <power-saver-script.h>

int main(int argc, char* argv[]){
{
    Arguments args;
    args.parse(argc, argv);
    args.printArgs();
    pss::PowerSaver()(args);
}
    return 0;
}