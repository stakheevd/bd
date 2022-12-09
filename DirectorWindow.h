#ifndef DIRECTORWINDOW_H
#define DIRECTORWINDOW_H

#include "User.h"
#include "Registration.h"

namespace Ui {
class DirectorWindow;
}

class DirectorWindow : public QMainWindow
{
  Q_OBJECT

signals:
  void closed();
  void operator_deleted();
  void description_changed();
  void status_created();
  void no_change_data();
  void data_changed();

protected:
  void closeEvent(QCloseEvent * event)
  {
      QMainWindow::closeEvent(event);

      if (event->isAccepted())
          emit closed();
  }

public:
  explicit DirectorWindow(size_t dir_id, UserType type_of_user, QWidget *parent = nullptr);
  ~DirectorWindow();

private slots:
  void on_edit_cred_dir_button_clicked();

  void on_reg_oper_button_clicked();

  void on_pushButton_clicked();

  void on_only_cancelled_cb_stateChanged(int arg1);

  void on_save_delete_oper_clicked();

  void on_delete_operator_button_clicked();

  void on_edit_status_button_clicked();

  void on_save_new_diff_status_clicked();

  void on_statuses_cb_currentTextChanged(const QString &arg1);

  void on_add_new_status_button_clicked();

  void on_save_new_status_button_clicked();

  void on_edit_data_button_clicked();

  void on_edit_save_button_clicked();

  void on_operator_emails_cb_currentTextChanged(const QString &arg1);

private:
  Ui::DirectorWindow *ui;
  Registration* reg_operator_window;
  User* user;

  QString name;
  QString phone;
  QString email;

  void fill_operators_table();
  void fill_books_table();
  void fill_orders_table();
  void fill_statuses_table();
  void fill_operators_list();
  void fill_statuses_list();

  void print_most_popular_book();
  void print_most_popular_author();
  void print_most_popular_genre();
  void print_most_profit_clients();
  void print_most_money();
};

#endif // DIRECTORWINDOW_H
