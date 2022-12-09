#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

enum UserType
{
  Client,
  Operator,
  Director
};

class Validator
{
public:
  Validator();

  void foo();
  UserType get_type_of_user();
  bool password_valid(QString login, QString password);


private:
  //int a = 5;
};

#endif // VALIDATOR_H
