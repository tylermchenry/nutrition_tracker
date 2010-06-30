/*
 * variant_value_item_model.h
 * Part of nutrition_tracker
 *
 *  Created on: May 19, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef VARIANT_VALUE_ITEM_MODEL_H_
#define VARIANT_VALUE_ITEM_MODEL_H_

#include <QtGui/QStandardItemModel>

template<typename T>
class VariantValueItemModel : public QStandardItemModel
{
  public:

    explicit VariantValueItemModel(QObject* parent = 0);

    VariantValueItemModel(int rows, int columns, QObject* parent = 0);

    virtual QModelIndexList match
      (const QModelIndex& start, int role, const QVariant& value, int hits,
       Qt::MatchFlags flags) const;
};

template<typename T>
  VariantValueItemModel<T>::VariantValueItemModel(QObject* parent)
    : QStandardItemModel(parent)
{
}

template<typename T>
  VariantValueItemModel<T>::VariantValueItemModel(int rows, int columns, QObject* parent)
    : QStandardItemModel(rows, columns, parent)
{
}

template<typename T>
 QModelIndexList VariantValueItemModel<T>::match
   (const QModelIndex& start, int role, const QVariant& value, int hits,
    Qt::MatchFlags flags) const
{
  // Code taken from Qt's own implementation of QAbstractItemModel::match
  // If the value being search for is not convertible to T, or if the match
  // is not supposed to be exact, then fall back to parent's implementation.
  // Otherwise, convert the needle and each item in the haystack to T before
  // doing a value comparison, to avoid problems with QVariant comparisons when
  // the variant holds a nonstandard type.

  QModelIndexList result;
  uint matchType = flags & 0x0F;
  bool recurse = flags & Qt::MatchRecursive;
  bool wrap = flags & Qt::MatchWrap;
  bool allHits = (hits == -1);
  QString text; // only convert to a string if it is needed
  QModelIndex p = parent(start);
  int from = start.row();
  int to = rowCount(p);

  if (matchType != Qt::MatchExactly || !value.canConvert<T>()) {
    return QStandardItemModel::match(start, role, value, hits, flags);
  }

  T tValue = value.value<T>();

  // iterates twice if wrapping
  for (int i = 0; (wrap && i < 2) || (!wrap && i < 1); ++i) {
      for (int r = from; (r < to) && (allHits || result.count() < hits); ++r) {
          QModelIndex idx = index(r, start.column(), p);
          QVariant v = data(idx, role);

          if (v.canConvert<T>() && v.value<T>() == tValue) {
            result.append(idx);
          }

          if (recurse && hasChildren(idx)) { // search the hierarchy
              result += match(index(0, idx.column(), idx), role,
                              (text.isEmpty() ? value : text),
                              (allHits ? -1 : hits - result.count()), flags);
          }
      }
      // prepare for the next iteration
      from = 0;
      to = start.row();
  }
  return result;
}

#endif /* VARIANT_VALUE_ITEM_MODEL_H_ */
