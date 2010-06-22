#ifndef EDIT_TEMPLATE_H
#define EDIT_TEMPLATE_H

#include <QtGui/QDialog>
#include "ui_edit_template.h"
#include "data/template.h"

class EditTemplate : public QDialog
{
    Q_OBJECT

  public:

    explicit EditTemplate(QWidget *parent = 0,
                             const QSharedPointer<Template>& food =
                               QSharedPointer<Template>());

    ~EditTemplate();

  public slots:

    void clearFood();

    bool saveFood();

  private:

    Ui::EditTemplateUI ui;

    QSharedPointer<Template> food;

    void populateUserSelector(QComboBox* cboOwner);

    void loadFoodInformation();

  private slots:

    void saveFoodAndClose();
    void saveFoodAndClear();
};

#endif // EDIT_TEMPLATE_H
