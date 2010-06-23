#ifndef EDIT_COMPOSITE_FOOD_H
#define EDIT_COMPOSITE_FOOD_H

#include <QtGui/QDialog>
#include "ui_edit_composite_food.h"
#include "data/composite_food.h"

class EditCompositeFood : public QDialog
{
    Q_OBJECT

  public:

    static const double MAX_ENTRY;
    static const int MAX_DECIMALS;

    explicit EditCompositeFood(QWidget *parent = 0,
                                   const QSharedPointer<CompositeFood>& food = QSharedPointer<CompositeFood>());

    ~EditCompositeFood();

  public slots:

    void clearFood();

    bool saveFood();

  private:

    Ui::EditCompositeFoodUI ui;

    QSharedPointer<CompositeFood> food;

    // TODO: These methods are largely in common with the EditFood class;
    // refactor them so into a common base or utility class.

    void populateUserSelector(QComboBox* cboOwner);

    void populateUnitSelector(QComboBox* cboUnit, Unit::Dimensions::Dimension dimension);

    void loadAmountInformation(QLineEdit* txtAmount, QComboBox* cboUnit,
                                   Unit::Dimensions::Dimension dimension);

    void loadFoodInformation();

  private slots:

    void setExpiryMinimumDate(const QDate& date);
    void saveFoodAndClose();
    void saveFoodAndClear();
};

#endif // EDIT_COMPOSITE_FOOD_H
