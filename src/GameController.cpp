#include "GameController.hpp"
#include "GameConfig.hpp"
#include "Util.hpp"

GameController controller;

bool GameController::init()
{
    initKeycodeMap();
    if (!updateBinding()) return false;
    return true;
}

/*
Updates the key bindings based on config
Returns true on success, false if one or more keys failed to bind
*/
bool GameController::updateBinding()
{
    //Configurable binding of multiple keyboard keys
    const std::string config_list[] = {"bind_enter", "bind_escape", "bind_up",
                                       "bind_down", "bind_left", "bind_right"};
    const Key key_list[] = {K_ENTER, K_ESCAPE, K_UP, K_DOWN, K_LEFT, K_RIGHT};
    bool flag = true;
    std::set<std::string> used;
    for (unsigned i = 0; i < 6; i++) {
        key_map[key_list[i]] = std::vector<sf::Keyboard::Key>();
        std::vector<std::string> key_name = util::split(config.getStr(config_list[i], ""));
        for (unsigned j = 0; j < key_name.size(); j++) {
            const std::string& name = key_name[j];
            if (used.find(name) == used.end() && keycode_map.find(name) != keycode_map.end()) {
                key_map[key_list[i]].push_back(keycode_map[name]);
            }
        }
        if (key_map[key_list[i]].empty()) {
            flag = false;
            key_map[key_list[i]].push_back(sf::Keyboard::Unknown);
        }
    }

    //Non-configurable binding of mouse buttons
    button_map[B_LEFT] = sf::Mouse::Left;
    button_map[B_MIDDLE] = sf::Mouse::Middle;
    button_map[B_RIGHT] = sf::Mouse::Right;

    return flag;
}

/*
Check if a mouse button is being pressed
*/
bool GameController::clicking(Button button)
{
    return sf::Mouse::isButtonPressed(button_map[button]);
}

/*
Check if a specific key code is the binded key
*/	
bool GameController::pressing(Key key, sf::Keyboard::Key code)
{
    for (unsigned i = 0; i < key_map[key].size(); i++)
        if (key_map[key][i] == code)
            return true;
    return false;
}

/*
Check if a binded key is being pressed
*/
bool GameController::pressing(Key key)
{
    for (unsigned i = 0; i < key_map[key].size(); i++)
        if (sf::Keyboard::isKeyPressed(key_map[key][i]))
            return true;
    return false;
}

