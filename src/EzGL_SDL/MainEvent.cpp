/*  EzGL_SDL/MainEvent.cpp
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

#include "EzGL_SDL/MainEvent.hpp"

#include "EzGL/Object.hpp"
#include "EzGL_SDL/MainRender.hpp"

#include <iostream>
#include <SDL2/SDL.h>

namespace EzGL
{



void MainEvent::init(Object &self, Object &main)
{
    // TODO: If SDL wasn't initialized already
    if (true)
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_version linked;
        SDL_GetVersion(&linked);

        std::cout << "Using SDL version " <<
            static_cast<int>(linked.major) << "." <<
            static_cast<int>(linked.minor) << "." <<
            static_cast<int>(linked.patch) << "." << std::endl <<
            "Initialized all SDL systems." << std::endl;
    }

    self.data["pause"] = false;
    self.data["quit"] = false;

#define KEY(keyname) \
    self.data["input"][#keyname] = false

    KEY(F1); KEY(F2); KEY(F3); KEY(F4); KEY(F5); KEY(F6); KEY(F7); KEY(F8);
    KEY(F9); KEY(F10); KEY(F11); KEY(F12); KEY(F13); KEY(F14); KEY(F15);
    KEY(F16); KEY(F17); KEY(F18); KEY(F19); KEY(F20); KEY(F21); KEY(F22);
    KEY(F23); KEY(F24);

    KEY(0); KEY(1); KEY(2); KEY(3); KEY(4); KEY(5); KEY(6); KEY(7); KEY(8);
    KEY(9); KEY(NUMLOCKCLEAR);

    KEY(EQUALS); KEY(MINUS); KEY(QUOTE);

    KEY(a); KEY(b); KEY(c); KEY(d); KEY(e); KEY(f); KEY(g); KEY(h); KEY(i);
    KEY(j); KEY(k); KEY(l); KEY(m); KEY(n); KEY(o); KEY(p); KEY(q); KEY(r);
    KEY(s); KEY(t); KEY(u); KEY(v); KEY(w); KEY(x); KEY(y); KEY(z);

    KEY(BACKSLASH); KEY(BACKQUOTE); KEY(LEFTBRACKET); KEY(PERIOD);
    KEY(RIGHTBRACKET); KEY(SEMICOLON); KEY(SLASH);

    KEY(DOWN); KEY(UP); KEY(LEFT); KEY(RIGHT);

    KEY(DELETE); KEY(END); KEY(HOME); KEY(INSERT); KEY(PAGEDOWN); KEY(PAGEUP);
    KEY(PAUSE); KEY(PRINTSCREEN); KEY(SCROLLLOCK); KEY(SYSREQ);

    KEY(CAPSLOCK); KEY(ESCAPE); KEY(LALT); KEY(LCTRL); KEY(LGUI); KEY(LSHIFT);
    KEY(MENU); KEY(RALT); KEY(RCTRL); KEY(RGUI); KEY(RSHIFT);

    KEY(BACKSPACE); KEY(RETURN); KEY(SPACE); KEY(TAB);

#undef KEY

    // In case there is no Control component
    self.data["input"]["pause"] = false;
    self.data["input"]["quit"] = false;
    self.data["input"]["show_cursor"] = false;
    self.data["input"]["bordered"] = false;
    self.data["input"]["fullscreen"] = false;

    this->cooldown = 0;
}



void MainEvent::update(Object &self, Object &main)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            self.data["quit"] = true;
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
#define KEY(keyname) \
            case SDLK_##keyname: \
                self.data["input"][#keyname] = \
                    (e.key.type == SDL_KEYDOWN ? true : false); \
                break;

            KEY(F1); KEY(F2); KEY(F3); KEY(F4); KEY(F5); KEY(F6);
            KEY(F7); KEY(F8); KEY(F9); KEY(F10); KEY(F11); KEY(F12);
            KEY(F13); KEY(F14); KEY(F15); KEY(F16); KEY(F17); KEY(F18);
            KEY(F19); KEY(F20); KEY(F21); KEY(F22); KEY(F23); KEY(F24);

            KEY(0); KEY(1); KEY(2); KEY(3); KEY(4); KEY(5); KEY(6);
            KEY(7); KEY(8); KEY(9); KEY(NUMLOCKCLEAR);

            KEY(EQUALS); KEY(MINUS); KEY(QUOTE);

            KEY(a); KEY(b); KEY(c); KEY(d); KEY(e); KEY(f); KEY(g);
            KEY(h); KEY(i); KEY(j); KEY(k); KEY(l); KEY(m); KEY(n);
            KEY(o); KEY(p); KEY(q); KEY(r); KEY(s); KEY(t); KEY(u);
            KEY(v); KEY(w); KEY(x); KEY(y); KEY(z);

            KEY(BACKSLASH); KEY(BACKQUOTE); KEY(LEFTBRACKET); KEY(PERIOD);
            KEY(RIGHTBRACKET); KEY(SEMICOLON); KEY(SLASH);

            KEY(DOWN); KEY(UP); KEY(LEFT); KEY(RIGHT);

            KEY(DELETE); KEY(END); KEY(HOME); KEY(INSERT); KEY(PAGEDOWN);
            KEY(PAGEUP); KEY(PAUSE); KEY(PRINTSCREEN); KEY(SCROLLLOCK);
            KEY(SYSREQ);

            KEY(CAPSLOCK); KEY(ESCAPE); KEY(LALT); KEY(LCTRL); KEY(LGUI);
            KEY(LSHIFT); KEY(MENU); KEY(RALT); KEY(RCTRL); KEY(RGUI);
            KEY(RSHIFT);

            KEY(BACKSPACE); KEY(RETURN); KEY(SPACE); KEY(TAB);

#undef KEY_SDLK
            }
            break;
        // End SDL_KEYDOWN || SDL_KEYUP

        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                self.data["pause"] = false;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                self.data["pause"] = true;
                break;
            }
        }
    } // End event polling

    // Handle window setting events
    if (self.data["input"]["pause"])
        self.data["pause"] = !self.data["pause"].get<bool>();

#ifndef __EMSCRIPTEN__
    if (self.data["input"]["quit"])
    {
        self.data["quit"] = true;
    }

    if (cooldown == 0)
    {
        if (self.data["input"]["show_cursor"])
        {
            // TODO
        }

        if (self.data["input"]["bordered"])
        {
            bool isbl = SDL_WINDOW_BORDERLESS &
                SDL_GetWindowFlags(MainRender::Window);

            SDL_SetWindowBordered(MainRender::Window,
                    static_cast<SDL_bool>(isbl));

            if (!isbl)
                SDL_SetWindowPosition(MainRender::Window,
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            else
                SDL_SetWindowPosition(MainRender::Window, 4, 32);

            cooldown = 250;
        }

        if (self.data["input"]["fullscreen"])
        {
            bool isfs = SDL_WINDOW_FULLSCREEN_DESKTOP &
                SDL_GetWindowFlags(MainRender::Window);

            SDL_SetWindowFullscreen(MainRender::Window,static_cast<SDL_bool>(
                    isfs ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP));

            cooldown = 250;
        }
    }
    else if (cooldown < 0) cooldown = 0;
    else if (cooldown > 0) cooldown -= (main.data["dt"].get<double>()*1000.0);
#endif


}



}; /* namespace EzGL */
