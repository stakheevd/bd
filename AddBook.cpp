#include "AddBook.h"
#include "ui_AddBook.h"

AddBook::AddBook(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddBook),
  author_ids()
{
  ui->setupUi(this);

  ui->author_names_cb->setVisible(false);
  ui->fio_autorh_label_line->setVisible(false);
  ui->fio_author_value->setVisible(false);
  ui->birthdate_label->setVisible(false);
  ui->birthdate_value->setVisible(false);
  ui->deathdate_label->setVisible(false);
  ui->deathdate_value->setVisible(false);
  ui->is_alive_checkbox->setVisible(false);
  ui->error_label->setVisible(false);

  ui->without_author_rb->setChecked(true);

  fill_genres_cb();
  fill_authors_cb();
}

AddBook::~AddBook()
{
  delete ui;
}

void AddBook::on_exist_author_rb_toggled(bool checked)
{
  if (checked)
  {
    ui->fio_autorh_label_line->setVisible(true);
    ui->author_names_cb->setVisible(true);
  }
  else
  {
    ui->fio_autorh_label_line->setVisible(false);
    ui->author_names_cb->setVisible(false);
  }
}

void AddBook::on_new_author_rb_toggled(bool checked)
{
 if (checked)
 {
  ui->fio_autorh_label_line->setVisible(true);
  ui->fio_author_value->setVisible(true);
  ui->birthdate_label->setVisible(true);
  ui->birthdate_value->setVisible(true);
  ui->deathdate_label->setVisible(true);
  ui->deathdate_value->setVisible(true);
  ui->is_alive_checkbox->setVisible(true);
 }
 else
 {
  ui->fio_autorh_label_line->setVisible(false);
  ui->fio_author_value->setVisible(false);
  ui->is_alive_checkbox->setChecked(false);
  ui->birthdate_label->setVisible(false);
  ui->birthdate_value->setVisible(false);
  ui->deathdate_label->setVisible(false);
  ui->deathdate_value->setVisible(false);
  ui->is_alive_checkbox->setVisible(false);
 }
}

void AddBook::fill_genres_cb()
{
  QSqlQuery query;

  query.prepare("SELECT Genre FROM Genre");
  query.exec();

  while (query.next())
  {
    ui->genre_cb->addItem(query.value(0).toString());
  }
}

void AddBook::fill_authors_cb()
{
  QSqlQuery query;

  query.prepare("SELECT FIO, AuthorId FROM Author");
  query.exec();

  author_ids.reserve(query.numRowsAffected());

  while (query.next())
  {
    ui->author_names_cb->addItem(query.value(0).toString());
    author_ids.push_back(query.value(1).toULongLong());
  }
}

void AddBook::on_is_alive_checkbox_stateChanged(int arg1)
{
  if (arg1)
  {
    ui->deathdate_label->setVisible(false);
    ui->deathdate_value->setVisible(false);
  }
  else
  {
    ui->deathdate_label->setVisible(true);
    ui->deathdate_value->setVisible(true);
  }
}


