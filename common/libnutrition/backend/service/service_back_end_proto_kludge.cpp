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

#include "libnutrition/proto/error.pb.h"
#include "libnutrition/proto/food_component.pb.h"
#include "libnutrition/proto/composite_food.pb.h"
#include "libnutrition/proto/group.pb.h"
#include "libnutrition/proto/meal.pb.h"
#include "libnutrition/proto/nutrient.pb.h"
#include "libnutrition/proto/single_food.pb.h"
#include "libnutrition/proto/specialized_unit.pb.h"
#include "libnutrition/proto/template.pb.h"
#include "libnutrition/proto/unit.pb.h"
#include "libnutrition/proto/user.pb.h"
#include "libnutrition/proto/data.pb.h"
#include "libnutrition/proto/login.pb.h"
#include "libnutrition/proto/search.pb.h"
