#include "ClientWindow.h"
#include "ui_ClientWindow.h"

ClientWindow::ClientWindow(size_t clnt_id, UserType type_of_user, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ClientWindow),
  create_order_window(new CreateOrder(clnt_id))
{
  //client_id = clnt_id;
  //qDebug() << clnt_id;
  ui->setupUi(this);

  ui->change_password_clnt_label->setVisible(false);
  ui->change_password_clnt_value->setVisible(false);
  ui->save_password_clnt_button->setVisible(false);

  user = new User(clnt_id, type_of_user);
  delete_window = new DeleteAccout(user);

  //QString text = query.value(0).toString();
  ui->name_clnt_value->setText(user->get_name());
  ui->email_clnt_value->setText(user->get_email());
  ui->phone_clnt_value->setText(user->get_phone_number());
  ui->bd_clnt_value->setText(user->get_birthdate().toString());

  fill_books_table();
  fill_genre_list();
  fill_author_list();
  fill_orders_table();

 //ui->min_date_value->setDisplayFormat("yyyy-MM-dd"); //ui->name_clnt_value = QString::fr query.value(0);
 //ui->max_date_value->setDisplayFormat("yyyy-MM-dd"); //ui->name_clnt_value = QString::fr query.value(0);

  //if (query.isValid())
  //{
  //  type = UserType::Client;
  //  set_user_id(query.value(0).toULongLong());
  //  return;
  //}
  QObject::connect(this, &ClientWindow::genre_set, this, [&]{
    ui->books_table_widget->horizontalHeaderItem(5)->setText("Цена");
    ui->books_table_widget->clearContents();
    fill_books_table(ui->authors_combo_box->currentText(), ui->genre_combo_box->currentText());
  });

  QObject::connect(this, &ClientWindow::author_set, this, [&]{
    ui->books_table_widget->horizontalHeaderItem(5)->setText("Цена");
    ui->books_table_widget->clearContents();
    fill_books_table(ui->authors_combo_box->currentText(), ui->genre_combo_box->currentText());
  });

  QObject::connect(&*create_order_window, &CreateOrder::order_created, this, [&]{
    ui->books_table_widget->horizontalHeaderItem(5)->setText("Цена");
    ui->books_table_widget->clearContents();
    fill_books_table(ui->authors_combo_box->currentText(), ui->genre_combo_box->currentText());

    fill_orders_table();

    ui->statusbar->showMessage("Заказ создан");
    create_order_window->close();
  });

  QObject::connect(&*delete_window, &DeleteAccout::delete_account, this, [&]{
    delete_account();
    delete_window->close();
    emit closed();
  });

  //QObject::connect(&*order, &Order::order_cancelled, this, [&]{
  //  fill_orders_table();
  //  ui->statusbar->showMessage("Заказ отменён");
  //  order->close();
  //});
}

ClientWindow::~ClientWindow()
{
  qDebug() << "Destructor client_window";
  delete ui;
}

void ClientWindow::on_change_password_clnt_button_clicked()
{
  ui->change_password_clnt_label->setVisible(true);
  ui->change_password_clnt_value->setVisible(true);
  ui->save_password_clnt_button->setVisible(true);


}


void ClientWindow::on_save_password_clnt_button_clicked()
{
  QString password;

  if (ui->change_password_clnt_value->text().isEmpty())
  {
    ui->statusbar->showMessage("Некорректный пароль");
    return;
  }
  else
  {
    password = ui->change_password_clnt_value->text();
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

    ui->change_password_clnt_label->setVisible(false);
    ui->change_password_clnt_value->setVisible(false);
    ui->save_password_clnt_button->setVisible(false);
    //emit registration_successful();
  }
  else
  {
    //qDebug() << query.lastError().text();
    ui->statusbar->showMessage("Не удалось изменить пароль");
  }

}

void ClientWindow::on_edit_cred_clnt_button_clicked()
{
  EditCredWindow edit_window(user->get_user_id(), UserType::Client, this);
  edit_window.setModal(true);
  edit_window.exec();

  QObject::connect(&edit_window, &EditCredWindow::information_changed, this, [&]{
    ui->statusbar->showMessage("Персональные данные успешно изменены");
  });
}

