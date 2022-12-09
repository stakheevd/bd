#include "OperatorWindow.h"
#include "ui_OperatorWindow.h"

OperatorWindow::OperatorWindow(size_t oper_id, UserType type_of_user, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::OperatorWindow),
  add_book_window(new AddBook())
{
  ui->setupUi(this);

  user = new User(oper_id, type_of_user);

  ui->name_oper_value->setText(user->get_name());
  ui->email_oper_value->setText(user->get_email());
  ui->phone_oper_value->setText(user->get_phone_number());

  ui->change_password_oper_label->setVisible(false);
  ui->change_password_oper_value->setVisible(false);
  ui->save_password_oper_button->setVisible(false);

  ui->name_new_label->setVisible(false);
  ui->name_new_value->setVisible(false);
  ui->birthdate_new_label->setVisible(false);
  ui->birthdate_new_value->setVisible(false);
  ui->deathdate_new_label->setVisible(false);
  ui->deathdate_new_value->setVisible(false);
  ui->save_new_author->setVisible(false);
  ui->cancel_new_genre->setVisible(false);
  ui->is_new_alive_checkbox->setVisible(false);

  ui->genre_new_label->setVisible(false);
  ui->genre_new_value->setVisible(false);
  ui->save_new_genre->setVisible(false);

  // Изменение названия жанра
  ui->edit_genre_cb_label->setVisible(false);
  ui->edit_genre_cb->setVisible(false);
  ui->edit_genre_label->setVisible(false);
  ui->edit_genre_value->setVisible(false);
  ui->edit_genre_save_button->setVisible(false);

  // Изменение данных о книге
  ui->groupBox_2->setVisible(false);
  ui->edit_book_titles_cb->setVisible(false);
  ui->edit_amount_sb->setVisible(false);
  ui->edit_price->setVisible(false);
  ui->edit_book_save_button->setVisible(false);

  ui->statuses_cb->addItem("Любой");
  ui->clients_cb->addItem("Любой");

  fill_books_table();
  fill_authors_table();
  fill_genres_table();
  fill_orders_table();
  fill_statuses_cb();
  fill_clients_cb();
  fill_genre_list();

  QObject::connect(&*add_book_window, &AddBook::book_added, this, [&]{
    add_book_window->close();
    fill_books_table();
    fill_authors_table();
    ui->statusbar->showMessage("Книга добавлена");
    //ui->books_table_widget->horizontalHeaderItem(5)->setText("Цена");
    //ui->books_table_widget->clearContents();
    //fill_books_table(ui->authors_combo_box->currentText(), ui->genre_combo_box->currentText());
  });

  QObject::connect(this, &OperatorWindow::author_changed, this, [&]{
    fill_books_table();
  });

  QObject::connect(this, &OperatorWindow::author_added, this, [&]{
    fill_books_table();
    fill_authors_table();

    ui->name_new_label->setVisible(false);
    ui->name_new_value->setVisible(false);
    ui->birthdate_new_label->setVisible(false);
    ui->birthdate_new_value->setVisible(false);
    ui->deathdate_new_label->setVisible(false);
    ui->deathdate_new_value->setVisible(false);
    ui->save_new_author->setVisible(false);
    ui->is_new_alive_checkbox->setVisible(false);

    ui->statusbar->showMessage("Автор добавлен");
  });

  QObject::connect(this, &OperatorWindow::genre_added, this, [&]{
    ui->genre_new_label->setVisible(false);
    ui->genre_new_value->setVisible(false);
    ui->genre_new_value->clear();
    ui->save_new_genre->setVisible(false);
    ui->cancel_new_genre->setVisible(false);

    fill_genres_table();
    fill_genre_list();
    ui->statusbar->showMessage("Жанр добавлен");
  });

  QObject::connect(this, &OperatorWindow::genre_changed, this, [&]{
    ui->statusbar->showMessage("Название жанра изменено");
    fill_genres_table();
    fill_genre_list();
    fill_books_table();

    ui->edit_genre_cb_label->setVisible(false);
    ui->edit_genre_cb->setVisible(false);
    ui->edit_genre_label->setVisible(false);
    ui->edit_genre_value->setVisible(false);
    ui->edit_genre_value->clear();
    ui->edit_genre_save_button->setVisible(false);
  });

  QObject::connect(this, &OperatorWindow::info_changed, this, [&]{
    fill_books_table();

    ui->groupBox_2->setVisible(false);
    ui->edit_book_titles_cb->setVisible(false);
    ui->edit_amount_sb->setVisible(false);
    ui->edit_price->setVisible(false);
    ui->edit_book_save_button->setVisible(false);

    ui->statusbar->showMessage("Информация о книге изменена");
  });
}

