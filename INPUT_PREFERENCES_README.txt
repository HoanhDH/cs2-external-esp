INPUT PREFERENCES (UI ONLY)

Changed files:
  src/config/Current.hpp
  src/config/Config.cpp
  src/gui/frontend/menu/Menu.cpp
New file:
  src/gui/frontend/menu/InputPreferences.hpp

Open cs2-external-esp.sln in Visual Studio 2026.
Check C++ dependencies in README: the repository ZIP may omit Git submodules.
Open Settings -> Input Preferences. Click the key button, press a keyboard key
or Mouse 4/Mouse 5, Esc cancels; choose Hold/Toggle and Head/Body.
The live ACTIVE/INACTIVE label is a menu-only key-state preview.
Your existing Config::Write call persists these preferences in config.json.
This patch does NOT aim, trigger shots, read game targets, or send input to CS2.
Build was NOT executed under Windows/MSVC.
