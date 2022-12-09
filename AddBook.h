#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>
#include <QVector>
#include "User.h"

namespace Ui {
class AddBook;
}

class AddBook : public QDialog
{
  Q_OBJECT

signals:
  void book_added();

public:
  explicit AddBook(QWidget *parent = nullptr);
  ~AddBook();

private slots:

  void on_exist_author_rb_toggled(bool checked);

  void on_new_author_rb_toggled(bool checked);

  void on_is_alive_checkbox_stateChanged(int arg1);

  void on_add_book_button_clicked();

private:
  Ui::AddBook *ui;
  QVector<size_t> author_ids;

  void fill_genres_cb();
  void fill_authors_cb();
};

#endif // ADDBOOK_H
