# #####################################################################
# Automatically generated by qmake (2.01a) Sun May 9 06:23:55 2010
# #####################################################################
QT += sql network
win32:CONFIG += static
TEMPLATE = app
TARGET = 

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

# Generate a few compiled-in strings that are used for version
# identification in the About box
versiontarget.target = version.cpp
win32:versiontarget.commands = make_version.bat
else:versiontarget.commands = ./make_version.sh
versiontarget.depends = FORCE
PRE_TARGETDEPS += version.cpp
QMAKE_EXTRA_TARGETS += versiontarget
LIBS += -L../common \
    -lnutrition \
    -lprotobuf

# Input
HEADERS += src/dialogs/about.h \
    src/dialogs/add_food.h \
    src/dialogs/database_information.h \
    src/dialogs/edit_composite_food.h \
    src/dialogs/edit_food.h \
    src/dialogs/edit_template.h \
    src/dialogs/fill_in.h \
    src/dialogs/instantiate_template.h \
    src/dialogs/my_foods.h \
    src/dialogs/options.h \
    src/dialogs/select_template.h \
    src/dialogs/user_login.h \
    src/dialogs/view_food.h \
    src/menus/food_amount_context_menu.h \
    src/menus/food_component_context_menu.h \
    src/menus/food_context_menu.h \
    src/menus/meal_context_menu.h \
    src/widgets/food_search_control.h \
    src/widgets/food_tree.h \
    src/widgets/nutrient_amount_display.h \
    src/widgets/nutrition_tracker.h \
    src/widgets/select_food_control.h \
    src/widgets/template_components.h \
    src/nutrition_tracker_application.h \
    version.h \
    src/model/food_tree_collection_item.h \
    src/model/food_tree_amount_item.h \
    src/model/food_tree_component_item.h \
    src/model/food_tree_meal_item.h \
    src/model/food_tree_model.h \
    src/model/food_tree_item.h \
    src/model/variant_value_item_model.h \
    src/nutrition_tracker_main.h
FORMS += src/nutrition_tracker_main.ui \
	src/widgets/food_search_control.ui \
	src/widgets/food_tree.ui \
	src/widgets/nutrition_tracker.ui \
	src/widgets/select_food_control.ui \
	src/widgets/template_components.ui \
	src/dialogs/about.ui \
	src/dialogs/add_food.ui \
	src/dialogs/database_information.ui \
	src/dialogs/edit_composite_food.ui \
	src/dialogs/edit_food.ui \
	src/dialogs/edit_template.ui \
	src/dialogs/fill_in.ui \
	src/dialogs/instantiate_template.ui \
	src/dialogs/my_foods.ui \
	src/dialogs/options.ui \
	src/dialogs/select_template.ui \
	src/dialogs/user_login.ui \
	src/dialogs/view_food.ui
SOURCES += src/dialogs/about.cpp \
    src/dialogs/add_food.cpp \
    src/dialogs/database_information.cpp \
    src/dialogs/edit_composite_food.cpp \
    src/dialogs/edit_food.cpp \
    src/dialogs/edit_template.cpp \
    src/dialogs/fill_in.cpp \
    src/dialogs/instantiate_template.cpp \
    src/dialogs/my_foods.cpp \
    src/dialogs/options.cpp \
    src/dialogs/select_template.cpp \
    src/dialogs/user_login.cpp \
    src/dialogs/view_food.cpp \
    src/menus/food_amount_context_menu.cpp \
    src/menus/food_component_context_menu.cpp \
    src/menus/food_context_menu.cpp \
    src/menus/meal_context_menu.cpp \
    src/widgets/food_search_control.cpp \
    src/widgets/food_tree.cpp \
    src/widgets/nutrient_amount_display.cpp \
    src/widgets/nutrition_tracker.cpp \
    src/widgets/select_food_control.cpp \
    src/widgets/template_components.cpp \
    src/nutrition_tracker_application.cpp \
    version.cpp \
    src/model/food_tree_collection_item.cpp \
    src/model/food_tree_amount_item.cpp \
    src/model/food_tree_component_item.cpp \
    src/model/food_tree_meal_item.cpp \
    src/model/food_tree_model.cpp \
    src/model/food_tree_item.cpp \
    src/main.cpp \
    src/nutrition_tracker_main.cpp
