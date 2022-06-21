TEMPLATE = subdirs
SUBDIRS += \
    instrument-cluster \
    ch1-basics \
    ch2-enums-structs \
    ch3-simulation-backend \
    ch4-simulation-behavior \
    ch5-ipc \

!win32:qtHaveModule(dbus): SUBDIRS += ch6-own-backend
