#include "settings.h"

#include <EEPROM.h>

PersistentSettings settings = {};
RuntimeSettings runtime = {};

namespace {

inline void InitSettings() {
  settings = {};
  settings.magic = SETTINGS_MAGIC;
  settings.size = sizeof(PersistentSettings);
}

inline bool SettingsValid() {
  return settings.magic == SETTINGS_MAGIC &&
         settings.size == sizeof(PersistentSettings);
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
