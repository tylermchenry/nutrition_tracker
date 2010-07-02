#include "template.h"
#include "impl/template_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"

int Template::tempId = -1;

QSharedPointer<Template> Template::createNewTemplate
  (const QSharedPointer<const Template>& copy)
{
  QSharedPointer<Template> food(new TemplateImpl(copy));
  DataCache<Template>::getInstance().insert(food->getTemplateId(), food);
  return food;
}

QSharedPointer<Template> Template::getTemplate(int id)
{
  if (DataCache<Template>::getInstance().contains(id)) {
    return DataCache<Template>::getInstance().get(id);
  } else {
    return BackEnd::getBackEnd()->loadTemplate(id);
  }
}

QMultiMap<QString, int> Template::getFoodsForUser(int userId)
{
  return BackEnd::getBackEnd()->loadTemplateNamesForUser(userId);
}

QSharedPointer<Food> Template::getCanonicalSharedPointer() const
{
  return DataCache<Template>::getInstance().get(getTemplateId());
}

