#include "DirectorWindow.h"
#include "ui_DirectorWindow.h"

DirectorWindow::DirectorWindow(size_t dir_id, UserType type_of_user, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::DirectorWindow),
  reg_operator_window(new Registration(1))
{
  ui->setupUi(this);

  user = new User(dir_id, type_of_user);

  ui->name_dir_value->setText(user->get_name());
  ui->email_dir_value->setText(user->get_email());
  ui->phone_dir_value->setText(user->get_phone_number());

  ui->operator_emails_cb->setVisible(false);
  ui->save_delete_oper->setVisible(false);

  ui->statuses_cb->setVisible(false);
  ui->status_name_label->setVisible(false);
  ui->save_new_diff_status->setVisible(false);
  ui->status_diff_value->setVisible(false);
  ui->status_diff_label->setVisible(false);
  ui->save_new_status_button->setVisible(false);
  ui->new_status_name_value->setVisible(false);

  // Изменение данных оператора

  ui->edit_phone_value->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*")));
  ui->edit_save_button->setVisible(false);
  ui->edit_name_label->setVisible(false);
  ui->edit_name_value->setVisible(false);
  ui->edit_email_label->setVisible(false);
  ui->edit_email_value->setVisible(false);
  ui->edit_phone_value->setVisible(false);
  ui->edit_phone_label->setVisible(false);

  fill_operators_table();
  fill_books_table();
  fill_orders_table();
  fill_statuses_table();

  fill_operators_list();
  fill_statuses_list();

  QObject::connect(&*reg_operator_window, &Registration::operator_created, this, [&]{
    fill_operators_table();
    reg_operator_window->close();
    ui->statusbar->showMessage("Оператор зарегистрирован");
  });

  QObject::connect(this, &DirectorWindow::operator_deleted, this, [&]{
    fill_operators_table();

    ui->statusbar->showMessage("Аккаунт оператора деактивирован");
    ui->operator_emails_cb->setVisible(false);
    ui->save_delete_oper->setVisible(false);
  });

  QObject::connect(this, &DirectorWindow::description_changed, this, [&]{
    fill_statuses_table();
    ui->statusbar->showMessage("Описание статуса изменено");
    ui->status_diff_value->setVisible(false);
    ui->status_diff_label->setVisible(false);
    ui->status_diff_value->clear();
    ui->status_name_label->setVisible(false);
    ui->new_status_name_value->clear();
    ui->new_status_name_value->setVisible(false);
    ui->statuses_cb->setVisible(false);
    ui->save_new_diff_status->setVisible(false);
    ui->save_new_status_button->setVisible(false);
  });

  QObject::connect(this, &DirectorWindow::status_created, this, [&]{
    fill_statuses_list();
    fill_statuses_table();
    ui->statusbar->showMessage("Статус добавлен");
    ui->status_diff_value->setVisible(false);
    ui->status_diff_label->setVisible(false);
    ui->status_diff_value->clear();
    ui->status_name_label->setVisible(false);
    ui->new_status_name_value->clear();
    ui->new_status_name_value->setVisible(false);
    ui->statuses_cb->setVisible(false);
    ui->save_new_diff_status->setVisible(false);
    ui->save_new_status_button->setVisible(false);
  });

  QObject::connect(this, &DirectorWindow::data_changed, this, [&]{
    fill_operators_table();
    fill_operators_list();

    ui->statusbar->showMessage("Данные оператора изменены");
    ui->operator_emails_cb->setVisible(false);
    ui->edit_save_button->setVisible(false);
    ui->edit_name_label->setVisible(false);
    ui->edit_name_value->setVisible(false);
    ui->edit_name_value->clear();
    ui->edit_email_label->setVisible(false);
    ui->edit_email_value->setVisible(false);
    ui->edit_email_value->clear();
    ui->edit_phone_value->setVisible(false);
    ui->edit_phone_label->setVisible(false);
    ui->edit_phone_value->clear();
  });
}

DirectorWindow::~DirectorWindow()
{
  delete ui;
}

void DirectorWindow::on_edit_cred_dir_button_clicked()
{
  EditCredWindow edit_window(user->get_user_id(), UserType::Director);
  edit_window.setModal(true);
  edit_window.exec();
}


void DirectorWindow::on_reg_oper_button_clicked()
{
  reg_operator_window->show();
}

void DirectorWindow::fill_operators_table()
{
  QSqlQuery query;

  query.prepare("SELECT FIO, Email, PhoneNumber FROM Operator");

  query.exec();

  size_t index = 0;

  ui->all_operators_table->clearContents();
  ui->all_operators_table->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->all_operators_table->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->all_operators_table->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->all_operators_table->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));

    index++;
  }

  ui->all_operators_table->resizeColumnsToContents();
}

