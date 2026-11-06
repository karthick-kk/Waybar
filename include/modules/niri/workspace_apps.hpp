#pragma once

#include <string>
#include <vector>

namespace waybar::modules::niri {

// Keep one entry per window, not per application: multiple instances of the
// same app (e.g. three foot terminals) must render three icons.
inline std::vector<std::string> workspaceAppInstances(const std::vector<std::string>& app_ids) {
  std::vector<std::string> instances;
  instances.reserve(app_ids.size());
  for (const auto& app_id : app_ids) {
    if (!app_id.empty()) instances.push_back(app_id);
  }
  return instances;
}

}  // namespace waybar::modules::niri
