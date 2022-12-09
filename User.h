#ifndef USER_H
#define USER_H

#include <QDate>

#include "Login.h"
#include "EditCredWindow.h"

class User
{
public:
  User(size_t usr_id, UserType type_of_uesr);

  QString get_name() const;
  void set_name(const QString& new_name);

  QString get_email() const;
  void set_email(const QString& new_email);

  QString get_phone_number() const;
  void set_phone_number(QString new_phone_number);

  QDate get_birthdate() const;
  void set_birthdate(const QDate& new_birthdate);

  size_t get_credentials_id() const;

  size_t get_user_id() const;

private:
  size_t user_id;
  QString name;
  QString email;
  QString phone_number;
  QDate birthdate;
  size_t credentials_id;
  //bool is_active;
  //QString login; -- Надо ли?

};

#endif // USER_H
