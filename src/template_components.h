#ifndef TEMPLATE_COMPONENTS_H
#define TEMPLATE_COMPONENTS_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include "ui_template_components.h"
#include "data/template.h"
#include "data/food_collection.h"

class TemplateComponents : public QWidget
{
    Q_OBJECT

  public:

    TemplateComponents(const QSharedPointer<const Template>& templ,
                          QWidget *parent = 0);

    ~TemplateComponents();

    QSharedPointer<const FoodCollection> getCollection() const;

  private:

    Ui::TemplateComponentsUI ui;

    QSharedPointer<const Template> templ;

    class ComponentWidgetGroup {

      public:

        ComponentWidgetGroup
          (FoodAmount foodAmount, TemplateComponents* parent);

        FoodAmount getFoodAmount() const;

      private:

        QSharedPointer<Food> food;
        QLabel* lblFoodName;
        QLineEdit* txtAmount;
        QComboBox* cbUnit;
    };

    QList<ComponentWidgetGroup> componentWidgetGroups;
};

#endif // TEMPLATE_COMPONENTS_H
