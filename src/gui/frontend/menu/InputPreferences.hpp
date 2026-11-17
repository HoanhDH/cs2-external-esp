#pragma once
#include <imgui.h>
#include "config/Current.hpp"

// Menu-only input settings: never reads game memory or issues game input.
namespace input_preferences_ui {

inline const char* KeyLabel(int key) {
    if (key == static_cast<int>(ImGuiKey_MouseX1)) return "Mouse 4";
    if (key == static_cast<int>(ImGuiKey_MouseX2)) return "Mouse 5";
    if (key <= 0) return "Unassigned";
    const char* name = ImGui::GetKeyName(static_cast<ImGuiKey>(key));
    return (name && name[0]) ? name : "Unknown";
}

inline bool IsHeld(int key) {
    if (key == static_cast<int>(ImGuiKey_MouseX1)) return ImGui::IsMouseDown(3);
    if (key == static_cast<int>(ImGuiKey_MouseX2)) return ImGui::IsMouseDown(4);
    return ImGui::IsKeyDown(static_cast<ImGuiKey>(key));
}

inline bool IsPressed(int key) {
    if (key == static_cast<int>(ImGuiKey_MouseX1)) return ImGui::IsMouseClicked(3);
    if (key == static_cast<int>(ImGuiKey_MouseX2)) return ImGui::IsMouseClicked(4);
    return ImGui::IsKeyPressed(static_cast<ImGuiKey>(key), false);
}

inline void Render() {
    auto& cfg = cfg::input_preferences;
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::TextUnformatted("Input Preferences (UI only)");
    ImGui::TextDisabled("No automatic aiming or firing is connected.");

    ImGui::TextUnformatted("Activation Key");
    ImGui::SameLine();
    if (ImGui::Button(KeyLabel(cfg.activation_key))) {
        ImGui::OpenPopup("Assign Activation Key");
    }

    if (ImGui::BeginPopupModal("Assign Activation Key", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextUnformatted("PRESS ANY KEY...");
        ImGui::TextDisabled("Mouse 4 / Mouse 5 supported. ESC to cancel.");
        if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
            ImGui::CloseCurrentPopup();
        } else if (ImGui::IsMouseClicked(3)) {
            cfg.activation_key = static_cast<int>(ImGuiKey_MouseX1);
            ImGui::CloseCurrentPopup();
        } else if (ImGui::IsMouseClicked(4)) {
            cfg.activation_key = static_cast<int>(ImGuiKey_MouseX2);
            ImGui::CloseCurrentPopup();
        } else {
            for (int k = static_cast<int>(ImGuiKey_NamedKey_BEGIN);
                 k < static_cast<int>(ImGuiKey_NamedKey_END); ++k) {
                if (k >= static_cast<int>(ImGuiKey_MouseLeft)
                    && k <= static_cast<int>(ImGuiKey_MouseWheelY)) continue;
                if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(k), false)) {
                    cfg.activation_key = k;
                    ImGui::CloseCurrentPopup();
                    break;
                }
            }
        }
        ImGui::EndPopup();
    }

    static const char* modes[] = { "Hold", "Toggle" };
    ImGui::Combo("Activation Mode", &cfg.activation_mode, modes, IM_ARRAYSIZE(modes));
    static const char* zones[] = { "Head", "Body" };
    ImGui::Combo("Aim Preference (display only)", &cfg.target_zone, zones, IM_ARRAYSIZE(zones));

    // Demonstrates Hold/Toggle input state while menu is open. No game action.
    static bool toggled = false;
    if (!ImGui::IsPopupOpen("Assign Activation Key")) {
        if (cfg.activation_mode == 1 && IsPressed(cfg.activation_key)) toggled = !toggled;
    }
    const bool active = !ImGui::IsPopupOpen("Assign Activation Key")
        && (cfg.activation_mode == 0 ? IsHeld(cfg.activation_key) : toggled);
    ImGui::Text("Key state preview: %s", active ? "ACTIVE" : "INACTIVE");
    ImGui::TextDisabled("Saved with existing config.json on normal config save.");
}

} // namespace input_preferences_ui
