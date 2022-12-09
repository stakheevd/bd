#include "User.h"

User::User(size_t usr_id, UserType type_of_user)
{
  QSqlQuery query;

  if (type_of_user == UserType::Client)
  {
    query.prepare("SELECT FIO, PhoneNumber, Email, CredId, BirthDate FROM Client WHERE ClientId=:client_id");
    query.bindValue(":client_id", usr_id);
  }
  else if (type_of_user == UserType::Operator)
  {
    query.prepare("SELECT FIO, PhoneNumber, Email, CredId FROM Operator WHERE OperatorId=:operator_id");
    query.bindValue(":operator_id", usr_id);
  }
  else if (type_of_user == UserType::Director)
  {
    query.prepare("SELECT FIO, PhoneNumber, Email, CredId FROM Director WHERE DirectorId=:director_id");
    query.bindValue(":director_id", usr_id);
  }

  query.exec();
  query.next();

  user_id = usr_id;
  name = query.value(0).toString();
  phone_number = query.value(1).toString();
  email = query.value(2).toString();
  credentials_id = query.value(3).toInt();

  if (type_of_user == UserType::Client)
    birthdate = query.value(4).toDate();

}

QString User::get_name() const
{
  return name;
}

void User::set_name(const QString& new_name)
{
  name = new_name;
}

QString User::get_email() const
{
  return email;
}

void User::set_email(const QString& newEmail)
{
  email = newEmail;
}

QString User::get_phone_number() const
{
  return phone_number;
}

void User::set_phone_number(QString new_phone_number)
{
  phone_number = new_phone_number;
}

QDate User::get_birthdate() const
{
  return birthdate;
}

void User::set_birthdate(const QDate& new_birthdate)
{
  birthdate = new_birthdate;
}

size_t User::get_credentials_id() const
{
  return credentials_id;
}

size_t User::get_user_id() const
{
  return user_id;
}
