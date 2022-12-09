#ifndef DELETEACCOUT_H
#define DELETEACCOUT_H

#include <QDialog>
#include "User.h"

namespace Ui {
class DeleteAccout;
}

class DeleteAccout : public QDialog
{
  Q_OBJECT

signals:
  void delete_account();
  void reject_delete();

public:
  explicit DeleteAccout(User* usr, QWidget *parent = nullptr);
  ~DeleteAccout();

private slots:
  void on_buttonBox_accepted();

private:
  Ui::DeleteAccout *ui;
  User* user;
};

#endif // DELETEACCOUT_H
