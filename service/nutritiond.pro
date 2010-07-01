TEMPLATE = app
TARGET = nutritiond
QT += core \
    sql \
    network
    
# TODO: Avoid specifying both ../ and ../../

DEPENDPATH += . \
    src \
    ../src \
    ../common/ \
    ../../common 
INCLUDEPATH += . \
    src \
    ../src \
    ../common/ \
    ../../common
    
LIBS += -L../common \
    -lnutrition

HEADERS += 
SOURCES += src/main.cpp
FORMS += 
RESOURCES += 
