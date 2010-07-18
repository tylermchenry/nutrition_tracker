TEMPLATE = lib
TARGET = nutrition
CONFIG += dll \
    staticlib
VERSION = 0.1.0
QT += core \
    sql \
    network
LIBS += -lprotobuf
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
# must come first, and Data must come after everything except Login and Search
PROTOS += libnutrition/proto/error.proto \
    libnutrition/proto/food_component.proto \
    libnutrition/proto/composite_food.proto \
    libnutrition/proto/group.proto \
    libnutrition/proto/meal.proto \
    libnutrition/proto/nutrient.proto \
    libnutrition/proto/single_food.proto \
    libnutrition/proto/specialized_unit.proto \
    libnutrition/proto/template.proto \
    libnutrition/proto/unit.proto \
    libnutrition/proto/user.proto \
    libnutrition/proto/data.proto \
    libnutrition/proto/login.proto \
    libnutrition/proto/search.proto
protobuf_decl.name = protobuf \
    header
protobuf_decl.input = PROTOS
protobuf_decl.output = libnutrition/proto/${QMAKE_FILE_BASE}.pb.h
protobuf_decl.commands = protoc \
    --cpp_out="." \
    ${QMAKE_FILE_NAME}
protobuf_decl.variable_out = GENERATED_FILES
QMAKE_EXTRA_COMPILERS += protobuf_decl
protobuf_impl.name = protobuf \
    implementation
protobuf_impl.input = PROTOS
protobuf_impl.output = libnutrition/proto/${QMAKE_FILE_BASE}.pb.cc
protobuf_impl.depends = libnutrition/proto/${QMAKE_FILE_BASE}.pb.h
protobuf_impl.commands = $$escape_expand(\n)
protobuf_impl.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl
