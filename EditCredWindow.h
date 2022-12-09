#ifndef EDITCREDWINDOW_H
#define EDITCREDWINDOW_H

#include <QDialog>
#include <QRegularExpressionValidator>
#include <QSqlError>
#include "Login.h"

namespace Ui {
class EditCredWindow;
}

class EditCredWindow : public QDialog
{
  Q_OBJECT

signals:
  void information_changed();

public:
  explicit EditCredWindow(size_t usr_id, UserType type_of_user, QWidget *parent = nullptr);
  ~EditCredWindow();

private slots:
  void on_edit_save_button_clicked();

private:
  Ui::EditCredWindow *ui;

  size_t user_id;
  UserType type;
};

#endif // EDITCREDWINDOW_H
