/*  test_json/main.cpp
 *
 *  Copyright (c) 2018 Kirk Lange <github.com/kirklange>
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 */

/** @file       test_json/main.cpp
 *  @brief      Test driving nlohmann::json
 */

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <vector>

// Counteract SDL hijacking
#define SDL_main main



void iterate(nlohmann::json obj, int depth=0)
{
    for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++)
    {
        for (int i = depth; i > 0; i--)
        {
            std::cout << "  ";
        }

        std::cout << it.key() << ":" << it.value() << std::endl;

        if (it.value().is_structured() && !it.value().is_array())
        {
            iterate(it.value(), depth+1);
        }
    }
}



int main(int argc, char *argv[])
{
    std::ifstream infile("data/main.json");
    nlohmann::json settings;
    infile >> settings;
    iterate(settings);
    infile.close();

    return 0;
}
