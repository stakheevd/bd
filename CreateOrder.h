#ifndef CREATEORDER_H
#define CREATEORDER_H

#include <QDialog>
#include <QCheckBox>
#include <QSpinBox>
#include <QVector>
#include "User.h"

namespace Ui {
class CreateOrder;
}

class CreateOrder : public QDialog
{
  Q_OBJECT

signals:
  void order_created();
  void close_window();

public:
  explicit CreateOrder(size_t usr_id, QWidget *parent = nullptr);
  ~CreateOrder();

private slots:
  void on_back_to_main_button_clicked();

  void on_make_order_button_clicked();

private:
  Ui::CreateOrder *ui;
  size_t user_id;
  QVector<size_t> books_id;

  void fill_create_order_table();
};

#endif // CREATEORDER_H
