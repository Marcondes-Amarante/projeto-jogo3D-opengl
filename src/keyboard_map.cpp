#include "include/keyboard_map.hpp"

KeyboardMap *KeyboardMap::instance = nullptr;

KeyboardMap* KeyboardMap::get_instance() {
    if (!KeyboardMap::instance)
        KeyboardMap::instance = new KeyboardMap();
    return KeyboardMap::instance;
} 

void KeyboardMap::add_key_func(string keys, function<void(int, int, int)> func) {
    auto& kf = get_instance()->keyboard_func_with_input;
    for (unsigned char key: keys) {
        kf[key].push_back(func);
    }
}

void KeyboardMap::add_key_func(string keys, function<void()> func) {
    auto& kf = get_instance()->keyboard_func_action_only;
    for (unsigned char key: keys) {
        kf[key].push_back(func);
    }
}

void KeyboardMap::keyPressAction(unsigned char key, int mouse_x, int mouse_y) {
    auto& kf_with_input = get_instance()->keyboard_func_with_input;
    auto& kf_action_only = get_instance()->keyboard_func_action_only;

    if (kf_with_input.count(key)) {
        for (auto& func: kf_with_input[key])
            func(key, mouse_x, mouse_y);
    }

    if (kf_action_only.count(key)) {
        for (auto& func: kf_action_only[key])
            func();
    };
}