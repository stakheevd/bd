#include "Registration.h"
#include "ui_Registration.h"

Registration::Registration(bool root, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Registration),
  is_admin(root)
{
  ui->setupUi(this);

  if (root)
  {
    ui->new_birthdate->setVisible(false);
    ui->reg_birthdate_label->setVisible(false);
  }

  ui->new_phone_number->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
}

Registration::~Registration()
{
  delete ui;
}


  //QSqlQuery query;
  //query.exec("SELECT Genre FROM Genre");

  //while (query.next())
  //{
  //  QString gen = query.value(0).toString();
  //  qDebug() << gen << ' ';
  //}

// TODO: Закрытие регистрации не завершает приложение

void Registration::on_create_account_button_clicked()
{
  QString login;

  if ((ui->new_login->text().isEmpty()) || (ui->new_login->text().length() > 100))
  {
    ui->statusbar->showMessage("Некорректный логин");
    return;
  }
  else
  {
    login = ui->new_login->text();
  }

  QString password;

  if (ui->new_password->text().isEmpty())
  {
    ui->statusbar->showMessage("Некорректный пароль");
    return;
  }
  else
  {
    password = ui->new_password->text();
  }

  QByteArray ba = password.toUtf8();
  QString password_with_hash = QCryptographicHash::hash(ba, QCryptographicHash::Blake2b_512).toHex();

  QString fio;
  if ((ui->new_fio->text().isEmpty()) || (ui->new_fio->text().length() > 100))
  {
    ui->statusbar->showMessage("Некорректное имя");
    return;
  }
  else
  {
    fio = ui->new_fio->text();
  }

  QString email;
  if ((ui->new_email->text().isEmpty()) || (ui->new_email->text().length() > 128))
  {
    ui->statusbar->showMessage("Некорректный адрес электронной почты");
    return;
  }
  else
  {
    email = ui->new_email->text();
  }

  QString phone;
  if (ui->new_phone_number->text().length() != 11)
  {
    ui->statusbar->showMessage("Некорректный номер телефона");
    return;
  }
  else
  {
    phone = ui->new_phone_number->text();
  }

  QSqlQuery query;
  query.setForwardOnly(true);

  if (is_admin)
  {
    query.prepare("{CALL Register_Operator(:login, :password, :fio, :email, :phone)}");
    query.bindValue(":login", login, QSql::In);
    query.bindValue(":password", password_with_hash, QSql::In);
    query.bindValue(":fio", fio, QSql::In);
    query.bindValue(":email", email, QSql::In);
    query.bindValue(":phone", phone, QSql::In);

    if (query.exec())
    {
      qDebug() << "Success oper created!";
      emit operator_created();
    }
    else
    {
      ui->statusbar->showMessage("Пользователь с такими данными уже существует");
    }

  }
  else
  {
    QDate birthdate = ui->new_birthdate->date();

    query.prepare("{CALL Register_Client(:login, :password, :fio, :email, :phone, :birthdate)}");
    query.bindValue(":login", login, QSql::In);
    query.bindValue(":password", password_with_hash, QSql::In);
    query.bindValue(":fio", fio, QSql::In);
    query.bindValue(":email", email, QSql::In);
    query.bindValue(":phone", phone, QSql::In);
    query.bindValue(":birthdate", birthdate.toString("yyyy-MM-dd"), QSql::In);

    if (query.exec())
    {
      qDebug() << "Success!";
      emit registration_successful();
    }
    else
    {
      ui->statusbar->showMessage("Пользователь с такими данными уже существует");
    }

  }
}

