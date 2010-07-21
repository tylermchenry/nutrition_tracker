/*
 * omissions.h
 *
 *  Created on: Jul 21, 2010
 *      Author: tyler
 */

#ifndef OMISSIONS_H_
#define OMISSIONS_H_

class DataLoadRequest; // forward decl

struct Omissions
{
  bool single_foods;
  bool composite_foods;
  bool templates;
  bool meals;
  bool nutrients;
  bool units;
  bool specializedUnits;
  bool groups;
  bool users;

  Omissions();

  Omissions(const DataLoadRequest& req);
};


#endif /* OMISSIONS_H_ */
