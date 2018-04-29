/*  pong_paddle.c
 *
 *  <!---------
 *  Copyright (c) 2018 Kirk Lange
 *  
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
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
 *  ---------->
 */

#include "EzSDL/ezsdl_window.h"
#include "EzUtil/ezutil_log.h"
#include "EzUtil/ezutil_observer.h"
#include "pong_ball.h"
#include "pong_paddle.h"

#include <math.h>
#include <stdio.h>




pong_paddle* pong_paddle_new(uint16_t keyUp, uint16_t keyDown,
        pong_ball *ball, SDL_Color *color, ezsdl_window *window)
{
    pong_paddle *self = (pong_paddle*) malloc(sizeof(pong_paddle));
    self->window = window;
    self->color = color;
    self->ball = ball;
    self->x = self->dy = 0.0;
    self->w = self->window->displayMode->w * 0.02;
    self->h = self->window->displayMode->h * 0.15;
    self->y = (self->window->displayMode->h/2) - (self->h/2);
    self->keyUp = keyUp;
    self->keyDown = keyDown;

    ezutil_observer_add(self->window->headEvent, &pong_paddle_event, self);
    ezutil_observer_add(self->window->headUpdate, &pong_paddle_update, self);
    ezutil_observer_add(self->window->headDraw, &pong_paddle_draw, self);

    return self;
}



uint8_t pong_paddle_del(pong_paddle **self)
{
    if (*self)
    {
        free(*self);
        *self = 0;

        return 1;
    }
    else
    {
        ezutil_log(VITAL, __func__, "Skipped deletion of pong_paddle "
                "instance. Cannot free a null pointer.");
        return 0;
    }

}



uint8_t pong_paddle_event(pong_paddle *self)
{
    if (self)
    {
        SDL_Event e = *(self->window->event);

        switch (e.type)
        {
            case SDL_KEYDOWN:
                self->dy = ((float) self->window->displayMode->h / 1080.0) *
                    PONG_PADDLE_VEL;

                if (e.key.keysym.scancode == self->keyUp)
                    self->dy *= -1;
                else if (e.key.keysym.scancode == self->keyDown)
                    self->dy *= 1;
                else
                    self->dy *= 0;

                break;
            case SDL_KEYUP:
                if (e.key.keysym.scancode == self->keyUp ||
                        e.key.keysym.scancode == self->keyDown)
                    self->dy = 0;
                break;
        }

        return 1;
    }
    else
    {
        /* TODO: error handling */
        return 0;
    }
}



uint8_t pong_paddle_update(pong_paddle *self)
{
    if (self)
    {
        self->y += self->dy * self->window->delta;

        if (self->ball->pauseTime)
            self->y = (self->window->displayMode->h/2) - (self->h/2);

        if (self->y < 0) self->y = 0;
        else if (self->y > self->window->displayMode->h - self->h)
            self->y = self->window->displayMode->h - self->h;

        if (self->ball->x < self->x+self->w &&
                self->ball->x+self->ball->r > self->x &&
                self->ball->y+self->ball->r > self->y &&
                self->ball->y < self->y+self->h)
        {
            float dxSign = self->ball->dx>0 ? -1 : 1;
            float angleIn = RTOD(atan(self->ball->dy/self->ball->dx)) * dxSign;

            float angleMult =
                ( ((self->y+(self->h/2)) - (self->ball->y+(self->ball->r/2))) /
                (self->h/2) ) / 2.0;

            float angleOut = angleIn - ((90.0-abs(angleIn))*angleMult);
            if (angleOut<-60.0) angleOut = -60.0;
            else if (angleOut>60.0) angleOut = 60.0;

            float windowRatio = (float) self->window->displayMode->w / 1920.0;

            self->ball->dx =
                dxSign * cos(DTOR(angleOut)) * windowRatio * PONG_BALL_VEL;
            self->ball->dy =
                -sin(DTOR(angleOut)) * windowRatio * PONG_BALL_VEL;

            if (self->ball->dx > 0)
                self->ball->x = self->x+self->w+1;
            else
                self->ball->x = self->x-self->ball->r-1;
        }
    }
    else
    {
        /* TODO: error handling */
        return 0;
    }
}



uint8_t pong_paddle_draw(pong_paddle *self)
{
    if (self)
    {
        ezsdl_window_drawRect(self->window, self->color,
                self->x, self->y, self->w, self->h);
        return 1;
    }
    else
    {
        /* TODO: error handling */
        return 0;
    }
}