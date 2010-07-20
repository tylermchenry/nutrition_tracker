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
    -lnutrition \
    -lprotobuf
HEADERS += src/client_connection.h \
    src/nutrition_server_application.h
SOURCES += src/client_connection.cpp \
    src/nutrition_server_application.cpp \
    src/main.cpp
FORMS += 
RESOURCES += 
