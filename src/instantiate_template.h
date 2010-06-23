#ifndef INSTANTIATE_TEMPLATE_H
#define INSTANTIATE_TEMPLATE_H

#include <QtGui/QDialog>
#include "ui_instantiate_template.h"
#include "data/template.h"
#include "template_components.h"

class InstantiateTemplate : public QDialog
{
    Q_OBJECT

  public:

    InstantiateTemplate(const QSharedPointer<const Template>& templ,
                           QWidget *parent = 0);

    ~InstantiateTemplate();

  private:

    Ui::InstantiateTemplateUI ui;

    TemplateComponents* tcComponents;
};

#endif // INSTANTIATE_TEMPLATE_H
