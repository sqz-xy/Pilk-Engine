/* Resources
* - https://learnopengl.com/Getting-started/Hello-Window
*/

#include "Engine/Managers/SceneManager.h"

/**
    * \brief The main program
    * \return exit code
    */
int main()
{
    SceneManager sm(800, 600, "Hello World!!!!");
    int exitCode = sm.Run();
    return exitCode;
}
