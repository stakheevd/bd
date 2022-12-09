#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMainWindow>
#include <QCryptographicHash>
#include <QCloseEvent>
#include <QRegularExpressionValidator>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace Ui {
class Registration;
}

class Registration : public QMainWindow
{
  Q_OBJECT

signals:
  void registration_successful();
  void operator_created();
  void registration_failed();
  void closed();

protected:
  void closeEvent(QCloseEvent * event)
  {
      QMainWindow::closeEvent(event);

      if (event->isAccepted())
          emit closed();
  }

public:
  explicit Registration(bool root, QWidget *parent = nullptr);
  ~Registration();

private slots:

  void on_create_account_button_clicked();

private:
  Ui::Registration *ui;
  bool is_admin;
};

#endif // REGISTRATION_H
