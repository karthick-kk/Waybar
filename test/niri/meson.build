test_inc = include_directories('../../include')

test_dep = [catch2, fmt, gtkmm, jsoncpp, spdlog]
test_src = files('../main.cpp', 'workspace_navigation.cpp')

niri_test = executable(
  'niri_test', test_src, dependencies: test_dep, include_directories: test_inc,
)
test('niri', niri_test, workdir: meson.project_source_root())
