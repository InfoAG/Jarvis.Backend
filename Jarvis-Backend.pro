TEMPLATE = subdirs

SUBDIRS += build-lib \
    cextensions/std

CONFIG += ordered

SUBDIRS += modules/basicarith \
    modules/variables \
    modules/flowcontrol \
    build

OTHER_FILES += \
    modules/basicarith.jpkg \
    modules/variables.jpkg \
    modules/flowcontrol.jpkg \
    debug/cextensions/std.jce
