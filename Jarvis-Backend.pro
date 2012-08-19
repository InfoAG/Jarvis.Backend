TEMPLATE = subdirs

SUBDIRS += modules/basicarith \
    modules/variables

CONFIG += ordered

SUBDIRS += build

OTHER_FILES += \
    modules/basicarith.jpkg \
    modules/variables.jpkg
