#include "DsDemo.h"


int wWinMain(void* hInstance, void* hPrevInstance, wchar_t* lpCmdLine, int nCmdShow)
{
    DsDemo demo;

    if (demo.init()) {
        demo.run();
    }

    demo.free();
    return 0;
}
