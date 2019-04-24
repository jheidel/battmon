#include "settings.h"

#include <EEPROM.h>

PersistentSettings settings = {};
RuntimeSettings runtime = {};

namespace {

inline bool SettingsValid() {
  return settings.magic == SETTINGS_MAGIC &&
         settings.size == sizeof(PersistentSettings);
}

void InitSettings() {
  memset(&settings, 0, sizeof(settings));
  // Defaults
  settings.magic = SETTINGS_MAGIC;
  settings.size = sizeof(PersistentSettings);
  settings.enable_heartbeat = true;
}

} // namespace

void LoadSettings() {
  EEPROM.get(0, settings);
  if (!SettingsValid()) {
    InitSettings();
    PersistSettings();
  }
}

void PersistSettings() {
  EEPROM.put(0, settings);
}
