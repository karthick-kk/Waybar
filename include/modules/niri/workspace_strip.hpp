#pragma once

#include <gtkmm/box.h>
#include <gtkmm/cssprovider.h>
#include <json/value.h>

#include <memory>

#include <string>
#include <unordered_map>
#include <vector>

#include "AModule.hpp"
#include "bar.hpp"
#include "modules/niri/backend.hpp"
#include "util/icon_loader.hpp"

namespace waybar::modules::niri {

// Workspace strip: one embossed tile per populated (or active) workspace,
// carrying the real icon of every distinct app open on it.
// Data comes from the shared niri IPC event stream (no polling); tiles are
// rebuilt only when the signature of visible state actually changes.
class WorkspaceStrip : public AModule, public EventHandler {
 public:
  WorkspaceStrip(const std::string& id, const Bar& bar, const Json::Value& config);
  ~WorkspaceStrip() override;

  void update() override;

 private:
  struct TileData {
    unsigned id{};
    unsigned idx{};
    bool active{};
    std::vector<std::string> app_ids;
  };

  void onEvent(const Json::Value& ev) override;
  bool handleScroll(GdkEventScroll* event) override;
  void rebuild(const std::vector<TileData>& tiles);
  std::vector<TileData> collect() const;
  static std::string signatureOf(const std::vector<TileData>& tiles);
  std::string resolveIconName(const std::string& app_id);

  const Bar& bar_;
  Gtk::Box box_;
  Glib::RefPtr<Gtk::CssProvider> accent_provider_;
  std::unique_ptr<IconLoader> icon_loader_;
  unsigned icon_size_{16};
  std::string last_signature_;
  std::unordered_map<std::string, std::string> icon_cache_;
};

}  // namespace waybar::modules::niri
