#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "User.h"
#include "Order.h"
#include "CreateOrder.h"
#include "DeleteAccout.h"

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
  Q_OBJECT

signals:
  void closed();
  void genre_set();
  void author_set();

protected:
  void closeEvent(QCloseEvent * event)
  {
      QMainWindow::closeEvent(event);

      if (event->isAccepted())
          emit closed();
  }

public:
  explicit ClientWindow(size_t clnt_id, UserType type_of_user, QWidget *parent = nullptr);
  ~ClientWindow();

private slots:
  void on_change_password_clnt_button_clicked();

  void on_save_password_clnt_button_clicked();

  void on_edit_cred_clnt_button_clicked();

  void on_genre_combo_box_currentTextChanged();

  void on_authors_combo_box_currentTextChanged();

  void on_sort_by_pop_button_clicked();

  void on_find_by_price_clicked();

  void on_clear_filters_button_clicked();

  void on_find_by_year_button_clicked();

  void on_orders_table_widget_cellDoubleClicked(int row, int column);

  void on_create_order_button_clicked();

  void on_processed_orders_button_clicked();

  void on_all_orders_button_clicked();

  void on_deactivate_account_button_clicked();

private:
  Ui::ClientWindow *ui;
  User* user;
  //Order* order;
  //Order* order;
  CreateOrder* create_order_window;
  DeleteAccout* delete_window;

  void delete_account();

  void fill_books_table(QString author = "Любой", QString genre = "Любой");
  void fill_genre_list();
  void fill_author_list();
  void fill_orders_table();
};

#endif // CLIENTWINDOW_H