OperatorWindow::~OperatorWindow()
{
  delete ui;
}

void OperatorWindow::on_edit_edit_oper_button_clicked()
{
  EditCredWindow edit_window(user->get_user_id(), UserType::Operator);
  edit_window.setModal(true);
  edit_window.exec();
}


void OperatorWindow::on_change_password_oper_button_clicked()
{
  ui->change_password_oper_label->setVisible(true);
  ui->change_password_oper_value->setVisible(true);
  ui->save_password_oper_button->setVisible(true);
}


void OperatorWindow::on_save_password_oper_button_clicked()
{
  QString password;

  if (ui->change_password_oper_value->text().isEmpty())
  {
    ui->statusbar->showMessage("Некорректный пароль");
    return;
  }
  else
  {
    password = ui->change_password_oper_value->text();
  }

  QByteArray ba = password.toUtf8();
  QString password_with_hash = QCryptographicHash::hash(ba, QCryptographicHash::Blake2b_512).toHex();

  QSqlQuery query;
  query.prepare("UPDATE Credentials SET Password=:pwd WHERE CredentialsId=:cred_id");
  query.bindValue(":pwd", QVariant(password_with_hash));
  query.bindValue(":cred_id", user->get_credentials_id());

  if (query.exec())
  {
    //qDebug() << "Success!";
    ui->statusbar->showMessage("Пароль успешно изменён");

    ui->change_password_oper_label->setVisible(false);
    ui->change_password_oper_value->setVisible(false);
    ui->save_password_oper_button->setVisible(false);
    //emit registration_successful();
  }
  else
  {
    //qDebug() << query.lastError().text();
    ui->statusbar->showMessage("Не удалось изменить пароль");
  }

}


void OperatorWindow::on_add_book_button_clicked()
{
  add_book_window->setModal(true);
  add_book_window->exec();
}

void OperatorWindow::fill_books_table()
{
  QSqlQuery query;

  query.prepare("SELECT Название, Автор, Жанр, Количество, Год_публикации, Цена, Описание FROM BooksInfo");

  query.exec();

  size_t index = 0;
  ui->books_table_widget->clearContents();
  ui->books_table_widget->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->books_table_widget->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->books_table_widget->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->books_table_widget->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->books_table_widget->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
    ui->books_table_widget->setItem(index, 4, new QTableWidgetItem(query.value(4).toString()));
    ui->books_table_widget->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));
    ui->books_table_widget->setItem(index, 6, new QTableWidgetItem(query.value(6).toString()));

    index++;
  }

  ui->books_table_widget->resizeColumnsToContents();
}

void OperatorWindow::fill_authors_table()
{
  QSqlQuery query;

  query.prepare("SELECT FIO, BirthDate, DeathDate FROM Author");

  query.exec();

  size_t index = 0;
  ui->authors_tabel->clearContents();
  ui->authors_tabel->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->authors_tabel->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->authors_tabel->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->authors_tabel->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));

    index++;
  }

  ui->authors_tabel->resizeColumnsToContents();

}

void OperatorWindow::fill_genres_table()
{
  QSqlQuery query;

  query.prepare("SELECT Genre FROM Genre");

  query.exec();

  size_t index = 0;
  ui->genre_table->clearContents();
  ui->genre_table->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->genre_table->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));

    index++;
  }

  ui->genre_table->resizeColumnsToContents();
}

