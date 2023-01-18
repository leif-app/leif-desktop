copyssllib.commands = copy $$system_path($$[QT_INSTALL_PREFIX]/../../Tools/OpenSSL/Win_x86/bin/*.dll) $$system_path(release/)

QMAKE_EXTRA_TARGETS *= copyssllib

POST_TARGETDEPS *= copyssllib
