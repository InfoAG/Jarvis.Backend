TEMPLATE = subdirs

SUBDIRS += modules/basicarith \
    modules/variables

CONFIG += ordered

SUBDIRS += build

OTHER_FILES += \
    Modules/basicarith.jpkg \
    Modules/variables.jpkg
