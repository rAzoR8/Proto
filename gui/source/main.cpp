#include "proto/ProtoWindow.h"

int main(int, char**)
{
    proto::ProtoWindow wnd;

    if (wnd.init(1920, 1080))
    {
        return wnd.exec();
    }

    return -1;
}
