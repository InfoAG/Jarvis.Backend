TEMPLATE = subdirs

SUBDIRS += modules/basicarith \
    modules/variables \
    modules/flowcontrol

CONFIG += ordered

SUBDIRS += build

OTHER_FILES += \
    modules/basicarith.jpkg \
    modules/variables.jpkg \
    modules/flowcontrol.jpkg