void OperatorWindow::fill_orders_table()
{
  QSqlQuery query;

  query.prepare("SELECT OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName "
                "FROM Orders JOIN Client ON ClientId=ClntId "
                "JOIN OrderStatus ON OrdId=OrderId "
                "JOIN Status ON StatusId=StatId "
                "WHERE StatId=(SELECT TOP(1) StatId FROM OrderStatus WHERE OrderId=OrdId ORDER BY StatId DESC) "
                "GROUP BY OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName, StatId");

  query.exec();

  size_t index = 0;

  ui->all_orders->clearContents();
  ui->all_orders->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->all_orders->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->all_orders->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->all_orders->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->all_orders->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
    ui->all_orders->setItem(index, 4, new QTableWidgetItem(query.value(4).toString()));
    ui->all_orders->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));
    ui->all_orders->setItem(index, 6, new QTableWidgetItem(query.value(6).toString()));

    index++;
  }

  ui->all_orders->resizeColumnsToContents();
}

void OperatorWindow::fill_genre_list()
{
  QSqlQuery query;
  query.prepare("SELECT Genre FROM Genre");
  query.exec();

  while (query.next())
  {
    ui->edit_genre_cb->addItem(query.value(0).toString());
  }
}

void OperatorWindow::fill_title_list()
{
  QSqlQuery query;
  query.prepare("SELECT Title FROM Book");
  query.exec();

  ui->edit_book_titles_cb->clear();

  while (query.next())
  {
    ui->edit_book_titles_cb->addItem(query.value(0).toString());
  }
}

void OperatorWindow::fill_statuses_cb()
{
  QSqlQuery query;
  query.prepare("SELECT StatusName FROM Status");
  query.exec();

  while (query.next())
  {
    ui->statuses_cb->addItem(query.value(0).toString());
  }
}

void OperatorWindow::fill_clients_cb()
{
  QSqlQuery query;
  query.prepare("SELECT Email FROM Client");
  query.exec();

  while (query.next())
  {
    ui->clients_cb->addItem(query.value(0).toString());
  }

}


void OperatorWindow::on_authors_tabel_cellChanged(int row, int column)
{
  QSqlQuery query;

  if (column == 0)
  {
    // Изменить имя
    QString name = ui->authors_tabel->item(row, column)->text();

    if (name.isEmpty())
    {
      ui->statusbar->showMessage("Некорректное имя");
      return;
    }

    query.prepare("UPDATE Author "
                 "SET FIO = :name "
                 "WHERE AuthorId = :author_id");
    query.bindValue(":name", name);
    query.bindValue(":author_id", row + 1);

    if (query.exec())
    {
      //ui->statusbar->showMessage("Имя автора изменено");
      emit author_changed();
    }
    else
    {
      ui->statusbar->showMessage("Неудалось изменить имя автора");
     //qDebug() << query.lastError().text();
    }
    //ui->authors_tabel->item(row, 0)->text();
  }

  if (column == 1)
  {
   // Изменить дату рождения
    //QString birthdate= ui->authors_tabel->item(row, column)->text();

    //if (name.isEmpty())
    //{
    //  ui->statusbar->showMessage("Некорректное имя");
    //  return;
    //}

    //query.prepare("UPDATE Author "
    //             "SET FIO = :name "
    //             "WHERE AuthorId = :author_id");
    //query.bindValue(":name", name);
    //query.bindValue(":author_id", row + 1);

    //if (query.exec())
    //{
    //  //ui->statusbar->showMessage("Имя автора изменено");
    //  emit author_changed();
    //}
    //else
    //{
    // qDebug() << query.lastError().text();
    //}
  }

  if (column == 2)
  {
   // Изменить дату смерти
  }
}


