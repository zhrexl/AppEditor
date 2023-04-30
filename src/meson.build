#project('AppEditor', 'c')

gnome = import('gnome')

executable('AppEditor',
  'Window/window.h',
  'Window/window.c',
  'main.c',
  dependencies: [
    dependency('gtk4'),
    dependency('libadwaita-1'),
    dependency('glib-2.0'),
    dependency('gio-2.0'),
  ],
  install: true,
)
