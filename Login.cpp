#include "Login.h"
#include "ui_Login.h"

Login::Login(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Login)
{
  ui->setupUi(this);

  db = QSqlDatabase::addDatabase("QODBC");
  db.setDatabaseName("DRIVER={SQL Server};SERVER=LEGIONWIN;DATABASE=Bookstore;");
  db.setUserName("sa");
  db.setPassword("sa");

  if (!db.open())
    qDebug() << "Err =(";
  else
    qDebug() << "Suc =)";
}

Login::~Login()
{
  delete ui;
}

void Login::on_pushButton_clicked()
{
  QString login = ui->login->text();
  QString password = ui->password->text();

  QByteArray ba = password.toUtf8();
  QString password_with_hash = QCryptographicHash::hash(ba, QCryptographicHash::Blake2b_512).toHex();

  size_t cred_id = password_valid(login, password_with_hash);

  if (cred_id > 0)
  {
    init_user(cred_id);
    qDebug() << cred_id;
    qDebug() << type;
    qDebug() << user_id;
    if (type == UserType::Client)
    {
      emit login_client();
    }
    else if (type == UserType::Operator)
    {
      emit login_operator();
    }
    else if (type == UserType::Director)
    {
      emit login_director();
    }
  }
  //else
  //{
  //  emit loginFailed();
  //}
}


void Login::on_pushButton_4_clicked()
{
  emit open_registration();
}

size_t Login::get_user_id() const
{
  return user_id;
}

void Login::set_user_id(size_t new_client_id)
{
  user_id = new_client_id;
}

UserType Login::get_type() const
{
  return type;
}

void Login::init_user(size_t credentials_id)
{
  QSqlQuery query;
  query.prepare("SELECT ClientId FROM Client WHERE CredId=:cred_id");
  query.bindValue(":cred_id", credentials_id);
  query.exec();
  query.next();

  if (query.isValid())
  {
    type = UserType::Client;
    set_user_id(query.value(0).toULongLong());
    return;
  }

  query.prepare("SELECT OperatorId FROM Operator WHERE CredId=:cred_id");
  query.bindValue(":cred_id", credentials_id);
  query.exec();
  query.next();

  if (query.isValid())
  {
    type = UserType::Operator;
    set_user_id(query.value(0).toULongLong());
    return;
  }

  query.prepare("SELECT DirectorId FROM Director WHERE CredId=:cred_id");
  query.bindValue(":cred_id", credentials_id);
  query.exec();
  query.next();

  if (query.isValid())
  {
    type = UserType::Director;
    set_user_id(query.value(0).toULongLong());
    return;
  }
}

size_t Login::password_valid(QString login, QString password)
{
  QSqlQuery query;

  query.prepare("SELECT CredentialsId FROM Credentials WHERE Login LIKE :login AND Password LIKE :password");
  query.bindValue(":login", login);
  query.bindValue(":password", password);
  query.exec();
  query.next();

  if (query.isValid())
    return query.value(0).toULongLong();

  //qDebug() << "Incorrect login or password";
  ui->statusbar->showMessage("Некорректный логин или пароль");
  return 0;
}