void OperatorWindow::on_add_author_button_clicked()
{
  ui->name_new_label->setVisible(true);
  ui->name_new_value->setVisible(true);
  ui->birthdate_new_label->setVisible(true);
  ui->birthdate_new_value->setVisible(true);
  ui->deathdate_new_label->setVisible(true);
  ui->deathdate_new_value->setVisible(true);
  ui->is_new_alive_checkbox->setVisible(true);
  ui->save_new_author->setVisible(true);
}


void OperatorWindow::on_save_new_author_clicked()
{
  QSqlQuery query;

  QString name = ui->name_new_value->text();

  if (name.isEmpty())
  {
    ui->statusbar->showMessage("Имя не может быть пустым");
    return;
  }

  query.prepare("INSERT INTO Author (FIO, BirthDate, DeathDate) VALUES (:name, :birthdate, :deathdate)");
  query.bindValue(":name", name);
  query.bindValue(":birthdate", ui->birthdate_new_value->date().toString("yyyy-MM-dd"));

  if (ui->is_new_alive_checkbox->isChecked())
  {
    // Добавляем автора без даты смерти
    query.bindValue(":deathdate", QVariant(QVariant::String), QSql::In);
  }
  else
  {
    query.bindValue(":deathdate", ui->deathdate_new_value->date().toString("yyyy-MM-dd"));
  }

  if (query.exec())
  {
    emit author_added();
  }
  else
  {
    ui->statusbar->showMessage("Некорретные данные об авторе");
    //qDebug() << query.lastError().text();
  }

}


void OperatorWindow::on_is_new_alive_checkbox_stateChanged(int arg1)
{
  if (arg1)
  {
    ui->deathdate_new_label->setVisible(false);
    ui->deathdate_new_value->setVisible(false);
  }
  else
  {
    ui->deathdate_new_label->setVisible(true);
    ui->deathdate_new_value->setVisible(true);
  }
}


void OperatorWindow::on_save_new_genre_clicked()
{
  QSqlQuery query;

  QString name = ui->genre_new_value->text();

  if (name.isEmpty())
  {
    ui->statusbar->showMessage("Название жанра не может быть пустым");
    return;
  }

  query.prepare("INSERT INTO Genre (Genre) VALUES (:name)");
  query.bindValue(":name", name);

  if (query.exec())
  {
    emit genre_added();
  }
  else
  {
    //qDebug() << query.lastError().text();
    ui->statusbar->showMessage("Такой жанр уже существует");
  }
}


void OperatorWindow::on_add_genre_button__clicked()
{
  ui->genre_new_label->setVisible(true);
  ui->genre_new_value->setVisible(true);
  ui->save_new_genre->setVisible(true);
  ui->cancel_new_genre->setVisible(true);
}


void OperatorWindow::on_cancel_new_genre_clicked()
{
  ui->genre_new_label->setVisible(false);
  ui->genre_new_value->clear();
  ui->genre_new_value->setVisible(false);
  ui->save_new_genre->setVisible(false);
  ui->cancel_new_genre->setVisible(false);
}


void OperatorWindow::on_statuses_cb_currentTextChanged(const QString &arg1)
{
  //emit status_set();

  if (arg1 == "Любой")
  {
    fill_orders_table();
    return;
  }
  else
  {
    QSqlQuery query;
    query.prepare("SELECT OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName "
                  "FROM Orders JOIN Client ON ClientId=ClntId "
                  "JOIN OrderStatus ON OrdId=OrderId "
                  "JOIN Status ON StatusId=StatId "
                  "WHERE StatId=(SELECT TOP(1) StatId FROM OrderStatus WHERE OrderId=OrdId ORDER BY StatId DESC) "
                  "AND StatusName=:name "
                  "GROUP BY OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName, StatId");
    query.bindValue(":name", arg1);

    query.exec();

    size_t index = 0;

    ui->all_orders->clearContents();
    ui->all_orders->setRowCount(query.numRowsAffected());

    while (query.next())
    {
      ui->all_orders->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
      ui->all_orders->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
      ui->all_orders->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
      ui->all_orders->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
      ui->all_orders->setItem(index, 4, new QTableWidgetItem(query.value(4).toString()));
      ui->all_orders->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));
      ui->all_orders->setItem(index, 6, new QTableWidgetItem(query.value(6).toString()));

      index++;
    }
  }
}


