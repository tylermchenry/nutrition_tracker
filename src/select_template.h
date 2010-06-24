#ifndef SELECT_TEMPLATE_H
#define SELECT_TEMPLATE_H

#include <QtGui/QDialog>
#include "ui_select_template.h"
#include "data/template.h"
#include <QMap>

class SelectTemplate : public QDialog
{
  Q_OBJECT

  public:

    SelectTemplate(QWidget *parent = 0);

    ~SelectTemplate();

    inline QSharedPointer<const Template> getSelectedTemplate()
      { return selectedTemplate; }

  private slots:

    void select();
    void selectionChanged(int row);

  private:

    Ui::SelectTemplateUI ui;

    QMap<int, int> rowToTemplateId;
    QSharedPointer<const Template> selectedTemplate;
};

#endif // SELECT_TEMPLATE_H
