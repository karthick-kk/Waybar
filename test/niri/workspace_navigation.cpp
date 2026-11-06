#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "modules/niri/workspace_navigation.hpp"
#include "modules/niri/workspace_accent.hpp"
#include "modules/niri/workspace_apps.hpp"

using waybar::modules::niri::adjacentVisibleWorkspaceId;
using waybar::modules::niri::workspaceAppInstances;

TEST_CASE("workspace strip skips unpopulated workspace indexes", "[niri][workspace-strip]") {
  const std::vector<std::pair<uint64_t, bool>> visible{{3, true}, {5, false}};

  SECTION("scroll down from 3 goes directly to 5, not empty 4") {
    REQUIRE(adjacentVisibleWorkspaceId(visible, +1) == 5);
  }

  SECTION("scroll up from 5 goes directly to 3") {
    const std::vector<std::pair<uint64_t, bool>> from_five{{3, false}, {5, true}};
    REQUIRE(adjacentVisibleWorkspaceId(from_five, -1) == 3);
  }

  SECTION("wrap from last visible tile to first") {
    const std::vector<std::pair<uint64_t, bool>> from_five{{3, false}, {5, true}};
    REQUIRE(adjacentVisibleWorkspaceId(from_five, +1) == 3);
  }
}

TEST_CASE("workspace strip accent comes from config", "[niri][workspace-strip][accent]") {
  using waybar::modules::niri::activeTileCss;
  using waybar::modules::niri::parseHexColor;

  SECTION("valid accent produces an active-tile gradient with that base colour") {
    const auto css = activeTileCss("#008080");
    REQUIRE(css.find("#niri-workspace-strip button.ws-tile.active") != std::string::npos);
    REQUIRE(css.find("linear-gradient") != std::string::npos);
    REQUIRE(css.find("rgba(0, 128, 128, 0.65)") != std::string::npos);
  }

  SECTION("invalid accents are rejected so style.css stays in charge") {
    REQUIRE(activeTileCss("teal").empty());
    REQUIRE(activeTileCss("#12345").empty());
    REQUIRE(activeTileCss("#gggggg").empty());
    REQUIRE(parseHexColor("#008080").has_value());
  }
}

TEST_CASE("workspace strip shows one icon per window instance", "[niri][workspace-strip][icons]") {
  SECTION("three foot terminals produce three icons, not one") {
    const auto icons = workspaceAppInstances({"foot", "foot", "foot"});
    REQUIRE(icons.size() == 3);
    REQUIRE(icons[0] == "foot");
    REQUIRE(icons[2] == "foot");
  }

  SECTION("distinct apps keep their layout order and count") {
    const auto icons = workspaceAppInstances({"zcode", "org.gnome.TextEditor"});
    REQUIRE(icons == std::vector<std::string>{"zcode", "org.gnome.TextEditor"});
  }

  SECTION("windows without an app id are skipped") {
    const auto icons = workspaceAppInstances({"foot", "", "floorp"});
    REQUIRE(icons == std::vector<std::string>{"foot", "floorp"});
  }
}

TEST_CASE("workspace strip navigation handles missing active or empty list", "[niri][workspace-strip]") {
  REQUIRE_FALSE(adjacentVisibleWorkspaceId({}, +1).has_value());
  REQUIRE_FALSE(adjacentVisibleWorkspaceId({{3, false}, {5, false}}, +1).has_value());
  REQUIRE_FALSE(adjacentVisibleWorkspaceId({{3, true}}, 0).has_value());
}
