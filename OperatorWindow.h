#ifndef OPERATORWINDOW_H
#define OPERATORWINDOW_H

#include "User.h"
#include "AddBook.h"
#include "Order.h"

namespace Ui {
class OperatorWindow;
}

class OperatorWindow : public QMainWindow
{
  Q_OBJECT

signals:
  void closed();
  void author_changed();
  void author_added();
  void genre_added();
  void genre_changed();
  void info_not_changed();
  void info_changed();

protected:
  void closeEvent(QCloseEvent * event)
  {
      QMainWindow::closeEvent(event);

      if (event->isAccepted())
          emit closed();
  }

public:
  explicit OperatorWindow(size_t oper_id, UserType type_of_user, QWidget *parent = nullptr);
  ~OperatorWindow();

private slots:
  void on_edit_edit_oper_button_clicked();

  void on_change_password_oper_button_clicked();

  void on_save_password_oper_button_clicked();

  void on_add_book_button_clicked();

  void on_authors_tabel_cellChanged(int row, int column);

  void on_add_author_button_clicked();

  void on_save_new_author_clicked();

  void on_is_new_alive_checkbox_stateChanged(int arg1);

  void on_save_new_genre_clicked();

  void on_add_genre_button__clicked();

  void on_cancel_new_genre_clicked();

  void on_statuses_cb_currentTextChanged(const QString &arg1);

  void on_checkBox_stateChanged(int arg1);

  void on_clients_cb_currentTextChanged(const QString &arg1);

  void on_change_squad_clicked();

  void on_change_perc_button_clicked();

  void on_all_orders_cellDoubleClicked(int row, int column);

  void on_edit_genre_button_clicked();

  void on_edit_genre_cb_currentTextChanged(const QString &arg1);

  void on_edit_genre_save_button_clicked();

  void on_edit_info_button_clicked();

  void on_edit_book_titles_cb_currentTextChanged(const QString &arg1);

  void on_edit_book_save_button_clicked();

private:
  Ui::OperatorWindow *ui;
  User* user;
  AddBook* add_book_window;

  QString title_o;
  QString author_o;
  QString genre_o;
  int amount_o;
  QString date_o;
  double price_o;
  QString description_o;

  void fill_books_table();
  void fill_authors_table();
  void fill_genres_table();
  void fill_orders_table();

  void fill_genre_list();
  //void fill_genre_book_list();
  //void fill_author_list();
  void fill_title_list();
  void fill_statuses_cb();
  void fill_clients_cb();
};

#endif // OPERATORWINDOW_H
