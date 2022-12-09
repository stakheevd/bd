#include "Order.h"
#include "ui_Order.h"

Order::Order(QString ord_id, QString stat, UserType type, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Order),
  order_id(ord_id.toULongLong()),
  type_of_user(type),
  status(stat)
{
  ui->setupUi(this);

  ui->err_mes->setVisible(false);

  if ((status == "Завершён") || (status == "Отменён"))
  {
    ui->order_buttons_box->setVisible(false);
  }

  if (type == UserType::Client)
  {
    ui->next_stage_button->setVisible(false);
  }
  else if (type == UserType::Operator)
  {
    if (stat == "Заказ создан")
    {
      ui->next_stage_button->setText("Передать в доставку");
    }
    else if (stat == "Передан в доставку")
    {
      ui->next_stage_button->setText("Выдать заказ");
    }
    else if (stat == "Готов к выдаче")
    {
      ui->next_stage_button->setText("Завершить заказ");
    }
    else if (stat == "Клиент отменил заказ")
    {
      ui->next_stage_button->setVisible(false);
    }
    else
    {
      ui->next_stage_button->setText("Выдать заказ");
    }
  }

  fill_details();
  fill_statuses();

}

Order::~Order()
{
  delete ui;
}

void Order::fill_details()
{
  QSqlQuery query;
  ui->details_of_order_table->clearContents();

  query.prepare("SELECT Title, Author.FIO, Genre, OrderBook.Amount, YearPublication, Annotation "
                "FROM Book LEFT JOIN Author ON AuthorId=AuthId "
                "JOIN Genre On GenreId=GenId "
                "JOIN OrderBook ON BookId=BkId "
                "JOIN Orders ON OrderId=OrdId "
                "WHERE OrderId=:order_id");
  query.bindValue(":order_id", order_id);
  query.exec();

  size_t index = 0;
  ui->details_of_order_table->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->details_of_order_table->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->details_of_order_table->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->details_of_order_table->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->details_of_order_table->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
    ui->details_of_order_table->setItem(index, 4, new QTableWidgetItem(query.value(4).toString()));
    ui->details_of_order_table->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));

    index++;
  }

  ui->details_of_order_table->resizeColumnsToContents();
}

void Order::fill_statuses()
{
  QSqlQuery query;
  ui->statuses_of_order_table->clearContents();

  query.prepare("SELECT StatusName, OrderStatus.Date FROM OrderStatus JOIN Status ON StatusId=StatId "
                "WHERE OrdId=:order_id "
                "ORDER BY StatId DESC");
  query.bindValue(":order_id", order_id);
  query.exec();

  size_t index = 0;
  ui->statuses_of_order_table->setRowCount(query.numRowsAffected());

  while (query.next())
  {
    ui->statuses_of_order_table->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->statuses_of_order_table->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));

    index++;
  }

  ui->statuses_of_order_table->resizeColumnsToContents();
}

void Order::on_cancel_order_button_clicked()
{
  if (type_of_user == UserType::Client)
  {
    QSqlQuery query;

    query.prepare("INSERT INTO OrderStatus (OrdId, StatId, Date) "
                  "VALUES (:order_id, :stat_id, :date)");
    query.bindValue(":order_id", order_id);
    query.bindValue(":stat_id", 3);
    query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));

    if (query.exec())
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Статус заказа изменён");
    }
    else
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Не удалось изменить статус заказа");
    }

    this->close();
  }
  else if (type_of_user == UserType::Operator)
  {
    QSqlQuery query;

    query.prepare("INSERT INTO OrderStatus (OrdId, StatId, Date) "
                  "VALUES (:order_id, :stat_id, :date)");
    query.bindValue(":order_id", order_id);
    query.bindValue(":stat_id", 4);
    query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));

    if (query.exec())
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Статус заказа изменён");
    }
    else
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Не удалось изменить статус заказа");
    }

    this->close();

  }
  //emit order_cancelled();
}


void Order::on_next_stage_button_clicked()
{
  if (status == "Заказ создан")
  {
    QSqlQuery query;

    query.prepare("INSERT INTO OrderStatus (OrdId, StatId, Date) "
                  "VALUES (:order_id, :stat_id, :date)");
    query.bindValue(":order_id", order_id);
    query.bindValue(":stat_id", 1005);
    query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));

    if (query.exec())
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Статус заказа изменён");
    }
    else
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Не удалось изменить статус заказа");
    }
      //qDebug() << "Perfff";
    //else
      //qDebug() << "Nooooo";

    this->close();
  }
  else if (status == "Передан в доставку")
  {
    QSqlQuery query;

    query.prepare("INSERT INTO OrderStatus (OrdId, StatId, Date) "
                  "VALUES (:order_id, :stat_id, :date)");
    query.bindValue(":order_id", order_id);
    query.bindValue(":stat_id", 2);
    query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));

    if (query.exec())
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Статус заказа изменён");
    }
    else
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Не удалось изменить статус заказа");
    }

    this->close();
  }
  else if (status == "Готов к выдаче")
  {
    QSqlQuery query;

    query.prepare("INSERT INTO OrderStatus (OrdId, StatId, Date) "
                  "VALUES (:order_id, :stat_id, :date)");
    query.bindValue(":order_id", order_id);
    query.bindValue(":stat_id", 5);
    query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));

    if (query.exec())
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Статус заказа изменён");
    }
    else
    {
      ui->err_mes->setVisible(true);
      ui->err_mes->setText("Не удалось изменить статус заказа");
    }

    this->close();

  }
}

