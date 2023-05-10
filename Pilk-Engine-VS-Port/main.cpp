#pragma once
#include "SceneManager.h"

int main()
{
    SceneManager sm(800, 600, "Hello World!!!!");
    int exitCode = sm.Run();
    return exitCode;
}
