#include "instantiate_template.h"

InstantiateTemplate::InstantiateTemplate
  (const QSharedPointer<const Template>& templ, QWidget *parent)
    : QDialog(parent), tcComponents(new TemplateComponents(templ, this)),
      created(false)
{
	ui.setupUi(this);

	ui.txtName->setText(templ->getName());

	ui.txtAmount->setText("1");

	QVector<QSharedPointer<const Unit> > allUnits = Unit::getAllUnits();

	for (QVector<QSharedPointer<const Unit> >::const_iterator i = allUnits.begin();
	     i != allUnits.end(); ++i)
	{
	  ui.cbAmountUnit->addItem((*i)->getNameAndAbbreviation(), (*i)->getAbbreviation());
	}

	ui.cbAmountUnit->setCurrentIndex
      (ui.cbAmountUnit->findData
         (Unit::getPreferredUnit(Unit::Dimensions::Serving)->getAbbreviation()));

	ui.saComponents->setWidget(tcComponents);

	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btnCreate, SIGNAL(clicked()), this, SLOT(create()));
}

InstantiateTemplate::~InstantiateTemplate()
{
}

FoodAmount InstantiateTemplate::getInstanceAmount() const
{
  if (created) {
    return FoodAmount(tcComponents->getCollection(),
                      ui.txtAmount->text().toDouble(),
                      Unit::getUnit(ui.cbAmountUnit->itemData
                                     (ui.cbAmountUnit->currentIndex()).toString()));
  } else {
    return FoodAmount();
  }
}

void InstantiateTemplate::create()
{
  created = true;
  close();
}
