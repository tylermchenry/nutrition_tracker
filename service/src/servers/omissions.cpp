/*
 * omissions.cpp
 *
 *  Created on: Jul 21, 2010
 *      Author: tyler
 */

#include "servers/omissions.h"
#include "libnutrition/proto/service/data_messages.pb.h"

Omissions::Omissions()
  : single_foods(false), composite_foods(false), templates(false),
    meals(false), nutrients(false), units(false), specializedUnits(false),
    groups(false), users(false)
{
}

Omissions::Omissions(const DataLoadRequest& req)
  : single_foods(req.has_singlefoodloadrequest() && req.singlefoodloadrequest().omit()),
    composite_foods(req.has_compositefoodloadrequest() && req.compositefoodloadrequest().omit()),
    templates(req.has_templateloadrequest() && req.templateloadrequest().omit()),
    meals(req.has_mealloadrequest() && req.mealloadrequest().omit()),
    nutrients(req.has_nutrientloadrequest() && req.nutrientloadrequest().omit()),
    units(req.has_unitloadrequest() && req.unitloadrequest().omit()),
    specializedUnits(req.has_specializedunitloadrequest() && req.specializedunitloadrequest().omit()),
    groups(req.has_grouploadrequest() && req.grouploadrequest().omit()),
    users(req.has_userloadrequest() && req.userloadrequest().omit())
{
}
