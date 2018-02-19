/* main.cpp
Main function for ezSDL. None of this code is part of the engine itself.
Rather, treat this as an example or template for basing your future projects
off of.


ezSDL - The Easy Simple DirectMedia Layer Wrapper
Copyright (c) 2018 Kirk Lange

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "ezwindow.h"

#include <stdio.h>



int main(int argc, char *argv[])
{
    printf("Hello world, I am ezsdl2!\n");

    ezwindow *ezw = ezwindow_new();
    while (ezwindow_isRunning(ezw))
    {
        ezwindow_pollEvent(ezw);
        ezwindow_clear(ezw);
        ezwindow_update(ezw);
    }
    ezwindow_del(&ezw);

    return 0;
}