void OperatorWindow::on_checkBox_stateChanged(int arg1)
{
  if (arg1)
  {
    QSqlQuery query;

    query.prepare("SELECT Название, Автор, Жанр, Количество, Год_публикации, Цена, Описание FROM BooksInfo WHERE Количество = 0");

    query.exec();

    size_t index = 0;
    ui->books_table_widget->clearContents();
    ui->books_table_widget->setRowCount(query.numRowsAffected());

    while (query.next())
    {
      ui->books_table_widget->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
      ui->books_table_widget->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
      ui->books_table_widget->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
      ui->books_table_widget->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
      ui->books_table_widget->setItem(index, 4, new QTableWidgetItem(query.value(4).toString()));
      ui->books_table_widget->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));
      ui->books_table_widget->setItem(index, 6, new QTableWidgetItem(query.value(6).toString()));

      index++;
    }
  }
  else
  {
    fill_books_table();
  }
}


void OperatorWindow::on_clients_cb_currentTextChanged(const QString &arg1)
{
  if (arg1 == "Любой")
  {
    fill_books_table();
    return;
  }
  else
  {
    QSqlQuery query;
    query.prepare("SELECT DISTINCT Title, Author.FIO, Genre, Book.Amount, YearPublication, Price, Annotation "
                  "FROM Book LEFT JOIN Author ON Book.AuthId=Author.AuthorId "
                  "JOIN Genre ON GenreId=GenId "
                  "JOIN OrderBook ON BkId=BookId "
                  "JOIN Orders ON OrderId=OrdId "
                  "JOIN Client ON Client.ClientId=Orders.ClntId "
                  "WHERE Email=:email");
    query.bindValue(":email", QVariant(arg1));

    query.exec();

    size_t index = 0;

    ui->books_table_widget->clearContents();
    ui->books_table_widget->setRowCount(query.numRowsAffected());

    while (query.next())
    {
      ui->books_table_widget->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
      ui->books_table_widget->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
      ui->books_table_widget->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
      ui->books_table_widget->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
      ui->books_table_widget->setItem(index, 4, new QTableWidgetItem(query.value(4).toString()));
      ui->books_table_widget->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));
      ui->books_table_widget->setItem(index, 6, new QTableWidgetItem(query.value(6).toString()));

      index++;
    }
  }
}


void OperatorWindow::on_change_squad_clicked()
{
  QSqlQuery query;

  query.prepare("UPDATE Book SET Price = Price * 0.75 "
                "WHERE NOT EXISTS (SELECT DISTINCT BookId FROM OrderBook WHERE BookId=BkId)");

  if (query.exec())
  {
    ui->statusbar->showMessage("Цена книг, которых ни разу не заказывали, уменьшена на 25%");
    fill_books_table();
  }
  else
  {
    ui->statusbar->showMessage("Не удалось изменить цену");
    //qDebug() << query.lastError().text();
  }
}


void OperatorWindow::on_change_perc_button_clicked()
{
  QSqlQuery query;
  double price = ui->percent_value->value();

  query.prepare("UPDATE Book SET Price = Price * :price");
  query.bindValue(":price", price);

  if (query.exec())
  {
    ui->statusbar->showMessage("Цена всех книг изменена на указанный процент");
    fill_books_table();
  }
  else
  {
    //qDebug() << query.lastError().text();
    ui->statusbar->showMessage("Не удалось изменить цену");
  }
}


void OperatorWindow::on_all_orders_cellDoubleClicked(int row, int column)
{
  Order* order = new Order(ui->all_orders->item(row, 0)->text(), ui->all_orders->item(row, 6)->text(), UserType::Operator);
  order->setModal(true);
  order->exec();
}


