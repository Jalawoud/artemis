#pragma once

#include "SDL_compat.h"

// Remap both the modifier key itself and the modifier state on other keys.
// Keep left/right modifiers distinct and leave all other modifiers unchanged.
inline void swapCommandAndControl(SDL_Keysym& keysym)
{
    switch (keysym.scancode) {
    case SDL_SCANCODE_LCTRL:
        keysym.scancode = SDL_SCANCODE_LGUI;
        break;
    case SDL_SCANCODE_RCTRL:
        keysym.scancode = SDL_SCANCODE_RGUI;
        break;
    case SDL_SCANCODE_LGUI:
        keysym.scancode = SDL_SCANCODE_LCTRL;
        break;
    case SDL_SCANCODE_RGUI:
        keysym.scancode = SDL_SCANCODE_RCTRL;
        break;
    default:
        break;
    }

    Uint16 modifiers = keysym.mod & ~(KMOD_CTRL | KMOD_GUI);
    if (keysym.mod & KMOD_LCTRL) {
        modifiers |= KMOD_LGUI;
    }
    if (keysym.mod & KMOD_RCTRL) {
        modifiers |= KMOD_RGUI;
    }
    if (keysym.mod & KMOD_LGUI) {
        modifiers |= KMOD_LCTRL;
    }
    if (keysym.mod & KMOD_RGUI) {
        modifiers |= KMOD_RCTRL;
    }
    keysym.mod = modifiers;
}
