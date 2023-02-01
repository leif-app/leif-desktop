CONFIG(release, debug|release):BINPATH = release
CONFIG(debug, debug|release):BINPATH = debug

pathtest.commands = @echo $$shadowed(../$$BINPATH/leif.exe)
copyssllib.commands = copy $$system_path($$[QT_INSTALL_PREFIX]/../../Tools/OpenSSL/Win_x86/bin/*.dll) $$system_path($$BINPATH/)

procopyplugin.target = $$system_path($$shadowed(../$$BINPATH/plugins))
precopyplugin.commands = IF NOT EXIST $$procopyplugin.target MD $$procopyplugin.target

copyplugin.commands = copy $$system_path($$shadowed(../../plugins/uk/$$BINPATH/uk1.dll)) $$procopyplugin.target
copyplugin.depends = precopyplugin

QMAKE_EXTRA_TARGETS += copyplugin precopyplugin copyssllib pathtest

PRE_TARGETDEPS *= copyssllib pathtest copyplugin
