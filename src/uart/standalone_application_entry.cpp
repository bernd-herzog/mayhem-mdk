/*
 * Copyright (C) 2024 Bernd Herzog
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "standalone_application.hpp"
#include <memory>

const standalone_application_api_t *_api;

extern "C"
{
    __attribute__((section(".standalone_application_information"), used)) standalone_application_information_t _standalone_application_information = {
        /*.header_version = */ CURRENT_STANDALONE_APPLICATION_API_VERSION,

        /*.app_name = */ "UART",
        /*.bitmap_data = */ {
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0xFC,
            0x3F,
            0xFE,
            0x7F,
            0x02,
            0x40,
            0xBA,
            0x45,
            0x02,
            0x40,
            0xFE,
            0x7F,
            0xFE,
            0x7F,
            0x92,
            0x7C,
            0x92,
            0x7C,
            0xFC,
            0x3F,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
        },
        /*.icon_color = 16 bit: 5R 6G 5B*/ 0x0000FFE0,
        /*.menu_location = */ app_location_t::UTILITIES,

        /*.initialize_app = */ initialize,
        /*.on_event = */ on_event,
        /*.shutdown = */ shutdown,
        /*.PaintViewMirror = */ PaintViewMirror,
        /*.OnTouchEvent = */ OnTouchEvent,
        /*.OnFocus = */ OnFocus,
        /*.OnKeyEvent = */ OnKeyEvent,
        /*.OnEncoder = */ OnEncoder,
        /*.OnKeyboad = */ OnKeyboad,
    };
}

/* Implementing abort() eliminates requirement for _getpid(), _kill(), _exit(). */
extern "C" void abort()
{
    while (true)
        ;
}

// replace memory allocations to use heap from chibios
extern "C" void *malloc(size_t size)
{
    return _api->malloc(size);
}
extern "C" void *calloc(size_t num, size_t size)
{
    return _api->calloc(num, size);
}
extern "C" void *realloc(void *p, size_t size)
{
    return _api->realloc(p, size);
}
extern "C" void free(void *p)
{
    _api->free(p);
}

// redirect std lib memory allocations (sprintf, etc.)
extern "C" void *__wrap__malloc_r(size_t size)
{
    return _api->malloc(size);
}
extern "C" void __wrap__free_r(void *p)
{
    _api->free(p);
}