void ClientWindow::fill_books_table(QString author, QString genre)
{
  QSqlQuery query;

  if (((author != "Любой") && (author != "Без автора")) && (genre != "Любой"))
  {
    query.prepare("SELECT Название, Автор, Жанр, Количество, Год_публикации, Цена, Описание FROM BooksInfo WHERE Количество != 0 AND Автор = :author AND Жанр = :genre");
    query.bindValue(":author", QVariant(author));
    query.bindValue(":genre", QVariant(genre));
  }
  //else if ((author == "Без автора") && (genre != "Любой"))
  //{
  //  query.prepare("SELECT Название, Автор, Жанр, Количество, Год_публикации, Цена, Описание FROM BooksInfo WHERE Количество != 0 AND Автор = :author AND Жанр = :genre");
  //  query.bindValue(":author", QVariant(NULL);
  //  query.bindValue(":genre", QVariant(genre));
  //}
  else if ((author != "Любой") && (author != "Без автора"))
  {
    // Выводи книги одного автора
    query.prepare("SELECT Title, Author.FIO, Genre, Book.Amount, YearPublication, Price, Annotation FROM Book "
                  "JOIN Author ON AuthorId=AuthId "
                  "JOIN Genre On GenreId=GenId WHERE FIO= :author AND Book.Amount != 0");
    query.bindValue(":author", QVariant(author));
  }
  //else if (author == "Без автора")
  //{
  //  query.prepare("SELECT Title, Author.FIO, Genre, Book.Amount, YearPublication, Price, Annotation FROM Book "
  //                "JOIN Author ON AuthorId=AuthId "
  //                "JOIN Genre On GenreId=GenId WHERE FIO= :author AND Book.Amount != 0");
  //  query.bindValue(":author", NULL);
  //}
  else if (genre != "Любой")
  {
    // Выводи книги одного жанра
    query.prepare("SELECT Title, Author.FIO, Genre, Book.Amount, YearPublication, Price, Annotation FROM Book "
                  "LEFT JOIN Author ON AuthorId=AuthId "
                  "JOIN Genre On GenreId=GenId WHERE Genre= :genre AND Book.Amount != 0");
    query.bindValue(":genre", QVariant(genre));
  }
  else
  {
    // Выводим все книги
    query.prepare("SELECT Название, Автор, Жанр, Количество, Год_публикации, Цена, Описание FROM BooksInfo WHERE Количество != 0");
  }

  query.exec();

  size_t index = 0;
  ui->books_table_widget->setRowCount(query.numRowsAffected());
  while (query.next())
  {
    //ui->books_table_widget->insertRow(ui->books_table_widget->rowCount());
    //ui->books_table_widget->currentRow().
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

void ClientWindow::fill_genre_list()
{
  QSqlQuery query;

  query.prepare("SELECT Genre FROM Genre");
  query.exec();

  ui->genre_combo_box->addItem("Любой");

  while (query.next())
  {
    ui->genre_combo_box->addItem(query.value(0).toString());
  }

  //emit genre_set();
}

void ClientWindow::fill_author_list()
{
  QSqlQuery query;

  query.prepare("SELECT FIO FROM Author");
  query.exec();

  ui->authors_combo_box->addItem("Любой");
  ui->authors_combo_box->addItem("Без автора");

  while (query.next())
  {
    ui->authors_combo_box->addItem(query.value(0).toString());
  }

}

void ClientWindow::fill_orders_table()
{
  QSqlQuery query;

  query.prepare("SELECT Номер_заказа, Cтоимость, Дата_оформления, Текущий_статус FROM AllOrders WHERE Клиент = :client_id");
  query.bindValue(":client_id", QVariant(user->get_user_id()));
  query.exec();

  size_t index = 0;
  ui->orders_table_widget->clearContents();
  ui->orders_table_widget->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->orders_table_widget->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->orders_table_widget->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->orders_table_widget->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->orders_table_widget->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));

    index++;
  }

  ui->orders_table_widget->resizeColumnsToContents();

}


void ClientWindow::on_genre_combo_box_currentTextChanged()
{
  emit genre_set();
}


void ClientWindow::on_authors_combo_box_currentTextChanged()
{
  emit author_set();
}


void ClientWindow::on_sort_by_pop_button_clicked()
{
  ui->books_table_widget->clearContents();

  QSqlQuery query;

  query.prepare("SELECT Title, Author.FIO, Genre, Book.Amount, YearPublication, Annotation, "
                "SUM(OrderBook.Amount) as Total_ordered "
                "FROM Book LEFT JOIN Author ON AuthorId=AuthId "
                "JOIN Genre On GenreId=GenId "
                "LEFT JOIN OrderBook ON BookId=BkId "
                "WHERE Book.Amount != 0 "
                "GROUP BY BookId, Title, Author.FIO, Genre, Book.YearPublication, Book.Amount, Annotation "
                "ORDER BY SUM(OrderBook.Amount) DESC");

  query.exec();

  size_t index = 0;
  ui->books_table_widget->setRowCount(query.numRowsAffected());
  //ui->books_table_widget->setVerticalHeaderLabels({"Название, Автор, Жанр, Количество, Год публикации, Всего заказано, Описание"});
  ui->books_table_widget->horizontalHeaderItem(5)->setText("Всего заказано");

  while (query.next())
  {
    ui->books_table_widget->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->books_table_widget->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->books_table_widget->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->books_table_widget->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
    ui->books_table_widget->setItem(index, 4, new QTableWidgetItem(query.value(4).toString()));
    ui->books_table_widget->setItem(index, 5, new QTableWidgetItem(query.value(6).toString()));
    ui->books_table_widget->setItem(index, 6, new QTableWidgetItem(query.value(5).toString()));

    index++;
  }
}

