#ifndef INSTANTIATE_TEMPLATE_H
#define INSTANTIATE_TEMPLATE_H

#include <QtGui/QDialog>
#include "ui_instantiate_template.h"
#include "widgets/template_components.h"
#include "libnutrition/data/template.h"

class InstantiateTemplate : public QDialog
{
    Q_OBJECT

  public:

    InstantiateTemplate(const QSharedPointer<const Template>& templ,
                           bool promptForMeal = false,
                           QWidget *parent = 0);

    ~InstantiateTemplate();

    FoodAmount getInstanceAmount() const;

    int getSelectedMealId() const;

  private slots:

    void create();

  private:

    Ui::InstantiateTemplateUI ui;

    TemplateComponents* tcComponents;
    bool promptForMeal;
    bool created;
};

#endif // INSTANTIATE_TEMPLATE_H