void DirectorWindow::fill_books_table()
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

void DirectorWindow::fill_orders_table()
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

void DirectorWindow::fill_statuses_table()
{
  QSqlQuery query;

  query.prepare("SELECT StatusName, StatusDescription FROM Status");

  query.exec();

  size_t index = 0;

  ui->all_statuses->clearContents();
  ui->all_statuses->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->all_statuses->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->all_statuses->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));

    index++;
  }

  ui->all_statuses->resizeColumnsToContents();
}

void DirectorWindow::fill_operators_list()
{
  QSqlQuery query;

  query.prepare("SELECT Email FROM Operator");
  query.exec();

  ui->operator_emails_cb->clear();

  while (query.next())
  {
    ui->operator_emails_cb->addItem(query.value(0).toString());
  }
}

void DirectorWindow::fill_statuses_list()
{
  QSqlQuery query;

  query.prepare("SELECT StatusName FROM Status");
  query.exec();

  ui->statuses_cb->clear();

  while (query.next())
  {
    ui->statuses_cb->addItem(query.value(0).toString());
  }
}

void DirectorWindow::print_most_popular_book()
{
  QSqlQuery query;

  query.prepare("SELECT Title, FIO, Genre, YearPublication, Annotation, Price, SumAmount "
                "FROM (SELECT TOP(1) Title, Author.FIO, Genre,  YearPublication, Annotation, Price, SUM(OrderBook.Amount) as SumAmount "
                "FROM Book LEFT JOIN Author ON AuthorId=AuthId "
                "JOIN Genre On GenreId=GenId "
                "JOIN OrderBook ON BookId=BkId "
                "JOIN Orders ON OrderId=OrdId "
                "WHERE Orders.Date >= :start_date AND Orders.Date <= :end_date "
                "GROUP BY BookId, Title, Author.FIO, Genre, YearPublication, Book.Amount, Annotation, Price "
                "ORDER BY SUM(OrderBook.Amount) DESC) as a");
  query.bindValue(":start_date", ui->start_date->date().toString("yyyy-MM-dd"));
  query.bindValue(":end_date", ui->end_date->date().toString("yyyy-MM-dd"));

  query.exec();

  size_t index = 0;
  ui->most_popular_book_table->clearContents();
  ui->most_popular_book_table->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->most_popular_book_table->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->most_popular_book_table->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->most_popular_book_table->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->most_popular_book_table->setItem(index, 3, new QTableWidgetItem(query.value(6).toString()));
    ui->most_popular_book_table->setItem(index, 4, new QTableWidgetItem(query.value(3).toString()));
    ui->most_popular_book_table->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));
    ui->most_popular_book_table->setItem(index, 6, new QTableWidgetItem(query.value(4).toString()));

    index++;
  }
}

void DirectorWindow::print_most_popular_author()
{
  QSqlQuery author_query;

  author_query.prepare("SELECT FIO, BirthDate, DeathDate, SumAmount "
                       "FROM (SELECT TOP(1) Author.FIO, Author.BirthDate, Author.DeathDate, SUM(OrderBook.Amount) as SumAmount "
                       "FROM Book JOIN Author ON AuthorId=AuthId "
                       "JOIN OrderBook ON BookId=BkId "
                       "JOIN Orders ON OrderId=OrdId "
                       "WHERE Orders.Date >= :start_date AND Orders.Date <= :end_date "
                       "GROUP BY FIO, BirthDate, DeathDate "
                       "ORDER BY SUM(OrderBook.Amount) DESC) as a");
  author_query.bindValue(":start_date", ui->start_date->date().toString("yyyy-MM-dd"));
  author_query.bindValue(":end_date", ui->end_date->date().toString("yyyy-MM-dd"));

  author_query.exec();

  size_t index = 0;
  ui->most_popular_author_value->clearContents();
  ui->most_popular_author_value->setRowCount(author_query.numRowsAffected());

  while (author_query.next())
  {
    ui->most_popular_author_value->setItem(index, 0, new QTableWidgetItem(author_query.value(0).toString()));
    ui->most_popular_author_value->setItem(index, 1, new QTableWidgetItem(author_query.value(1).toString()));
    ui->most_popular_author_value->setItem(index, 2, new QTableWidgetItem(author_query.value(2).toString()));
    ui->most_popular_author_value->setItem(index, 3, new QTableWidgetItem(author_query.value(3).toString()));

    index++;
  }
}

