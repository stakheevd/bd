#ifndef ORDER_H
#define ORDER_H

#include <QDialog>
#include <QDate>
#include "Login.h"

namespace Ui {
class Order;
}

class Order : public QDialog
{
  Q_OBJECT

signals:
  void order_cancelled();

public:
  explicit Order(QString ord_id, QString stat, UserType type, QWidget *parent = nullptr);
  ~Order();

private slots:
  void on_cancel_order_button_clicked();

  void on_next_stage_button_clicked();

private:
  Ui::Order *ui;
  size_t order_id;
  UserType type_of_user;
  QString status;

  void fill_details();
  void fill_statuses();
};

#endif // ORDER_H
