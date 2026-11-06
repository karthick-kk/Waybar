#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace waybar::modules::niri {

// Return the adjacent visible workspace id, wrapping at either end.
// `visible` is the strip's rendered order: populated workspaces plus active.
inline std::optional<uint64_t> adjacentVisibleWorkspaceId(
    const std::vector<std::pair<uint64_t, bool>>& visible, int direction) {
  if (visible.empty() || direction == 0) return std::nullopt;

  for (std::size_t i = 0; i < visible.size(); ++i) {
    if (!visible[i].second) continue;
    const auto next = direction > 0 ? (i + 1) % visible.size()
                                    : (i + visible.size() - 1) % visible.size();
    return visible[next].first;
  }

  return std::nullopt;
}

}  // namespace waybar::modules::niri
