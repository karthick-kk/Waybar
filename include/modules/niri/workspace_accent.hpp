#pragma once

#include <array>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <string>

namespace waybar::modules::niri {

using Rgb = std::array<int, 3>;

// Parse "#rrggbb"; anything else is rejected so a typo cannot produce a
// half-applied gradient.
inline std::optional<Rgb> parseHexColor(const std::string& value) {
  if (value.size() != 7 || value.front() != '#') return std::nullopt;
  Rgb rgb{};
  for (int i = 0; i < 3; ++i) {
    const auto part = value.substr(static_cast<size_t>(1 + i * 2), 2);
    for (char c : part) {
      if (std::isxdigit(static_cast<unsigned char>(c)) == 0) return std::nullopt;
    }
    rgb[static_cast<size_t>(i)] = static_cast<int>(std::strtol(part.c_str(), nullptr, 16));
  }
  return rgb;
}

inline std::string toHex(const Rgb& rgb) {
  char buffer[8];
  std::snprintf(buffer, sizeof(buffer), "#%02x%02x%02x", rgb[0], rgb[1], rgb[2]);
  return buffer;
}

inline Rgb mixColor(const Rgb& from, const Rgb& to, double amount) {
  Rgb out{};
  for (size_t i = 0; i < 3; ++i) {
    out[i] = static_cast<int>(from[i] + (to[i] - from[i]) * amount + 0.5);
  }
  return out;
}

// CSS for the active tile, derived from a single accent colour: light
// specular top, accent body, darkened bottom edge, and a matching glow.
// Returns an empty string when the accent is not a valid #rrggbb colour.
inline std::string activeTileCss(const std::string& accent) {
  const auto parsed = parseHexColor(accent);
  if (!parsed) return {};

  const auto base = *parsed;
  const auto light = mixColor(base, {255, 255, 255}, 0.55);
  const auto upper = mixColor(base, {255, 255, 255}, 0.18);
  const auto lower = mixColor(base, {0, 0, 0}, 0.30);
  const auto dark = mixColor(base, {0, 0, 0}, 0.55);

  return "#niri-workspace-strip button.ws-tile.active {\n"
         "  background-image: linear-gradient(to bottom, " +
         toHex(light) + " 0%, " + toHex(upper) + " 45%, " + toHex(lower) + " 55%, " +
         toHex(dark) +
         " 100%);\n"
         "  box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.85),\n"
         "              inset 0 -1px 0 rgba(0, 0, 0, 0.55),\n"
         "              0 0 7px rgba(" +
         std::to_string(base[0]) + ", " + std::to_string(base[1]) + ", " +
         std::to_string(base[2]) +
         ", 0.65);\n"
         "}\n";
}

}  // namespace waybar::modules::niri