void DirectorWindow::print_most_popular_genre()
{
  QSqlQuery query;

  query.prepare("SELECT Genre, SumAmount "
                "FROM (SELECT TOP(1) Genre, SUM(OrderBook.Amount) as SumAmount "
                "FROM Book JOIN Author ON AuthorId=AuthId "
                "JOIN Genre On GenreId=GenId "
                "JOIN OrderBook ON BookId=BkId "
                "JOIN Orders ON OrderId=OrdId "
                "WHERE Orders.Date >= :start_date AND Orders.Date <= :end_date "
                "GROUP BY Genre "
                "ORDER BY SUM(OrderBook.Amount) DESC) as a");
  query.bindValue(":start_date", ui->start_date->date().toString("yyyy-MM-dd"));
  query.bindValue(":end_date", ui->end_date->date().toString("yyyy-MM-dd"));

  query.exec();

  size_t index = 0;
  ui->most_popular_genre_value->clearContents();
  ui->most_popular_genre_value->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->most_popular_genre_value->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->most_popular_genre_value->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));

    index++;
  }

}

void DirectorWindow::print_most_profit_clients()
{
  QSqlQuery query;

  query.prepare("SELECT FIO, PhoneNumber, Email, BirthDate, isActive, TotalPrice "
                "FROM (SELECT TOP(:number) FIO, PhoneNumber, Email, BirthDate, isActive, SUM(Orders.TotalPrice) as TotalPrice "
                "FROM Client JOIN Orders ON ClntId=ClientId "
                "WHERE Orders.Date >= :start_date AND Orders.Date <= :end_date "
                "GROUP BY FIO, PhoneNumber, Email, BirthDate, isActive "
                "ORDER BY SUM(Orders.TotalPrice) DESC) as a");
  query.bindValue(":number", ui->num_of_clients_sb->value());
  query.bindValue(":start_date", ui->start_date->date().toString("yyyy-MM-dd"));
  query.bindValue(":end_date", ui->end_date->date().toString("yyyy-MM-dd"));

  query.exec();

  size_t index = 0;
  ui->most_profit_clients_table->clearContents();
  ui->most_profit_clients_table->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->most_profit_clients_table->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->most_profit_clients_table->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->most_profit_clients_table->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->most_profit_clients_table->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
    if (query.value(4).toString() == "1")
    {
      ui->most_profit_clients_table->setItem(index, 4, new QTableWidgetItem("Активен"));
    }
    else
    {
      ui->most_profit_clients_table->setItem(index, 4, new QTableWidgetItem("Неактивен"));
    }
    ui->most_profit_clients_table->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));

    index++;
  }
}

void DirectorWindow::print_most_money()
{
  QSqlQuery query;

  query.prepare("SELECT SUM(TotalPrice) as SumPrice "
                "FROM Book JOIN OrderBook ON BookId=BkId "
                "JOIN Orders ON OrdId=OrderId "
                "JOIN OrderStatus ON OrderStatus.OrdId=OrderId "
                "JOIN Status ON StatusId=StatId "
                "WHERE StatusName = 'Завершён' AND Orders.Date >= :start_date AND Orders.Date <= :end_date");
  query.bindValue(":start_date", ui->start_date->date().toString("yyyy-MM-dd"));
  query.bindValue(":end_date", ui->end_date->date().toString("yyyy-MM-dd"));

  query.exec();

  ui->money_value->clear();

  query.next();

  if (query.value(0).isNull())
  {
    ui->money_value->setText("За указанный промежуток времени выручки не было");
  }
  else
  {
    ui->money_value->setText(query.value(0).toString());
  }
}


void DirectorWindow::on_pushButton_clicked()
{
  print_most_popular_book();
  print_most_popular_author();
  print_most_popular_genre();
  print_most_profit_clients();
  print_most_money();

}


