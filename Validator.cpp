#include "Validator.h"

Validator::Validator()
{
  //qDebug() << a;
}

void Validator::foo()
{
  QSqlQuery query;
  query.exec("SELECT Genre FROM Genre");

  while (query.next())
  {
    QString gen = query.value(0).toString();
    qDebug() << gen << ' ';
  }
}

bool Validator::password_valid(QString login, QString password)
{
}
