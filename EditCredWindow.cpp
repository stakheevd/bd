#include "EditCredWindow.h"
#include "ui_EditCredWindow.h"

EditCredWindow::EditCredWindow(size_t usr_id, UserType type_of_user, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditCredWindow),
  user_id(usr_id),
  type(type_of_user)
{
  ui->setupUi(this);
  ui->phone_edit_value->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
  QObject::connect(this, &EditCredWindow::information_changed, this, &QDialog::close);
  //QObject::connect(this, &EditCredWindow::information_changed, this, [&]{  });
}

EditCredWindow::~EditCredWindow()
{
  delete ui;
}

void EditCredWindow::on_edit_save_button_clicked()
{
  //  Добавить валидатор в виде регулярки
  if (!ui->name_edit_value->text().isEmpty())
  {
    // Тут должны быть проверки
    QString new_name = ui->name_edit_value->text();
    QSqlQuery query;
    switch (type) {
      case UserType::Client:
        query.prepare("UPDATE Client SET FIO=:name WHERE ClientId=:clnt_id");
        query.bindValue(":name", QVariant(new_name));
        query.bindValue(":clnt_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      case UserType::Operator:
        query.prepare("UPDATE Operator SET FIO=:name WHERE OperatorId=:oper_id");
        query.bindValue(":name", QVariant(new_name));
        query.bindValue(":oper_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      case UserType::Director:
        query.prepare("UPDATE Director SET FIO=:name WHERE DirectorId=:dir_id");
        query.bindValue(":name", QVariant(new_name));
        query.bindValue(":dir_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      default:
        break;
    }
    // Change name
  }

  if (!ui->email_edit_value->text().isEmpty())
  {
    // Тут должны быть проверки
    QString new_email = ui->email_edit_value->text();
    QSqlQuery query;
    switch (type) {
      case UserType::Client:
        query.prepare("UPDATE Client SET Email=:email WHERE ClientId=:clnt_id");
        query.bindValue(":email", QVariant(new_email));
        query.bindValue(":clnt_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      case UserType::Operator:
        query.prepare("UPDATE Operator SET Email=:email WHERE OperatorId=:oper_id");
        query.bindValue(":email", QVariant(new_email));
        query.bindValue(":oper_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      case UserType::Director:
        query.prepare("UPDATE Director SET Email=:email WHERE DirectorId=:dir_id");
        query.bindValue(":email", QVariant(new_email));
        query.bindValue(":dir_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      default:
        break;
    }
    // Change email
  }

  if (!ui->phone_edit_value->text().isEmpty())
  {
    // Change phone number
    QString new_phone = ui->phone_edit_value->text();
    QSqlQuery query;
    switch (type) {
      case UserType::Client:
        query.prepare("UPDATE Client SET PhoneNumber=:phone_number WHERE ClientId=:clnt_id");
        query.bindValue(":phone_number", QVariant(new_phone));
        query.bindValue(":clnt_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      case UserType::Operator:
        query.prepare("UPDATE Operator SET PhoneNumber=:phone_number WHERE OperatorId=:oper_id");
        query.bindValue(":phone_number", QVariant(new_phone));
        query.bindValue(":oper_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      case UserType::Director:
        query.prepare("UPDATE Director SET PhoneNumber=:phone_number WHERE DirectorId=:dir_id");
        query.bindValue(":phone_number", QVariant(new_phone));
        query.bindValue(":dir_id", QVariant(user_id));

        if (query.exec())
        {
          qDebug() << "Nice =)";
        }
        else
          qDebug() << "Bad =(";
        break;
      default:
        break;
    }
  }


  emit information_changed();
  // ПОСЛЕ ИЗМЕНЕНИЯ ДАННЫХ ЗАКРЫВАЕМ ОКНО И ОБНОВЛЯЕМ ОСНОВНОЕ
}