void OperatorWindow::on_edit_genre_button_clicked()
{
  //here
  ui->edit_genre_cb_label->setVisible(true);
  ui->edit_genre_cb->setVisible(true);
  ui->edit_genre_label->setVisible(true);
  ui->edit_genre_value->setVisible(true);
  ui->edit_genre_value->clear();
  ui->edit_genre_save_button->setVisible(true);
}


void OperatorWindow::on_edit_genre_cb_currentTextChanged(const QString &arg1)
{
  ui->edit_genre_value->setText(arg1);
}


void OperatorWindow::on_edit_genre_save_button_clicked()
{
  if (ui->edit_genre_cb->currentText() == ui->edit_genre_value->text())
  {
    ui->statusbar->showMessage("Такой жанр уже существует");
    return;
  }

  if (ui->edit_genre_value->text().isEmpty())
  {
    ui->statusbar->showMessage("Название жанра не может быть пустым");
    return;
  }

  QSqlQuery query;

  query.prepare("UPDATE Genre SET Genre = :genre WHERE Genre = :old_genre");
  query.bindValue(":genre", ui->edit_genre_value->text());
  query.bindValue(":old_genre", ui->edit_genre_cb->currentText());

  if (query.exec())
  {
    emit genre_changed();
  }
  else
  {
    ui->statusbar->showMessage("Не удалось изменить жанр");
  }
}


void OperatorWindow::on_edit_info_button_clicked()
{
  ui->groupBox_2->setVisible(true);
  ui->edit_book_titles_cb->setVisible(true);
  ui->edit_amount_sb->setVisible(true);
  ui->edit_price->setVisible(true);
  ui->edit_book_save_button->setVisible(true);

  fill_title_list();
}


void OperatorWindow::on_edit_book_titles_cb_currentTextChanged(const QString &arg1)
{
  QSqlQuery query;

  query.prepare("SELECT Название, Автор, Жанр, Количество, Год_публикации, Цена, Описание FROM BooksInfo WHERE Название = :title_cb");
  query.bindValue(":title_cb", QVariant(arg1));

  query.exec();

  ui->edit_amount_sb->clear();
  ui->edit_price->clear();

  if (query.next())
  {
    amount_o = query.value(3).toInt();
    price_o = query.value(5).toDouble();

    ui->edit_amount_sb->setValue(amount_o);
    ui->edit_price->setValue(price_o);
  }
}


void OperatorWindow::on_edit_book_save_button_clicked()
{
  QSqlQuery query;


  if (amount_o == ui->edit_amount_sb->value() && price_o == ui->edit_price->value())
  {
    emit info_not_changed();
  }
  else if (amount_o == ui->edit_amount_sb->value() && price_o != ui->edit_price->value())
  {
    // Change only price
    query.prepare("UPDATE Book SET Price=:price WHERE Title=:title_cb");
    query.bindValue(":price", QVariant(ui->edit_price->value()));
    query.bindValue(":title_cb", QVariant(ui->edit_book_titles_cb->currentText()));
  }
  else if (amount_o != ui->edit_amount_sb->value() && price_o == ui->edit_price->value())
  {
    // Change only amount
    query.prepare("UPDATE Book SET Amount=:amount WHERE Title=:title_cb");
    query.bindValue(":amount", QVariant(ui->edit_amount_sb->value()));
    query.bindValue(":title_cb", QVariant(ui->edit_book_titles_cb->currentText()));
  }
  else if (amount_o != ui->edit_amount_sb->value() && price_o != ui->edit_price->value())
  {
    // Change all data
    query.prepare("UPDATE Book SET Price=:price, Amount=:amount WHERE Title=:title_cb");
    query.bindValue(":price", QVariant(ui->edit_price->value()));
    query.bindValue(":amount", QVariant(ui->edit_amount_sb->value()));
    query.bindValue(":title_cb", QVariant(ui->edit_book_titles_cb->currentText()));
  }

  if (query.exec())
  {
    emit info_changed();
  }
  else
  {
    ui->statusbar->showMessage("Не удалось изменить данные");
  }
}