void DirectorWindow::on_only_cancelled_cb_stateChanged(int arg1)
{
  if (arg1)
  {
    QSqlQuery query;

    query.prepare("SELECT OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName "
                  "FROM Orders JOIN Client ON ClientId=ClntId "
                  "JOIN OrderStatus ON OrdId=OrderId "
                  "JOIN Status ON StatusId=StatId "
                  "WHERE StatId=(SELECT TOP 1 StatId FROM OrderStatus WHERE OrderId=OrdId ORDER BY StatId DESC) "
                  "AND StatusName=:name "
                  "GROUP BY OrderId, FIO, PhoneNumber, Email, TotalPrice, Orders.Date, StatusName, StatId");
    query.bindValue(":name", QVariant("Отменён"));

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
  else
  {
    fill_orders_table();
  }
}


void DirectorWindow::on_save_delete_oper_clicked()
{
  QSqlQuery query;
  query.prepare("DELETE FROM Operator WHERE Email=:email");
  query.bindValue(":email", ui->operator_emails_cb->currentText());

  if (query.exec())
  {
    emit operator_deleted();
  }
  else
  {
    //qDebug() << query.lastError().text();
    ui->statusbar->showMessage("Не удалось изменить пароль");
  }


}


void DirectorWindow::on_delete_operator_button_clicked()
{
  fill_operators_list();
  ui->operator_emails_cb->setVisible(true);
  ui->save_delete_oper->setVisible(true);
}


void DirectorWindow::on_edit_status_button_clicked()
{
  ui->save_new_status_button->setVisible(false);
  ui->new_status_name_value->setVisible(false);

  ui->statuses_cb->setVisible(true);
  ui->status_name_label->setVisible(true);
  ui->save_new_diff_status->setVisible(true);
  ui->status_diff_label->setVisible(true);
  ui->status_diff_value->setVisible(true);

  on_statuses_cb_currentTextChanged(ui->statuses_cb->currentText());
}


void DirectorWindow::on_save_new_diff_status_clicked()
{
  QSqlQuery query;

  query.prepare("UPDATE Status "
                "SET StatusDescription = :stat_desc "
                "WHERE StatusName = :stat_name");
  query.bindValue(":stat_desc", QVariant(ui->status_diff_value->toPlainText()));
  query.bindValue(":stat_name", QVariant(ui->statuses_cb->currentText()));

  if (query.exec())
  {
    emit description_changed();
  }
  else
  {
    ui->statusbar->showMessage("Не удалось изменить пароль");
  }
}


void DirectorWindow::on_statuses_cb_currentTextChanged(const QString &arg1)
{
  QSqlQuery query;

  query.prepare("SELECT StatusDescription FROM Status WHERE StatusName = :name");
  query.bindValue(":name", QVariant(arg1));
  query.exec();

  ui->status_diff_value->clear();

  if (query.next())
  {
    ui->status_diff_value->appendPlainText(query.value(0).toString());
  }
}


void DirectorWindow::on_add_new_status_button_clicked()
{
  ui->statuses_cb->setVisible(false);
  ui->save_new_diff_status->setVisible(false);

  ui->save_new_status_button->setVisible(true);
  ui->status_diff_value->setVisible(true);
  ui->status_diff_label->setVisible(true);
  ui->status_name_label->setVisible(true);
  ui->new_status_name_value->setVisible(true);
  ui->status_diff_value->clear();
}


void DirectorWindow::on_save_new_status_button_clicked()
{
  QSqlQuery query;

  if (ui->new_status_name_value->text().isEmpty())
  {
    ui->statusbar->showMessage("Название статуса не может быть пустым");
    return;
  }

  query.prepare("INSERT INTO Status (StatusName, StatusDescription) "
                "VALUES (:name, :desc)");
  query.bindValue(":name", QVariant(ui->new_status_name_value->text()));
  query.bindValue(":desc", QVariant(ui->status_diff_value->toPlainText()));

  if (query.exec())
  {
    emit status_created();
  }
  else
  {
    ui->statusbar->showMessage("Не удалось изменить пароль");
  }

}


void DirectorWindow::on_edit_data_button_clicked()
{
  ui->operator_emails_cb->setVisible(true);
  ui->edit_save_button->setVisible(true);
  ui->edit_name_label->setVisible(true);
  ui->edit_name_value->setVisible(true);
  ui->edit_name_value->clear();
  ui->edit_email_label->setVisible(true);
  ui->edit_email_value->setVisible(true);
  ui->edit_email_value->clear();
  ui->edit_phone_label->setVisible(true);
  ui->edit_phone_value->setVisible(true);
  ui->edit_phone_value->clear();

  on_operator_emails_cb_currentTextChanged(ui->operator_emails_cb->currentText());
}


void DirectorWindow::on_edit_save_button_clicked()
{
  if (ui->edit_phone_value->text().isEmpty())
  {
    ui->statusbar->showMessage("Номер телефона не может быть пустым");
    return;
  }
  else if (ui->edit_name_value->text().isEmpty())
  {
    ui->statusbar->showMessage("Имя не может быть пустым");
    return;
  }
  else if (ui->edit_email_value->text().isEmpty())
  {
    ui->statusbar->showMessage("Email не может быть пустым");
    return;
  }

  QSqlQuery query;




  if ((ui->edit_name_value->text() == name) && (ui->edit_phone_value->text() == phone) && (ui->edit_email_value->text() == email))
  {
    emit no_change_data();
  }
  else if ((ui->edit_name_value->text() == name) && (ui->edit_phone_value->text() == phone) && (ui->edit_email_value->text() != email))
  {
    // Change email only
    query.prepare("UPDATE Operator SET Email=:email WHERE Email=:old_email");
    query.bindValue(":email", QVariant(ui->edit_email_value->text()));
    query.bindValue(":old_email", QVariant(email));
  }
  else if ((ui->edit_name_value->text() == name) && (ui->edit_phone_value->text() != phone) && (ui->edit_email_value->text() == email))
  {
    // Change phone only
    query.prepare("UPDATE Operator SET PhoneNumber=:phone WHERE Email=:old_email");
    query.bindValue(":phone", QVariant(ui->edit_phone_value->text()));
    query.bindValue(":old_email", QVariant(email));
  }
  else if ((ui->edit_name_value->text() == name) && (ui->edit_phone_value->text() != phone) && (ui->edit_email_value->text() != email))
  {
    // Change phone and email
    query.prepare("UPDATE Operator SET PhoneNumber=:phone, Email=:email WHERE Email=:old_email");
    query.bindValue(":phone", QVariant(ui->edit_phone_value->text()));
    query.bindValue(":email", QVariant(ui->edit_email_value->text()));
    query.bindValue(":old_email", QVariant(email));
  }
  else if ((ui->edit_name_value->text() != name) && (ui->edit_phone_value->text() == phone) && (ui->edit_email_value->text() == email))
  {
    // Change name only
    query.prepare("UPDATE Operator SET FIO=:name WHERE Email=:old_email");
    query.bindValue(":name", QVariant(ui->edit_name_value->text()));
    query.bindValue(":old_email", QVariant(email));
  }
  else if ((ui->edit_name_value->text() != name) && (ui->edit_phone_value->text() == phone) && (ui->edit_email_value->text() != email))
  {
    // Change name and email
    query.prepare("UPDATE Operator SET FIO=:name, Email=:email WHERE Email=:old_email");
    query.bindValue(":name", QVariant(ui->edit_name_value->text()));
    query.bindValue(":email", QVariant(ui->edit_email_value->text()));
    query.bindValue(":old_email", QVariant(email));
  }
  else if ((ui->edit_name_value->text() != name) && (ui->edit_phone_value->text() != phone) && (ui->edit_email_value->text() == email))
  {
    // Change name and phone
    query.prepare("UPDATE Operator SET FIO=:name, PhoneNumber=:phone WHERE Email=:old_email");
    query.bindValue(":name", QVariant(ui->edit_name_value->text()));
    query.bindValue(":phone", QVariant(ui->edit_phone_value->text()));
    query.bindValue(":old_email", QVariant(email));
  }
  else if ((ui->edit_name_value->text() != name) && (ui->edit_phone_value->text() != phone) && (ui->edit_email_value->text() != email))
  {
    // Change all data
    query.prepare("UPDATE Operator SET FIO=:name, PhoneNumber=:phone, Email=:email WHERE Email=:old_email");
    query.bindValue(":name", QVariant(ui->edit_name_value->text()));
    query.bindValue(":phone", QVariant(ui->edit_phone_value->text()));
    query.bindValue(":email", QVariant(ui->edit_email_value->text()));
    query.bindValue(":old_email", QVariant(email));
  }

  if (query.exec())
  {
    //qDebug() << "Success!";
    emit data_changed();
    //emit registration_successful();
  }
  else
  {
    //qDebug() << query.lastError().text();
    ui->statusbar->showMessage("Не удалось изменить данные");
  }
}


void DirectorWindow::on_operator_emails_cb_currentTextChanged(const QString &arg1)
{
  QSqlQuery query;

  query.prepare("SELECT FIO, PhoneNumber, Email FROM Operator WHERE Email = :email");
  query.bindValue(":email", QVariant(arg1));
  query.exec();

  ui->edit_name_value->clear();
  ui->edit_email_value->clear();
  ui->edit_phone_value->clear();

  if (query.next())
  {
    name = query.value(0).toString();
    phone = query.value(1).toString();
    email = query.value(2).toString();

    ui->edit_name_value->setText(name);
    ui->edit_phone_value->setText(phone);
    ui->edit_email_value->setText(email);
  }

}