void AddBook::on_add_book_button_clicked()
{
  QString title;
  QString genre;
  QDate year_publication;
  QString annotation;
  double price;
  QString amount;

  if (ui->exist_author_rb->isChecked())
  {
    // Добавление книги с существующим автором
    title = ui->title_value->text();
    size_t author_id;

    if (title.isEmpty())
    {
      ui->error_label->setVisible(true);
      ui->error_label->setText("Название не может быть пустым");
      return;
    }

    genre = ui->genre_cb->currentText();
    year_publication = ui->year_publication_value->date();
    annotation = ui->annotation_value->toPlainText();
    price = ui->price_value->value();
    amount = ui->amount_value->text();
    author_id = author_ids.at(ui->author_names_cb->currentIndex());

    QSqlQuery query;
    query.setForwardOnly(true);

    query.prepare("{CALL Add_Book(:title, :genre, :author_id, :year_publication, :price, :amount, :annotation)}");
    query.bindValue(":title", title, QSql::In);
    query.bindValue(":genre", genre, QSql::In);
    query.bindValue(":author_id", author_id, QSql::In);
    query.bindValue(":year_publication", year_publication.toString("yyyy-MM-dd"), QSql::In);
    query.bindValue(":price", price, QSql::In);
    query.bindValue(":amount", amount, QSql::In);
    query.bindValue(":annotation", annotation, QSql::In);

    if (query.exec())
    {
      qDebug() << "Success!";
      emit book_added();
    }
    else
    {
      ui->error_label->setVisible(true);
      ui->error_label->setText("Некорректные данные о книге");
    }

  }
  else if (ui->new_author_rb->isChecked())
  {
    // Добавление книги с новым автором

    QString birthdate = ui->birthdate_value->date().toString("yyyy-MM-dd");

    QString name = ui->fio_author_value->text();

    if (name.isEmpty())
    {
      ui->error_label->setVisible(true);
      ui->error_label->setText("Имя автора не может быть пустым");
      return;
    }

    title = ui->title_value->text();

    if (title.isEmpty())
    {
      ui->error_label->setVisible(true);
      ui->error_label->setText("Название не может быть пустым");
      return;
    }

    genre = ui->genre_cb->currentText();
    year_publication = ui->year_publication_value->date();
    annotation = ui->annotation_value->toPlainText();
    price = ui->price_value->value();
    amount = ui->amount_value->text();

    QSqlQuery query;
    query.setForwardOnly(true);

    query.prepare("{CALL Add_Book_New_Author(:title, :genre, :name, :birthdate, :deathdate, :year_publication, :price, :amount, :annotation)}");
    query.bindValue(":title", title, QSql::In);
    query.bindValue(":genre", genre, QSql::In);
    query.bindValue(":name", name, QSql::In);
    query.bindValue(":birthdate", birthdate, QSql::In);

    if (ui->is_alive_checkbox->isChecked())
    {
      query.bindValue(":deathdate", QVariant(QVariant::String), QSql::In);
    }
    else
    {
      query.bindValue(":deathdate", ui->deathdate_value->date().toString("yyyy-MM-dd"), QSql::In);
    }

    query.bindValue(":year_publication", year_publication.toString("yyyy-MM-dd"), QSql::In);
    query.bindValue(":price", price, QSql::In);
    query.bindValue(":amount", amount, QSql::In);
    query.bindValue(":annotation", annotation, QSql::In);

    if (query.exec())
    {
      qDebug() << "Success!";
      emit book_added();
    }
    else
    {
      ui->error_label->setVisible(true);
      ui->error_label->setText("Некорректные данные о книге");
    }
  }
  else if (ui->without_author_rb->isChecked())
  {
    // Добавление книги без автора
    title = ui->title_value->text();

    if (title.isEmpty())
    {
      ui->error_label->setVisible(true);
      ui->error_label->setText("Название не может быть пустым");
      return;
    }

    genre = ui->genre_cb->currentText();
    year_publication = ui->year_publication_value->date();
    annotation = ui->annotation_value->toPlainText();
    price = ui->price_value->value();
    amount = ui->amount_value->text();

    QSqlQuery query;
    query.setForwardOnly(true);

    query.prepare("{CALL Add_Book_Without_Author(:title, :genre, :year_publication, :price, :amount, :annotation)}");
    query.bindValue(":title", title, QSql::In);
    query.bindValue(":genre", genre, QSql::In);
    query.bindValue(":year_publication", year_publication.toString("yyyy-MM-dd"), QSql::In);
    query.bindValue(":price", price, QSql::In);
    query.bindValue(":amount", amount, QSql::In);
    query.bindValue(":annotation", annotation, QSql::In);

    if (query.exec())
    {
      qDebug() << "Success!";
      emit book_added();
    }
    else
    {
      ui->error_label->setVisible(true);
      ui->error_label->setText("Некорректные данные о книге");
    }

  }

  emit book_added();
}

