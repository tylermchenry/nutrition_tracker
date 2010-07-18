/*
 * service_back_end_proto_kludge.cpp
 *
 *  Created on: Jul 16, 2010
 *      Author: tyler
 */

/*
 * This is a kludge to get qmake to generate all of the proto buffers
 * before trying to build non-generated source files
 */

#include "libnutrition/proto/data/food_component.pb.h"
#include "libnutrition/proto/data/composite_food.pb.h"
#include "libnutrition/proto/data/group.pb.h"
#include "libnutrition/proto/data/meal.pb.h"
#include "libnutrition/proto/data/nutrient.pb.h"
#include "libnutrition/proto/data/single_food.pb.h"
#include "libnutrition/proto/data/specialized_unit.pb.h"
#include "libnutrition/proto/data/template.pb.h"
#include "libnutrition/proto/data/unit.pb.h"
#include "libnutrition/proto/data/user.pb.h"
#include "libnutrition/proto/data/data.pb.h"
#include "libnutrition/proto/service/error.pb.h"
#include "libnutrition/proto/service/composite_food_messages.pb.h"
#include "libnutrition/proto/service/group_messages.pb.h"
#include "libnutrition/proto/service/meal_messages.pb.h"
#include "libnutrition/proto/service/nutrient_messages.pb.h"
#include "libnutrition/proto/service/single_food_messages.pb.h"
#include "libnutrition/proto/service/specialized_unit_messages.pb.h"
#include "libnutrition/proto/service/template_messages.pb.h"
#include "libnutrition/proto/service/unit_messages.pb.h"
#include "libnutrition/proto/service/user_messages.pb.h"
#include "libnutrition/proto/service/data_messages.pb.h"
#include "libnutrition/proto/service/login_messages.pb.h"
#include "libnutrition/proto/service/search_messages.pb.h"
