#include "proto/ProtoWindow.h"

int main(int, char**)
{
    proto::ProtoWindow wnd(1920, 1080);

    return wnd.exec();
}
