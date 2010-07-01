TEMPLATE = lib
TARGET = nutrition
CONFIG += dll \
    staticlib
VERSION = 0.1.0
QT += core \
    sql
HEADERS += libnutrition/data/amount.h \
    libnutrition/data/composite_food.h \
    libnutrition/data/food.h \
    libnutrition/data/food_amount.h \
    libnutrition/data/food_collection.h \
    libnutrition/data/food_component.h \
    libnutrition/data/group.h \
    libnutrition/data/meal.h \
    libnutrition/data/nutrient.h \
    libnutrition/data/nutrient_amount.h \
    libnutrition/data/single_food.h \
    libnutrition/data/specialized_unit.h \
    libnutrition/data/template.h \
    libnutrition/data/unit.h \
    libnutrition/data/user.h
SOURCES += libnutrition/data/composite_food.cpp \
    libnutrition/data/food.cpp \
    libnutrition/data/food_amount.cpp \
    libnutrition/data/food_collection.cpp \
    libnutrition/data/food_component.cpp \
    libnutrition/data/group.cpp \
    libnutrition/data/meal.cpp \
    libnutrition/data/nutrient.cpp \
    libnutrition/data/nutrient_amount.cpp \
    libnutrition/data/single_food.cpp \
    libnutrition/data/specialized_unit.cpp \
    libnutrition/data/template.cpp \
    libnutrition/data/unit.cpp \
    libnutrition/data/user.cpp
FORMS += 
RESOURCES += 
