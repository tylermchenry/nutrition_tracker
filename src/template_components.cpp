#include "template_components.h"
#include <QtGui/QSpacerItem>

TemplateComponents::TemplateComponents(const QSharedPointer<const Template>& templ,
                                             QWidget *parent)
  : QWidget(parent), templ(templ)
{
  ui.setupUi(this);

  QList<FoodComponent> components = templ->getComponents();

  for (QList<FoodComponent>::iterator i = components.begin(); i != components.end(); ++i)
  {
    componentWidgetGroups.append(ComponentWidgetGroup(i->getFoodAmount(), this));
  }

  ui.componentLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
                               ui.componentLayout->rowCount(), 0);
}

TemplateComponents::~TemplateComponents()
{
}

QSharedPointer<FoodCollection> TemplateComponents::getCollection() const
{
  QSharedPointer<FoodCollection> collection = FoodCollection::createFoodCollection(templ->getDisplayName());

  for (QList<ComponentWidgetGroup>::const_iterator i = componentWidgetGroups.begin();
       i != componentWidgetGroups.end(); ++i)
  {
    collection->addComponent(i->getFoodAmount());
  }

  return collection;
}

TemplateComponents::ComponentWidgetGroup::ComponentWidgetGroup
  (FoodAmount foodAmount, TemplateComponents* parent)
  : food(foodAmount.getFood()), lblFoodName(new QLabel(parent)),
    txtAmount(new QLineEdit(parent)), cbUnit(new QComboBox(parent)),
    chkIncludeRefuse(new QCheckBox(parent))
{
    int row = parent->ui.componentLayout->rowCount();

    parent->ui.componentLayout->addWidget(lblFoodName, row, 0);
    parent->ui.componentLayout->addWidget(txtAmount, row, 1);
    parent->ui.componentLayout->addWidget(cbUnit, row, 2);
    parent->ui.componentLayout->addWidget(chkIncludeRefuse, row, 3);

    lblFoodName->setText(food->getDisplayName());
    lblFoodName->setWordWrap(true);

    txtAmount->setText(QString::number(foodAmount.getAmount()));
    txtAmount->setMinimumWidth(50);
    txtAmount->setMaximumWidth(80);
    txtAmount->setAlignment(Qt::AlignRight);

    QMap<QString, QSharedPointer<const Unit> > unitsToShow;
    QList<Unit::Dimensions::Dimension> validDimensions = food->getValidDimensions();

    for (QList<Unit::Dimensions::Dimension>::const_iterator i = validDimensions.begin();
         i != validDimensions.end(); ++i)
    {
      QVector<QSharedPointer<const Unit> > units = Unit::getAllUnits(*i);

      for (QVector<QSharedPointer<const Unit> >::const_iterator i = units.begin();
           i != units.end(); ++i)
      {
        unitsToShow.insert((*i)->getName(), *i);
      }
    }

    for (QMap<QString, QSharedPointer<const Unit> >::iterator i = unitsToShow.begin();
         i != unitsToShow.end(); ++i)
    {
      cbUnit->addItem(i.value()->getNameAndAbbreviation(),
                      i.value()->getAbbreviation());
    }

    cbUnit->setCurrentIndex(cbUnit->findData(foodAmount.getUnit()->getAbbreviation()));

    chkIncludeRefuse->setText("Including inedible parts");
    chkIncludeRefuse->setChecked
      (foodAmount.includesRefuse() && foodAmount.getFood()->getPercentRefuse() > 0);
    chkIncludeRefuse->setEnabled(foodAmount.getFood()->getPercentRefuse() > 0);
}

FoodAmount TemplateComponents::ComponentWidgetGroup::getFoodAmount() const
{
  return FoodAmount(food, txtAmount->text().toDouble(),
                     Unit::getUnit(cbUnit->itemData(cbUnit->currentIndex()).toString()),
                     !chkIncludeRefuse->isEnabled() || chkIncludeRefuse->isChecked());
}
