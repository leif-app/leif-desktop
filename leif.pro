TEMPLATE = subdirs

SUBDIRS = plugins leif tests

leif.depends = plugins
test.depeds = leif