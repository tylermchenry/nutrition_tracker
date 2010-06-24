#include "select_template.h"

SelectTemplate::SelectTemplate(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	ui.btnSelect->setEnabled(false);

	// TODO: Real user ID
	QMultiMap<QString, int> userTemplates = Template::getFoodsForUser(1);

	for (QMultiMap<QString, int>::iterator i = userTemplates.begin();
         i != userTemplates.end(); ++i)
	{
	  rowToTemplateId[ui.lstTemplates->count()] = i.value();
	  ui.lstTemplates->addItem(i.key());
	}

	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btnSelect, SIGNAL(clicked()), this, SLOT(select()));
	connect(ui.lstTemplates, SIGNAL(currentRowChanged(int)), this, SLOT(selectionChanged(int)));
}

SelectTemplate::~SelectTemplate()
{
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
