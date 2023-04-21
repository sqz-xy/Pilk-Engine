/* Resources
* - https://learnopengl.com/Getting-started/Hello-Window
*/

#include "SceneManager.h"
#include <crtdbg.h>

/**
    * \brief The main program
    * \return exit code
    */
int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    SceneManager sm(800, 600, "Hello World!!!!");
    int exitCode = sm.Run();
    return exitCode;
}
