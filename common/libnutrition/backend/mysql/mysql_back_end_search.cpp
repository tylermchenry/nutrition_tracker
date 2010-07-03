#include "mysql_back_end.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QList<MySQLBackEnd::SearchResult> MySQLBackEnd::searchFoods
  (const SearchRequest&)
{
  return QList<SearchResult>();
}
