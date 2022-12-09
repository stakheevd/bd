#include "ClientWindow.h"
#include "Login.h"
#include "Registration.h"
#include "OperatorWindow.h"
#include "DirectorWindow.h"
#include "ClientWindow.h"

#include <QApplication>
#include <memory>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setQuitOnLastWindowClosed(false);
  // План действий :
  // 1. Сначала выводим окно с авторизацией
  // 2. на основе полученного типа пользователя строим нужное окно
  // TODO: Сделать общий класс для всех? В нём должно создаваться подключение - логин и создаст?
  // TODO: Не создавать лишние окна?
  // Пустая регистрация
  // ОТДЕЛЬНОЕ ОКНО ДЛЯ ИЗМЕНЕИЯ ДАННЫХ, А ПАРОЛЬ МЕНЯЕТСЯ У ВСЕХ В СВОЁМ ОКНЕ
  // TODO: При отказе обновлять данные в основном окне
  Login login;
  Registration reg(0);

  std::unique_ptr<ClientWindow> window_client_ptr;
  std::unique_ptr<OperatorWindow> window_operator_ptr;
  std::unique_ptr<DirectorWindow> window_director_ptr;

  // Если нажали кнопку "Зарегистрироваться"
  //QObject::connect(&login, &Login::open_registration, &login, &QMainWindow::close);
  QObject::connect(&login, &Login::open_registration, &login, &QMainWindow::hide);
  QObject::connect(&login, &Login::open_registration, &reg, &QMainWindow::show);

  // Если регистрация успешная
  // This would both close the login window and open the main one
  QObject::connect(&reg, &Registration::registration_successful, &reg, &QMainWindow::close);
  QObject::connect(&reg, &Registration::registration_successful, &login, &QMainWindow::show);

  // Если регистрация неуспешная
  QObject::connect(&reg, &Registration::registration_failed, &reg, &QMainWindow::close);

  // If login is successful, emit LoginWindow::loginSuccessful
  // This would both close the login window and open the main one
  // Если вошёл клиент
  QObject::connect(&login, &Login::login_client, &login, [&]{
    std::unique_ptr<ClientWindow> clnt_window = std::make_unique<ClientWindow>(login.get_user_id(), login.get_type());
    window_client_ptr = std::move(clnt_window);
    login.hide();
    window_client_ptr->show();
    QObject::connect(&*window_client_ptr, &ClientWindow::closed, &a, &QCoreApplication::quit);
  });

  // Если вошёл оператор
  QObject::connect(&login, &Login::login_operator, &login, [&]{
    std::unique_ptr<OperatorWindow> oper_window = std::make_unique<OperatorWindow>(login.get_user_id(), login.get_type());
    window_operator_ptr = std::move(oper_window);
    login.hide();
    window_operator_ptr->show();
    QObject::connect(&*window_operator_ptr, &OperatorWindow::closed, &a, &QCoreApplication::quit);
  });

  // Если вошёл директор
  QObject::connect(&login, &Login::login_director, &login, [&]{
    std::unique_ptr<DirectorWindow> dir_window = std::make_unique<DirectorWindow>(login.get_user_id(), login.get_type());
    window_director_ptr = std::move(dir_window);
    login.hide();
    window_director_ptr->show();
    QObject::connect(&*window_director_ptr, &DirectorWindow::closed, &a, &QCoreApplication::quit);
  });

  // If login failed, just close the login window
  QObject::connect(&login, &Login::loginFailed, &login, &QMainWindow::close);

  // If login failed, or the main window was closed, quit the application
  // This is needed, because we explicitly set the QGuiApplication::quitOnLastWindowClosed property to false.
  QObject::connect(&login, &Login::closed, &a, &QCoreApplication::quit);
  QObject::connect(&reg, &Registration::closed, &login, &QMainWindow::show);

  login.show();
  return QApplication::exec();

  //return a.exec();
}
