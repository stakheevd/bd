#include "DeleteAccout.h"
#include "ui_DeleteAccout.h"

DeleteAccout::DeleteAccout(User* usr, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DeleteAccout),
  user(usr)
{
  ui->setupUi(this);
  ui->err_mes->setVisible(false);
}

DeleteAccout::~DeleteAccout()
{
  delete ui;
}

void DeleteAccout::on_buttonBox_accepted()
{
  QSqlQuery query;

  QByteArray ba = ui->password_del->text().toUtf8();
  QString password_with_hash = QCryptographicHash::hash(ba, QCryptographicHash::Blake2b_512).toHex();

  query.prepare("SELECT CredentialsId FROM Credentials WHERE Login LIKE :login AND Password LIKE :password");
  query.bindValue(":login", ui->login_del->text());
  query.bindValue(":password", password_with_hash);
  query.exec();
  query.next();

  if (query.value(0).toULongLong() == user->get_credentials_id())
  {
    //qDebug() << "Sucs del";
    emit delete_account();
  }
  else
  {
    qDebug() << "Incorrect login or pass";
    ui->err_mes->setVisible(true);
    ui->err_mes->setText("Неверный логин или пароль");
  }
}

