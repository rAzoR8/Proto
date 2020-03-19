#include "proto/ProtoWindow.h"
#include "proto/WinHeapDbg.h"

int main(int, char**)
{
    proto::WinHeapDbg::init();

    proto::ProtoWindow wnd;

    if (wnd.init(1920, 1080))
    {
        return wnd.exec();
    }

    return -1;
}
