#include "settings.h"

#include <EEPROM.h>

Settings settings = {};

namespace {

inline void InitSettings() {
  settings = {};
  settings.magic = SETTINGS_MAGIC;
  settings.size = sizeof(Settings);
}

inline bool SettingsValid() {
  return settings.magic == SETTINGS_MAGIC && settings.size == sizeof(Settings);
}

} // namespace

void LoadSettings() {
  EEPROM.get(0, settings);
  if (!SettingsValid()) {
    InitSettings();
    PersistSettings();
  }
}

void PersistSettings() { EEPROM.put(0, settings); }