void ClientWindow::on_find_by_price_clicked()
{
  ui->books_table_widget->clearContents();

  QSqlQuery query;

  query.prepare("SELECT Title, Author.FIO, Genre, Book.Amount, YearPublication, Price, Annotation "
                "FROM Book JOIN Author ON AuthorId=AuthId "
                "JOIN Genre On GenreId=GenId "
                "WHERE Price >= :min_price AND Price <= :max_price AND Book.Amount != 0");
  query.bindValue(":min_price", QVariant(ui->min_price_value->value()));
  query.bindValue(":max_price", QVariant(ui->max_price_value->value()));

  query.exec();

  size_t index = 0;
  ui->books_table_widget->setRowCount(query.numRowsAffected());
  ui->books_table_widget->horizontalHeaderItem(5)->setText("Цена");

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


void ClientWindow::on_clear_filters_button_clicked()
{
  ui->books_table_widget->horizontalHeaderItem(5)->setText("Цена");

  ui->min_price_value->setValue(0.0);
  ui->max_price_value->setValue(0.0);
  ui->genre_combo_box->setCurrentIndex(0);
  ui->authors_combo_box->setCurrentIndex(0);
  ui->min_date_value->setDate(QDate(2000, 1, 1));
  ui->max_date_value->setDate(QDate(2000, 1, 1));

  ui->books_table_widget->clearContents();
  fill_books_table();
}


void ClientWindow::on_find_by_year_button_clicked()
{
  ui->books_table_widget->clearContents();

  QSqlQuery query;

  query.prepare("SELECT Title, Author.FIO, Genre, Book.YearPublication, Price, Book.Amount, Annotation "
                "FROM Book LEFT JOIN Author ON AuthorId=AuthId "
                "JOIN Genre On GenreId=GenId "
                "WHERE YearPublication >= :min_date AND YearPublication <= :max_date AND Book.Amount != 0");
  query.bindValue(":min_date", QVariant(ui->min_date_value->date().toString("yyyy-MM-dd")));
  query.bindValue(":max_date", QVariant(ui->max_date_value->date().toString("yyyy-MM-dd")));

  query.exec();

  size_t index = 0;
  ui->books_table_widget->setRowCount(query.numRowsAffected());
  ui->books_table_widget->horizontalHeaderItem(5)->setText("Цена");

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


void ClientWindow::on_orders_table_widget_cellDoubleClicked(int row, int column)
{
  //if (ui->orders_table_widget->itemAt(row, 0))
  //{
  Order* order = new Order(ui->orders_table_widget->item(row, 0)->text(), ui->orders_table_widget->item(row, 3)->text(), UserType::Client);
  order->setModal(true);
  order->exec();

  //}
}

void ClientWindow::on_create_order_button_clicked()
{
  create_order_window->setModal(true);
  create_order_window->exec();
}


void ClientWindow::on_processed_orders_button_clicked()
{
  QSqlQuery query;

  query.prepare("SELECT OrderId, TotalPrice, Orders.Date, StatusName "
                "FROM Orders JOIN Client ON ClientId=ClntId "
                "JOIN OrderStatus ON OrdId=OrderId "
                "JOIN Status ON StatusId=StatId "
                "WHERE StatId=(SELECT TOP(1) StatId FROM OrderStatus WHERE OrderId=OrdId ORDER BY StatId DESC) "
                "AND Status.StatusName != 'Завершён' AND Status.StatusName != 'Отменён' AND ClientId = :client_id "
                "GROUP BY OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName, StatId");
  query.bindValue(":client_id", QVariant(user->get_user_id()));
  query.exec();

  size_t index = 0;
  ui->orders_table_widget->clearContents();
  ui->orders_table_widget->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    //ui->books_table_widget->insertRow(ui->books_table_widget->rowCount());
    //ui->books_table_widget->currentRow().
    ui->orders_table_widget->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->orders_table_widget->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->orders_table_widget->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->orders_table_widget->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));

    index++;
  }
   /*SELECT OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName
FROM Orders JOIN Client ON ClientId=ClntId
      JOIN OrderStatus ON OrdId=OrderId
      JOIN Status ON StatusId=StatId
WHERE StatId=(SELECT TOP(1) StatId FROM OrderStatus WHERE OrderId=OrdId ORDER BY StatId DESC)
AND Status.StatusName != 'Завершён' AND Status.StatusName != 'Отменён'
GROUP BY OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName, StatId
*/
}


void ClientWindow::on_all_orders_button_clicked()
{
  fill_orders_table();
}


void ClientWindow::on_deactivate_account_button_clicked()
{
  delete_window->show();
}

void ClientWindow::delete_account()
{
  QSqlQuery query;

  query.prepare("UPDATE Client SET "
                "isActive = 0 WHERE "
                "ClientId=:client_id");
  query.bindValue(":client_id", user->get_user_id());

  if (query.exec())
  {
  }
  else
  {
    ui->statusbar->showMessage("Произошла ошибка во время удаления аккаунта");
  }
}

