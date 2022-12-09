#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QCryptographicHash>
#include <QCloseEvent>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QTabWidget>
#include <QVariant>

enum UserType
{
  Client,
  Operator,
  Director
};

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
  Q_OBJECT

signals:
  void open_registration();
  void loginSuccessful();
  void loginFailed();

  void login_client();
  void login_operator();
  void login_director();

  void closed();

protected:
  void closeEvent(QCloseEvent * event)
  {
      QMainWindow::closeEvent(event);

      if (event->isAccepted())
          emit closed();
  }

public:
  explicit Login(QWidget *parent = nullptr);
  size_t password_valid(QString login, QString password);



  ~Login();

  UserType get_type() const;
  void init_user(size_t credentials_id);

  size_t get_user_id() const;
  void set_user_id(size_t new_client_id);

private slots:
  void on_pushButton_clicked();

  void on_pushButton_4_clicked();

private:
  Ui::Login *ui;
  QSqlDatabase db;

  UserType type;
  size_t user_id;
};

#endif // LOGIN_H
