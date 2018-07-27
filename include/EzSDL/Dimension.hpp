/*  EzSDL/Dimension.hpp
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

#ifndef EZSDL_DIMENSION_HPP
#define EZSDL_DIMENSION_HPP

/** @file       EzSDL/Dimension.hpp
 *  @brief      Lorem ipsum
 *  @details    Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 */

#include <map>
#include <memory>



namespace EzSDL
{

/** @brief      Lorem ipsum
 *  @details    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do
 *              eiusmod tempor incididunt ut labore et dolore magna aliqua.
 */
template <typename T>
class Dimension
{
public:
    enum Key
    {
        X, Y, Z, W, H, D,
        DX, DY, DZ, DW, DH, DD,
        D2X, D2Y, D2Z, D2W, D2H, D2D
    };

    using DimensionPtr = std::unique_ptr<Dimension<T>>;
    static DimensionPtr create();

    T& operator[](Key const &key);
    T& at(Key const &key);

    virtual ~Dimension() = default;

protected:

private:
    Dimension() = default;
    Dimension(Dimension const &other) = delete;
    Dimension& operator=(Dimension const &other) = delete;

    using DimensionMap = std::map<Key, T>;
    DimensionMap dimensionMap;
};

using DDimensionPtr = Dimension<double>::DimensionPtr;
using FDimensionPtr = Dimension<float>::DimensionPtr;
using LDimensionPtr = Dimension<long>::DimensionPtr;
using IDimensionPtr = Dimension<int>::DimensionPtr;

}; /* namespace EzSDL */



#endif /* EZSDL_DIMENSION_HPP */