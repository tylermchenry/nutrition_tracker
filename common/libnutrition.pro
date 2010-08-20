TEMPLATE = lib
TARGET = nutrition
CONFIG += dll \
    staticlib 
VERSION = 0.1.0
QT += core \
    sql \
    network
LIBS += -lprotobuf

DEPENDPATH += . \
    ../ 
INCLUDEPATH += . \
    ../ 

HEADERS += libnutrition/backend/service/service_back_end.h \
    libnutrition/backend/mysql/mysql_back_end.h \
    libnutrition/data/data_cache.h \
    libnutrition/backend/back_end.h \
    libnutrition/data/impl/composite_food_impl.h \
    libnutrition/data/impl/food_collection_impl.h \
    libnutrition/data/impl/food_impl.h \
    libnutrition/data/impl/group_impl.h \
    libnutrition/data/impl/meal_impl.h \
    libnutrition/data/impl/nutrient_impl.h \
    libnutrition/data/impl/single_food_impl.h \
    libnutrition/data/impl/specialized_unit_impl.h \
    libnutrition/data/impl/template_impl.h \
    libnutrition/data/impl/unit_impl.h \
    libnutrition/data/impl/user_impl.h \
    libnutrition/data/amount.h \
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
SOURCES += libnutrition/backend/service/service_back_end_proto_kludge.cpp \
    libnutrition/backend/service/service_back_end_search.cpp \
    libnutrition/backend/service/service_back_end_meal.cpp \
    libnutrition/backend/service/service_back_end_template.cpp \
    libnutrition/backend/service/service_back_end_composite_food.cpp \
    libnutrition/backend/service/service_back_end_single_food.cpp \
    libnutrition/backend/service/service_back_end_specialized_unit.cpp \
    libnutrition/backend/service/service_back_end_nutrient.cpp \
    libnutrition/backend/service/service_back_end_user.cpp \
    libnutrition/backend/service/service_back_end_unit.cpp \
    libnutrition/backend/service/service_back_end_group.cpp \
    libnutrition/backend/service/service_back_end_login.cpp \
    libnutrition/backend/service/service_back_end.cpp \
    libnutrition/backend/mysql/mysql_back_end_user.cpp \
    libnutrition/backend/mysql/mysql_back_end_composite_food.cpp \
    libnutrition/backend/mysql/mysql_back_end_group.cpp \
    libnutrition/backend/mysql/mysql_back_end_login.cpp \
    libnutrition/backend/mysql/mysql_back_end_meal.cpp \
    libnutrition/backend/mysql/mysql_back_end_nutrient.cpp \
    libnutrition/backend/mysql/mysql_back_end_search.cpp \
    libnutrition/backend/mysql/mysql_back_end_single_food.cpp \
    libnutrition/backend/mysql/mysql_back_end_specialized_unit.cpp \
    libnutrition/backend/mysql/mysql_back_end_template.cpp \
    libnutrition/backend/mysql/mysql_back_end_unit.cpp \
    libnutrition/backend/mysql/mysql_back_end.cpp \
    libnutrition/backend/back_end.cpp \
    libnutrition/data/impl/composite_food_impl.cpp \
    libnutrition/data/impl/food_collection_impl.cpp \
    libnutrition/data/impl/food_impl.cpp \
    libnutrition/data/impl/group_impl.cpp \
    libnutrition/data/impl/meal_impl.cpp \
    libnutrition/data/impl/nutrient_impl.cpp \
    libnutrition/data/impl/single_food_impl.cpp \
    libnutrition/data/impl/specialized_unit_impl.cpp \
    libnutrition/data/impl/template_impl.cpp \
    libnutrition/data/impl/unit_impl.cpp \
    libnutrition/data/impl/user_impl.cpp \
    libnutrition/data/composite_food.cpp \
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

# Order is important for the protos, since some depend on each other's generated
# header files, and qmake is not knowledgeable about this. Error and Component
# must come first, and Data must come after everything 

DATA_PROTOS += libnutrition/proto/data/food_component.proto \
    libnutrition/proto/data/composite_food.proto \
    libnutrition/proto/data/group.proto \
    libnutrition/proto/data/meal.proto \
    libnutrition/proto/data/nutrient.proto \
    libnutrition/proto/data/single_food.proto \
    libnutrition/proto/data/specialized_unit.proto \
    libnutrition/proto/data/template.proto \
    libnutrition/proto/data/unit.proto \
    libnutrition/proto/data/user.proto \
    libnutrition/proto/data/data.proto 
probuf_data_decl.name = protobuf data header
probuf_data_decl.input = DATA_PROTOS
probuf_data_decl.output = libnutrition/proto/data/${QMAKE_FILE_BASE}.pb.h
probuf_data_decl.commands = protoc \
    --cpp_out="." \
    libnutrition/proto/data/${QMAKE_FILE_IN_BASE}.proto
probuf_data_decl.variable_out = GENERATED_FILES
QMAKE_EXTRA_COMPILERS += probuf_data_decl
probuf_data_impl.name = protobuf data implementation
probuf_data_impl.input = DATA_PROTOS
probuf_data_impl.output = libnutrition/proto/data/${QMAKE_FILE_BASE}.pb.cc
probuf_data_impl.depends = libnutrition/proto/data/${QMAKE_FILE_BASE}.pb.h
probuf_data_impl.commands = $$escape_expand(\n)
probuf_data_impl.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += probuf_data_impl

# Order is important for the protos, since some depend on each other's generated
# header files, and qmake is not knowledgeable about this. Error, and Storage 
# Modification must come first, and Data must come after everything except 
# Login and Search

SERVICE_PROTOS += libnutrition/proto/service/error.proto \
    libnutrition/proto/service/storage_modification.proto \
    libnutrition/proto/service/composite_food_messages.proto \
    libnutrition/proto/service/group_messages.proto \
    libnutrition/proto/service/meal_messages.proto \
    libnutrition/proto/service/nutrient_messages.proto \
    libnutrition/proto/service/single_food_messages.proto \
    libnutrition/proto/service/specialized_unit_messages.proto \
    libnutrition/proto/service/template_messages.proto \
    libnutrition/proto/service/unit_messages.proto \
    libnutrition/proto/service/user_messages.proto \
    libnutrition/proto/service/data_messages.proto \
    libnutrition/proto/service/login_messages.proto \
    libnutrition/proto/service/search_messages.proto
probuf_service_decl.name = protobuf service header
probuf_service_decl.input = SERVICE_PROTOS
probuf_service_decl.output = libnutrition/proto/service/${QMAKE_FILE_BASE}.pb.h
probuf_service_decl.commands = protoc \
    --cpp_out="." \
    libnutrition/proto/service/${QMAKE_FILE_IN_BASE}.proto
probuf_service_decl.variable_out = GENERATED_FILES
QMAKE_EXTRA_COMPILERS += probuf_service_decl
probuf_service_impl.name = protobuf service implementation
probuf_service_impl.input = SERVICE_PROTOS
probuf_service_impl.output = libnutrition/proto/service/${QMAKE_FILE_BASE}.pb.cc
probuf_service_impl.depends = libnutrition/proto/service/${QMAKE_FILE_BASE}.pb.h
probuf_service_impl.commands = $$escape_expand(\n)
probuf_service_impl.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += probuf_service_impl


