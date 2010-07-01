#include "select_template.h"
#include "libnutrition/data/user.h"
#include <QSettings>

SelectTemplate::SelectTemplate(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);

  ui.btnSelect->setEnabled(false);

  QMultiMap<QString, int> userTemplates = Template::getFoodsForUser
    (User::getLoggedInUser()->getId());

  for (QMultiMap<QString, int>::iterator i = userTemplates.begin();
  i != userTemplates.end(); ++i)
  {
    rowToTemplateId[ui.lstTemplates->count()] = i.value();
    ui.lstTemplates->addItem(i.key());
  }

  connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
  connect(ui.btnSelect, SIGNAL(clicked()), this, SLOT(select()));
  connect(ui.lstTemplates, SIGNAL(currentRowChanged(int)), this, SLOT(selectionChanged(int)));

  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("selecttemplate");
  resize(settings.value("size", size()).toSize());
  if (!settings.value("position").isNull()) {
    move(settings.value("position", pos()).toPoint());
  }
  settings.endGroup();
}

SelectTemplate::~SelectTemplate()
{
  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("selecttemplate");
  settings.setValue("size", size());
  settings.setValue("position", pos());
  settings.endGroup();
}

void SelectTemplate::select()
{
  int row = ui.lstTemplates->currentRow();

  if (row >= 0 && rowToTemplateId.contains(row)) {
    selectedTemplate = Template::getTemplate(rowToTemplateId[row]);
    close();
  }
}

void SelectTemplate::selectionChanged(int row)
{
  ui.btnSelect->setEnabled(row >= 0);
}