void GameController::initKeycodeMap()
{
    keycode_map["Key_A"] = sf::Keyboard::A;
    keycode_map["Key_B"] = sf::Keyboard::B;
    keycode_map["Key_C"] = sf::Keyboard::C;
    keycode_map["Key_D"] = sf::Keyboard::D;
    keycode_map["Key_E"] = sf::Keyboard::E;
    keycode_map["Key_F"] = sf::Keyboard::F;
    keycode_map["Key_G"] = sf::Keyboard::G;
    keycode_map["Key_H"] = sf::Keyboard::H;
    keycode_map["Key_I"] = sf::Keyboard::I;
    keycode_map["Key_J"] = sf::Keyboard::J;
    keycode_map["Key_K"] = sf::Keyboard::K;
    keycode_map["Key_L"] = sf::Keyboard::L;
    keycode_map["Key_M"] = sf::Keyboard::M;
    keycode_map["Key_N"] = sf::Keyboard::N;
    keycode_map["Key_O"] = sf::Keyboard::O;
    keycode_map["Key_P"] = sf::Keyboard::P;
    keycode_map["Key_Q"] = sf::Keyboard::Q;
    keycode_map["Key_R"] = sf::Keyboard::R;
    keycode_map["Key_S"] = sf::Keyboard::S;
    keycode_map["Key_T"] = sf::Keyboard::T;
    keycode_map["Key_U"] = sf::Keyboard::U;
    keycode_map["Key_V"] = sf::Keyboard::V;
    keycode_map["Key_W"] = sf::Keyboard::W;
    keycode_map["Key_X"] = sf::Keyboard::X;
    keycode_map["Key_Y"] = sf::Keyboard::Y;
    keycode_map["Key_Z"] = sf::Keyboard::Z;
    keycode_map["Key_Num0"] = sf::Keyboard::Num0;
    keycode_map["Key_Num1"] = sf::Keyboard::Num1;
    keycode_map["Key_Num2"] = sf::Keyboard::Num2;
    keycode_map["Key_Num3"] = sf::Keyboard::Num3;
    keycode_map["Key_Num4"] = sf::Keyboard::Num4;
    keycode_map["Key_Num5"] = sf::Keyboard::Num5;
    keycode_map["Key_Num6"] = sf::Keyboard::Num6;
    keycode_map["Key_Num7"] = sf::Keyboard::Num7;
    keycode_map["Key_Num8"] = sf::Keyboard::Num8;
    keycode_map["Key_Num9"] = sf::Keyboard::Num9;
    keycode_map["Key_Escape"] = sf::Keyboard::Escape;
    keycode_map["Key_LControl"] = sf::Keyboard::LControl;
    keycode_map["Key_LShift"] = sf::Keyboard::LShift;
    keycode_map["Key_LAlt"] = sf::Keyboard::LAlt;
    keycode_map["Key_LSystem"] = sf::Keyboard::LSystem;
    keycode_map["Key_RControl"] = sf::Keyboard::RControl;
    keycode_map["Key_RShift"] = sf::Keyboard::RShift;
    keycode_map["Key_RAlt"] = sf::Keyboard::RAlt;
    keycode_map["Key_RSystem"] = sf::Keyboard::RSystem;
    keycode_map["Key_Menu"] = sf::Keyboard::Menu;
    keycode_map["Key_LBracket"] = sf::Keyboard::LBracket;
    keycode_map["Key_RBracket"] = sf::Keyboard::RBracket;
    keycode_map["Key_SemiColon"] = sf::Keyboard::SemiColon;
    keycode_map["Key_Comma"] = sf::Keyboard::Comma;
    keycode_map["Key_Period"] = sf::Keyboard::Period;
    keycode_map["Key_Quote"] = sf::Keyboard::Quote;
    keycode_map["Key_Slash"] = sf::Keyboard::Slash;
    keycode_map["Key_BackSlash"] = sf::Keyboard::BackSlash;
    keycode_map["Key_Tilde"] = sf::Keyboard::Tilde;
    keycode_map["Key_Equal"] = sf::Keyboard::Equal;
    keycode_map["Key_Dash"] = sf::Keyboard::Dash;
    keycode_map["Key_Space"] = sf::Keyboard::Space;
    keycode_map["Key_Return"] = sf::Keyboard::Return;
    keycode_map["Key_BackSpace"] = sf::Keyboard::BackSpace;
    keycode_map["Key_Tab"] = sf::Keyboard::Tab;
    keycode_map["Key_PageUp"] = sf::Keyboard::PageUp;
    keycode_map["Key_PageDown"] = sf::Keyboard::PageDown;
    keycode_map["Key_End"] = sf::Keyboard::End;
    keycode_map["Key_Home"] = sf::Keyboard::Home;
    keycode_map["Key_Insert"] = sf::Keyboard::Insert;
    keycode_map["Key_Delete"] = sf::Keyboard::Delete;
    keycode_map["Key_Add"] = sf::Keyboard::Add;
    keycode_map["Key_Subtract"] = sf::Keyboard::Subtract;
    keycode_map["Key_Multiply"] = sf::Keyboard::Multiply;
    keycode_map["Key_Divide"] = sf::Keyboard::Divide;
    keycode_map["Key_Left"] = sf::Keyboard::Left;
    keycode_map["Key_Right"] = sf::Keyboard::Right;
    keycode_map["Key_Up"] = sf::Keyboard::Up;
    keycode_map["Key_Down"] = sf::Keyboard::Down;
    keycode_map["Key_Numpad0"] = sf::Keyboard::Numpad0;
    keycode_map["Key_Numpad1"] = sf::Keyboard::Numpad1;
    keycode_map["Key_Numpad2"] = sf::Keyboard::Numpad2;
    keycode_map["Key_Numpad3"] = sf::Keyboard::Numpad3;
    keycode_map["Key_Numpad4"] = sf::Keyboard::Numpad4;
    keycode_map["Key_Numpad5"] = sf::Keyboard::Numpad5;
    keycode_map["Key_Numpad6"] = sf::Keyboard::Numpad6;
    keycode_map["Key_Numpad7"] = sf::Keyboard::Numpad7;
    keycode_map["Key_Numpad8"] = sf::Keyboard::Numpad8;
    keycode_map["Key_Numpad9"] = sf::Keyboard::Numpad9;
    keycode_map["Key_F1"] = sf::Keyboard::F1;
    keycode_map["Key_F2"] = sf::Keyboard::F2;
    keycode_map["Key_F3"] = sf::Keyboard::F3;
    keycode_map["Key_F4"] = sf::Keyboard::F4;
    keycode_map["Key_F5"] = sf::Keyboard::F5;
    keycode_map["Key_F6"] = sf::Keyboard::F6;
    keycode_map["Key_F7"] = sf::Keyboard::F7;
    keycode_map["Key_F8"] = sf::Keyboard::F8;
    keycode_map["Key_F9"] = sf::Keyboard::F9;
    keycode_map["Key_F10"] = sf::Keyboard::F10;
    keycode_map["Key_F11"] = sf::Keyboard::F11;
    keycode_map["Key_F12"] = sf::Keyboard::F12;
    keycode_map["Key_F13"] = sf::Keyboard::F13;
    keycode_map["Key_F14"] = sf::Keyboard::F14;
    keycode_map["Key_F15"] = sf::Keyboard::F15;
    keycode_map["Key_Pause"] = sf::Keyboard::Pause;
}
