#pragma once

#include <windows.h>
#include <string>
#include "Resource.h"

namespace click_practice {
struct Settings {
    int key = VK_F6;
    const char* key_name = "F6";
    int delay_ms = 150;
    int zone = 2; // 0 = head, 1 = body, 2 = either
    bool toggle = false;
};

struct Session {
    Settings settings;
    bool key_down = false;
    bool active = false;
    bool fired = false;
    HWND pending = nullptr;
    ULONGLONG due = 0;
    HWND receiving = nullptr;
    unsigned clicks = 0;

    void Disarm() {
        active = false;
        fired = false;
        pending = nullptr;
    }

    void Tick(HWND dialog) {
        const bool down = (GetKeyState(settings.key) & 0x8000) != 0;
        if (GetForegroundWindow() != dialog) {
            Disarm();
        } else if (down && !key_down) {
            if (active && settings.toggle)
                Disarm();
            else
                active = true;
        } else if (!down && !settings.toggle) {
            Disarm();
        }
        key_down = down;
        if (!active || fired) return;

        POINT cursor{};
        const HWND hit = GetCursorPos(&cursor) ? WindowFromPoint(cursor) : nullptr;
        const HWND head = GetDlgItem(dialog, IDC_PRACTICE_HEAD);
        const HWND body = GetDlgItem(dialog, IDC_PRACTICE_BODY);
        if (!hit || !((hit == head && settings.zone != 1) || (hit == body && settings.zone != 0))) {
            pending = nullptr;
            return;
        }

        const ULONGLONG now = GetTickCount64();
        if (hit != pending) {
            pending = hit;
            due = now + settings.delay_ms;
        }
        if (now < due) return;

        fired = true;
        // BM_CLICK delivers a left-button down/up pair to this dialog's own control.
        receiving = hit;
        SendMessageW(receiving, BM_CLICK, 0, 0);
        receiving = nullptr;
    }
};

inline INT_PTR CALLBACK Procedure(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
    auto* session = reinterpret_cast<Session*>(GetWindowLongPtrW(dialog, DWLP_USER));
    switch (message) {
    case WM_INITDIALOG: {
        session = reinterpret_cast<Session*>(lparam);
        SetWindowLongPtrW(dialog, DWLP_USER, lparam);
        const auto& settings = session->settings;
        std::string help = std::string(settings.toggle ? "Toggle " : "Hold ") + settings.key_name +
            " over an enabled target for " + std::to_string(settings.delay_ms) + " ms.";
        SetDlgItemTextA(dialog, IDC_PRACTICE_HELP, help.c_str());
        EnableWindow(GetDlgItem(dialog, IDC_PRACTICE_HEAD), settings.zone != 1);
        EnableWindow(GetDlgItem(dialog, IDC_PRACTICE_BODY), settings.zone != 0);
        if (!SetTimer(dialog, 1, 10, nullptr)) EndDialog(dialog, -1);
        return TRUE;
    }
    case WM_ACTIVATE:
        if (session && LOWORD(wparam) == WA_INACTIVE) session->Disarm();
        if (session) session->key_down = (GetKeyState(session->settings.key) & 0x8000) != 0;
        break;
    case WM_TIMER:
        if (session && wparam == 1) session->Tick(dialog);
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wparam) == IDCANCEL) {
            EndDialog(dialog, 0);
            return TRUE;
        }
        if (session && session->receiving && reinterpret_cast<HWND>(lparam) == session->receiving &&
            HIWORD(wparam) == BN_CLICKED) {
            auto text = std::wstring(L"Clicks received: ") + std::to_wstring(++session->clicks);
            SetDlgItemTextW(dialog, IDC_PRACTICE_STATUS, text.c_str());
            return TRUE;
        }
        break;
    case WM_CLOSE:
        EndDialog(dialog, 0);
        return TRUE;
    }
    return FALSE;
}

inline bool Run(HWND owner, Settings settings = {}) {
    Session session{settings};
    return DialogBoxParamW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(IDD_CLICK_PRACTICE),
                           owner, Procedure, reinterpret_cast<LPARAM>(&session)) != -1;
}
} // namespace click_practice
