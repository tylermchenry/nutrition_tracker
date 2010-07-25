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
    -L../common/debug \
    -L../common/release \
    -lnutrition \
    -lprotobuf
win32:LIBS += -lpthread \
    -lwsock32
HEADERS += src/servers/listing.h \
    src/servers/response_objects.h \
    src/servers/food_server.h \
    src/servers/omissions.h \
    src/servers/composite_food_server.h \
    src/servers/data_server.h \
    src/servers/group_server.h \
    src/servers/login_server.h \
    src/servers/meal_server.h \
    src/servers/nutrient_server.h \
    src/servers/search_server.h \
    src/servers/single_food_server.h \
    src/servers/specialized_unit_server.h \
    src/servers/template_server.h \
    src/servers/unit_server.h \
    src/servers/user_server.h \
    src/client_connection.h \
    src/nutrition_server_application.h
SOURCES += src/servers/food_server.cpp \
    src/servers/omissions.cpp \
    src/servers/composite_food_server.cpp \
    src/servers/data_server.cpp \
    src/servers/group_server.cpp \
    src/servers/login_server.cpp \
    src/servers/meal_server.cpp \
    src/servers/nutrient_server.cpp \
    src/servers/search_server.cpp \
    src/servers/single_food_server.cpp \
    src/servers/specialized_unit_server.cpp \
    src/servers/template_server.cpp \
    src/servers/unit_server.cpp \
    src/servers/user_server.cpp \
    src/client_connection.cpp \
    src/nutrition_server_application.cpp \
    src/main.cpp
FORMS += 
RESOURCES += 
