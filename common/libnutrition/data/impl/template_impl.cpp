#include "template_impl.h"
#include "libnutrition/backend/back_end.h"
#include <QDebug>

TemplateImpl::TemplateImpl(int id, const QString& name, int ownerId,
                      const QList<FoodComponent>& components)
  : FoodCollectionImpl
      ("TEMPLATE_" + QString::number(id), name, ownerId, components,
       0, 0, 0, 0),
    id(id)
{
  if (needsToBeReSaved()) {
    saveToDatabase();
  }
}

TemplateImpl::TemplateImpl(const QSharedPointer<const Template>& copy)
  : FoodCollectionImpl("TEMPLATE_" + QString::number(tempId), copy),
    id(tempId--)
{
  qDebug() << "Created new template with temporary ID " << id;
}

TemplateImpl::~TemplateImpl()
{
}

void TemplateImpl::saveToDatabase()
{
  BackEnd::getBackEnd()->storeTemplate
    (getCanonicalSharedPointer().dynamicCast<Template>());
}

void TemplateImpl::deleteFromDatabase()
{
  BackEnd::getBackEnd()->deleteTemplate
    (getCanonicalSharedPointer().dynamicCast<Template>());
}
