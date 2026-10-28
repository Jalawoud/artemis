#include "streaming/input/keyboardmapping.h"

#include <cstdlib>
#include <iostream>

static void check(bool condition, const char* description)
{
    if (!condition) {
        std::cerr << description << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

int main()
{
    const struct {
        SDL_Scancode input;
        SDL_Scancode output;
        Uint16 inputModifiers;
        Uint16 outputModifiers;
    } cases[] = {
        {SDL_SCANCODE_LGUI, SDL_SCANCODE_LCTRL, KMOD_LGUI, KMOD_LCTRL},
        {SDL_SCANCODE_RGUI, SDL_SCANCODE_RCTRL, KMOD_RGUI, KMOD_RCTRL},
        {SDL_SCANCODE_LCTRL, SDL_SCANCODE_LGUI, KMOD_LCTRL, KMOD_LGUI},
        {SDL_SCANCODE_RCTRL, SDL_SCANCODE_RGUI, KMOD_RCTRL, KMOD_RGUI},
        {SDL_SCANCODE_C, SDL_SCANCODE_C, KMOD_LGUI, KMOD_LCTRL},
        {SDL_SCANCODE_V, SDL_SCANCODE_V, KMOD_RGUI, KMOD_RCTRL},
        {SDL_SCANCODE_V, SDL_SCANCODE_V, KMOD_LCTRL, KMOD_LGUI},
        {SDL_SCANCODE_S, SDL_SCANCODE_S, KMOD_LCTRL | KMOD_LSHIFT, KMOD_LGUI | KMOD_LSHIFT},
        {SDL_SCANCODE_TAB, SDL_SCANCODE_TAB, KMOD_LALT, KMOD_LALT},
        {SDL_SCANCODE_F4, SDL_SCANCODE_F4, KMOD_RALT, KMOD_RALT},
        {SDL_SCANCODE_C, SDL_SCANCODE_C, KMOD_LCTRL | KMOD_RGUI, KMOD_LGUI | KMOD_RCTRL},
        // Key-up events must use the same host scancode, even with no modifiers held.
        {SDL_SCANCODE_LGUI, SDL_SCANCODE_LCTRL, KMOD_NONE, KMOD_NONE},
        {SDL_SCANCODE_RGUI, SDL_SCANCODE_RCTRL, KMOD_NONE, KMOD_NONE},
        {SDL_SCANCODE_LCTRL, SDL_SCANCODE_LGUI, KMOD_NONE, KMOD_NONE},
        {SDL_SCANCODE_RCTRL, SDL_SCANCODE_RGUI, KMOD_NONE, KMOD_NONE},
    };

    for (const auto& test : cases) {
        SDL_Keysym keysym = {};
        keysym.scancode = test.input;
        keysym.mod = test.inputModifiers;
        swapCommandAndControl(keysym);
        check(keysym.scancode == test.output, "Incorrect host scancode");
        check(keysym.mod == test.outputModifiers, "Incorrect host modifiers");
    }

    // Exercise every SDL modifier-bit combination, including both sides held
    // together and all unrelated modifiers (Alt, Shift, Caps Lock, etc.).
    for (unsigned int modifiers = 0; modifiers <= 0xFFFF; modifiers++) {
        SDL_Keysym keysym = {};
        keysym.scancode = SDL_SCANCODE_C;
        keysym.sym = SDLK_c;
        keysym.mod = static_cast<Uint16>(modifiers);
        swapCommandAndControl(keysym);
        check((keysym.mod & KMOD_CTRL) == ((modifiers & KMOD_GUI) >> 4), "Command state not mapped to Control");
        check((keysym.mod & KMOD_GUI) == ((modifiers & KMOD_CTRL) << 4), "Control state not mapped to Command");
        check((keysym.mod & ~(KMOD_CTRL | KMOD_GUI)) == (modifiers & ~(KMOD_CTRL | KMOD_GUI)), "Unrelated modifier changed");
        check(keysym.scancode == SDL_SCANCODE_C && keysym.sym == SDLK_c, "Non-modifier key changed");
        swapCommandAndControl(keysym);
        check(keysym.mod == modifiers, "Swapping twice did not restore modifier state");
    }

    for (int scancode = SDL_SCANCODE_UNKNOWN; scancode < SDL_NUM_SCANCODES; scancode++) {
        SDL_Keysym keysym = {};
        keysym.scancode = static_cast<SDL_Scancode>(scancode);
        swapCommandAndControl(keysym);
        if (scancode != SDL_SCANCODE_LCTRL && scancode != SDL_SCANCODE_RCTRL &&
                scancode != SDL_SCANCODE_LGUI && scancode != SDL_SCANCODE_RGUI) {
            check(keysym.scancode == scancode, "Unrelated scancode changed");
        }
        swapCommandAndControl(keysym);
        check(keysym.scancode == scancode, "Swapping twice did not restore scancode");
    }

    std::cout << "Command/Control mapping tests passed" << std::endl;
}
