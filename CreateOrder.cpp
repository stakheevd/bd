#include "CreateOrder.h"
#include "ui_CreateOrder.h"

CreateOrder::CreateOrder(size_t usr_id, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CreateOrder),
  user_id(usr_id),
  books_id()
{
  ui->setupUi(this);

  fill_create_order_table();

  QObject::connect(this, &CreateOrder::close_window, this, &QDialog::close);
}

CreateOrder::~CreateOrder()
{
  delete ui;
}

void CreateOrder::fill_create_order_table()
{
  QSqlQuery query;
  ui->create_order_table->clearContents();
  query.prepare("SELECT Название, Автор, Жанр, Количество, Год_публикации, Цена, Описание, ID FROM BooksInfo WHERE Количество != 0");
  query.exec();

  size_t index = 0;
  ui->create_order_table->setRowCount(query.numRowsAffected());
  books_id.reserve(query.numRowsAffected());

  while (query.next())
  {
    ui->create_order_table->setItem(index, 0, new QTableWidgetItem(query.value(0).toString()));
    ui->create_order_table->setItem(index, 1, new QTableWidgetItem(query.value(1).toString()));
    ui->create_order_table->setItem(index, 2, new QTableWidgetItem(query.value(2).toString()));
    ui->create_order_table->setItem(index, 3, new QTableWidgetItem(query.value(3).toString()));
    ui->create_order_table->setItem(index, 4, new QTableWidgetItem(query.value(4).toString()));
    ui->create_order_table->setItem(index, 5, new QTableWidgetItem(query.value(5).toString()));
    ui->create_order_table->setItem(index, 6, new QTableWidgetItem(query.value(6).toString()));
    books_id.push_back(query.value(7).toULongLong());

    QWidget *checkBoxWidget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();      // We declare and initialize the checkbox
    QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // create a layer with reference to the widget
    layoutCheckBox->addWidget(checkBox);            // Set the checkbox in the layer
    layoutCheckBox->setAlignment(Qt::AlignCenter);  // Center the checkbox
    layoutCheckBox->setContentsMargins(0,0,0,0);    // Set the zero padding
    // Set the checkbox in the second column
    ui->create_order_table->setCellWidget(index, 7, checkBoxWidget);

    QWidget *spin_box_widget= new QWidget();
    QSpinBox *spin_box = new QSpinBox();
    spin_box->setMaximum(query.value(3).toULongLong());

    QHBoxLayout *layout_spin_box = new QHBoxLayout(spin_box_widget); // create a layer with reference to the widget
    layout_spin_box->addWidget(spin_box);            // Set the checkbox in the layer
    layout_spin_box->setAlignment(Qt::AlignCenter);  // Center the checkbox
    layout_spin_box->setContentsMargins(0,0,0,0);    // Set the zero padding

    ui->create_order_table->setCellWidget(index, 8, spin_box_widget);

    index++;
  }

  //for (auto& it : books_id)
  //  qDebug() << it;

  ui->create_order_table->resizeColumnsToContents();
}

void CreateOrder::on_back_to_main_button_clicked()
{
  emit close_window();
}


void CreateOrder::on_make_order_button_clicked()
{
  QSqlQuery query_init_order;
  query_init_order.setForwardOnly(true);

  for (int i = 0; i < ui->create_order_table->rowCount(); i++)
  {
    QWidget *item = (ui->create_order_table->cellWidget(i, 7));
    // Taking the widget from the layout and cast it to QCheckBox
    QCheckBox *checkB = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());

    if (checkB->isChecked())
    {
      QWidget *spin= (ui->create_order_table->cellWidget(i, 8));
      // Taking the widget from the layout and cast it to QCheckBox
      QSpinBox *spinB = qobject_cast<QSpinBox*>(spin->layout()->itemAt(0)->widget());

      if (spinB->value() == 0)
      {
        qDebug() << "Количество должно быть больше 0";
        spinB->setFocus();
        return;
      }
    }
  }

  query_init_order.prepare("{CALL Add_Order(:client_id, :date)}");
  query_init_order.bindValue(":client_id", user_id, QSql::In);
  query_init_order.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"), QSql::In);

  if (query_init_order.exec())
  {
    qDebug() << "Заказ создан!";
    QSqlQuery query_get_order_id;
    query_get_order_id.prepare("SELECT MAX(OrderId) FROM Orders");

    if (query_get_order_id.exec())
    {
      query_get_order_id.next();
      for (int index_of_book = 0; index_of_book < ui->create_order_table->rowCount(); index_of_book++)
      {
        QWidget *item = (ui->create_order_table->cellWidget(index_of_book, 7));
        // Taking the widget from the layout and cast it to QCheckBox
        QCheckBox *checkB = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());

        if (checkB->isChecked())
        {
          QWidget *spin= (ui->create_order_table->cellWidget(index_of_book, 8));
          // Taking the widget from the layout and cast it to QCheckBox
          QSpinBox *spinB = qobject_cast<QSpinBox*>(spin->layout()->itemAt(0)->widget());

          QSqlQuery query_add_books_in_order;
          query_add_books_in_order.setForwardOnly(true);

          query_add_books_in_order.prepare("{CALL Add_Book_In_Order(:order_id, :book_id, :amount)}");
          query_add_books_in_order.bindValue(":order_id", query_get_order_id.value(0).toULongLong(), QSql::In);
          query_add_books_in_order.bindValue(":book_id", books_id.at(index_of_book), QSql::In);
          query_add_books_in_order.bindValue(":amount", spinB->value(), QSql::In);

          if (query_add_books_in_order.exec())
          {
            qDebug() << "Книга добавлена в заказ!";
            QSqlQuery query_set_price;
            query_set_price.setForwardOnly(true);

            query_set_price.prepare("{CALL Set_Sum_Price(:order_id)}");
            query_set_price.bindValue(":order_id", query_get_order_id.value(0).toULongLong(), QSql::In);

            if (query_set_price.exec())
            {
            }
            else
            {
              //qDebug() << query_set_price.lastError().text();
            }
          }
          else
          {
            //qDebug() << query_add_books_in_order.lastError().text();
          }
        }
      }
    }
    emit order_created();
  }
  else
  {
      //qDebug() << query_init_order.lastError().text();
  }

}

