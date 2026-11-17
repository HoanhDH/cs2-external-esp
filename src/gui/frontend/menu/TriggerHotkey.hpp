#pragma once

#include <imgui.h>
#include <Windows.h>

// Translate assignable ImGui hotkeys to Windows virtual keys so that the
// visual feedback can respond while the game (not the menu) has focus.
// Pure input polling: this helper never synthesizes input.
namespace trigger_hotkey {
inline int ToVirtualKey(ImGuiKey key) {
    const int k = static_cast<int>(key);
    if (k >= static_cast<int>(ImGuiKey_A) && k <= static_cast<int>(ImGuiKey_Z))
        return 'A' + (k - static_cast<int>(ImGuiKey_A));
    if (k >= static_cast<int>(ImGuiKey_0) && k <= static_cast<int>(ImGuiKey_9))
        return '0' + (k - static_cast<int>(ImGuiKey_0));
    if (k >= static_cast<int>(ImGuiKey_F1) && k <= static_cast<int>(ImGuiKey_F12))
        return VK_F1 + (k - static_cast<int>(ImGuiKey_F1));
    if (k >= static_cast<int>(ImGuiKey_Keypad0) && k <= static_cast<int>(ImGuiKey_Keypad9))
        return VK_NUMPAD0 + (k - static_cast<int>(ImGuiKey_Keypad0));

    switch (key) {
    case ImGuiKey_MouseLeft:   return VK_LBUTTON;
    case ImGuiKey_MouseRight:  return VK_RBUTTON;
    case ImGuiKey_MouseMiddle: return VK_MBUTTON;
    case ImGuiKey_MouseX1:     return VK_XBUTTON1;
    case ImGuiKey_MouseX2:     return VK_XBUTTON2;
    case ImGuiKey_Tab:         return VK_TAB;
    case ImGuiKey_LeftArrow:   return VK_LEFT;
    case ImGuiKey_RightArrow:  return VK_RIGHT;
    case ImGuiKey_UpArrow:     return VK_UP;
    case ImGuiKey_DownArrow:   return VK_DOWN;
    case ImGuiKey_PageUp:      return VK_PRIOR;
    case ImGuiKey_PageDown:    return VK_NEXT;
    case ImGuiKey_Home:        return VK_HOME;
    case ImGuiKey_End:         return VK_END;
    case ImGuiKey_Insert:      return VK_INSERT;
    case ImGuiKey_Delete:      return VK_DELETE;
    case ImGuiKey_Backspace:   return VK_BACK;
    case ImGuiKey_Space:       return VK_SPACE;
    case ImGuiKey_Enter:       return VK_RETURN;
    case ImGuiKey_Escape:      return VK_ESCAPE;
    case ImGuiKey_LeftCtrl:    return VK_LCONTROL;
    case ImGuiKey_RightCtrl:   return VK_RCONTROL;
    case ImGuiKey_LeftShift:   return VK_LSHIFT;
    case ImGuiKey_RightShift:  return VK_RSHIFT;
    case ImGuiKey_LeftAlt:     return VK_LMENU;
    case ImGuiKey_RightAlt:    return VK_RMENU;
    case ImGuiKey_CapsLock:    return VK_CAPITAL;
    case ImGuiKey_NumLock:     return VK_NUMLOCK;
    case ImGuiKey_ScrollLock:  return VK_SCROLL;
    case ImGuiKey_KeypadDecimal:  return VK_DECIMAL;
    case ImGuiKey_KeypadDivide:   return VK_DIVIDE;
    case ImGuiKey_KeypadMultiply: return VK_MULTIPLY;
    case ImGuiKey_KeypadSubtract: return VK_SUBTRACT;
    case ImGuiKey_KeypadAdd:      return VK_ADD;
    case ImGuiKey_KeypadEnter:    return VK_RETURN;
    case ImGuiKey_GraveAccent:    return VK_OEM_3;
    case ImGuiKey_Minus:          return VK_OEM_MINUS;
    case ImGuiKey_Equal:          return VK_OEM_PLUS;
    case ImGuiKey_LeftBracket:    return VK_OEM_4;
    case ImGuiKey_RightBracket:   return VK_OEM_6;
    case ImGuiKey_Backslash:      return VK_OEM_5;
    case ImGuiKey_Semicolon:      return VK_OEM_1;
    case ImGuiKey_Apostrophe:     return VK_OEM_7;
    case ImGuiKey_Comma:          return VK_OEM_COMMA;
    case ImGuiKey_Period:         return VK_OEM_PERIOD;
    case ImGuiKey_Slash:          return VK_OEM_2;
    default: return 0; // Unsupported keys cannot activate the test.
    }
}

inline bool IsHeld(int imgui_key) {
    const int vk = ToVirtualKey(static_cast<ImGuiKey>(imgui_key));
    return vk != 0 && (GetAsyncKeyState(vk) & 0x8000) != 0;
}
} // namespace trigger_hotkey
