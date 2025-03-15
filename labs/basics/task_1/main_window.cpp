#include <QtWidgets>
#include <iostream>
#include "main_window.h"
#include "Ticket.h"
#include <random>
#include <string>

MainWindow::MainWindow()
{

    countOfTickets_ = 0;
    selected_row = -1;
    selected_column = -1;

    QWidget *central_widget = new QWidget;
    setCentralWidget(central_widget);

    count = new QSpinBox(this);
    count->setValue(countOfTickets_);

    view = new QTableWidget(this);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setColumnCount(3);
    view->setHorizontalHeaderLabels({"Номер", "Название", "Статус"});
    view->verticalHeader()->setVisible(false);
    view->setMaximumWidth(460);

    view->setColumnWidth(0, 60);
    view->setColumnWidth(1, 300);
    view->setColumnWidth(2, 100);


    // Отключаем возможность изменения размера колонок
    QHeaderView* header = view->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    header->setSectionResizeMode(2, QHeaderView::Fixed);

    number = new QLabel("Номер", this);
    name = new QLabel("Название", this);
    name_edit = new QLineEdit(this);

    status = new QComboBox(this);
    status->addItems({"Не выучен", "Повторить", "Выучен"});

    QGroupBox *question_view = new QGroupBox(this);
    QVBoxLayout *qv_layout = new QVBoxLayout();
    qv_layout->addWidget(number);
    qv_layout->addWidget(name);
    qv_layout->addWidget(name_edit);
    qv_layout->addWidget(status);
    question_view->setLayout(qv_layout);

  QHBoxLayout *buttons_layout = new QHBoxLayout();
next_question_button = new QPushButton("Новый билет", this);
previous_question_button = new QPushButton("Предыдущий билет", this);

next_question_button->setFixedSize(140, 40);
previous_question_button->setFixedSize(140, 40);

buttons_layout->addStretch();
buttons_layout->addWidget(next_question_button);
buttons_layout->addSpacing(20);
buttons_layout->addWidget(previous_question_button);
buttons_layout->addStretch();

// Прогресс-бары
total_progress = new QProgressBar(this);
total_progress->setValue(0);
total_progress->setFormat("%p%");
total_progress->setFixedHeight(20);
total_progress->setStyleSheet("QProgressBar {"
                            "height: 20px;"
                            "border-radius: 10px;"
                            "background-color: #dcdcdc;"
                            "}"
                            "QProgressBar::chunk {"
                            "background-color: #E0CE06;"  // Желтый цвет
                            "border-radius: 10px;"
                            "}");

green_progress = new QProgressBar(this);
green_progress->setValue(0);
green_progress->setFormat("%p%");
green_progress->setFixedHeight(20);
green_progress->setStyleSheet("QProgressBar {"
                            "height: 20px;"
                            "border-radius: 10px;"
                            "background-color: #dcdcdc;"
                            "}"
                            "QProgressBar::chunk {"
                            "background-color: #27A82B;"  // Зеленый цвет
                            "border-radius: 10px;"
                            "}");

    // Основной layout
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(5, 5, 5, 5);

    QHBoxLayout *count_layout = new QHBoxLayout();
    QLabel *count_label = new QLabel("Количество билетов", this);
    count_layout->addStretch();
    count_layout->addWidget(count_label);
    count_layout->addWidget(count);
    count_layout->addStretch();

    // Сборка всего интерфейса
    main_layout->addLayout(count_layout);
    main_layout->addSpacing(10);
    main_layout->addWidget(view);
    main_layout->addSpacing(10);
    main_layout->addLayout(buttons_layout);
    main_layout->addSpacing(10);
    main_layout->addWidget(question_view);
    main_layout->addSpacing(10);
    main_layout->addWidget(total_progress);
    main_layout->addWidget(green_progress);

    central_widget->setLayout(main_layout);

    createActions();

    setWindowTitle(tr("Билеты МА"));
    setFixedSize(470, 680);

}
void MainWindow::updateTicket(int row)
{
    if (row >= 0 && row < tickets_.size()) {
        Ticket& ticket = tickets_[row];
        revision_tickets_.erase(ticket);
        auto new_status = static_cast<Ticket::Status>(status->currentIndex());
        if (ticket.getStatus() != new_status) {
            if (new_status == Ticket::DEFAULT) {
                grey_count++;
            }
            else if (ticket.getStatus() == Ticket::DEFAULT) {
                grey_count--;
            }
        }

        ticket.setName(ticket.getName());
        ticket.setStatus(static_cast<Ticket::Status>(status->currentIndex()));

        if (ticket.getStatus() != Ticket::GREEN) {
            revision_tickets_.insert(ticket);
        }
        updateProgress();
    }
}

void MainWindow::updateProgress()
{
    total_progress->setValue((tickets_.size() - grey_count) * 100 / tickets_.size());
    green_progress->setValue((tickets_.size() - revision_tickets_.size()) * 100 / tickets_.size());
}


void MainWindow::onEnterPressed()
{
    if (selected_row >= 0) {
        // Обновляем таблицу
        view->setItem(selected_row, 1, new QTableWidgetItem(name_edit->text()));

        int new_status = status->currentIndex();

        // Обновляем статус в таблице и векторе
        view->setItem(selected_row, 2, new QTableWidgetItem(status_data[new_status].first.c_str()));
        QColor selected_color = status_data[new_status].second;

        for (int i = 0; i < view->columnCount(); ++i) {
            view->item(selected_row, i)->setBackground(selected_color);
        }
        name->setText(name_edit->text());

        // Обновляем данные билета
        updateTicket(selected_row);

    }
}

void MainWindow::next_question()
{
    updateTableSize();
}

void MainWindow::change_count(int value)
{
    countOfTickets_ = value;
    updateTableSize();
}
void MainWindow::chooseTicket(int row) {
    tickets_stack_.push(selected_ticket_);
    selected_ticket_ = tickets_[row];
    onCellClicked(row);
}
void MainWindow::onCellClicked(int row)
{
    view->selectRow(row);
    selected_row = row;

    QTableWidgetItem *item = view->item(row, 1);
    if (item) {
        name->setText(item->text());
    } else {
        name->setText("");
    }
    number->setText(QString::number(row + 1));
    name_edit->setText(item ? item->text() : "");
    status->setCurrentIndex(tickets_[row].getStatus());
}

void MainWindow::onCellDoubleClicked()
{
    tickets_stack_.push(selected_ticket_);
    selected_ticket_ = tickets_[selected_row];
    Ticket::Status new_status;
    if (selected_ticket_.getStatus() == Ticket::GREEN) {
       new_status = Ticket::YELLOW;

    }
    else {
        new_status = Ticket::GREEN;
    }

    status->setCurrentIndex(new_status);
    view->setItem(selected_row, 2, new QTableWidgetItem(status_data[new_status].first.c_str()));

    for (int i = 0; i < view->columnCount(); ++i) {
        view->item(selected_row, i)->setBackground(status_data[new_status].second);
    }

    updateTicket(selected_row);
}

void MainWindow::updateTableSize()
{
    number->setText("");
    name->setText("");
    name_edit->setText("");
    green_progress->setValue(0);
    total_progress->setValue(0);
    if (!view) {
        std::cerr << "Ошибка: view не инициализирован" << std::endl;
        return;
    }

    if (!tickets_.empty()) {
        tickets_.clear();
    }
    while (!tickets_stack_.empty()) {
        tickets_stack_.pop();
    }
    if (!revision_tickets_.empty()) {
        revision_tickets_.clear();
    }

    view->setRowCount(count->value());
    grey_count = count->value();

    for (int row = 0; row < count->value(); row++) {
        QTableWidgetItem *item0 = new QTableWidgetItem(
            QString("%1").arg(row + 1));
        view->setItem(row, 0, item0);
        QTableWidgetItem *item1 = new QTableWidgetItem(
            QString("Билет %1").arg(row + 1));
        view->setItem(row, 1, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(
            QString("Не выучен"));
        view->setItem(row, 2, item2);

        auto ticket = Ticket(row + 1, "Билет" + std::to_string(row + 1));
        std::cout << "ticket " << ticket.getName() << std::endl;
        tickets_.push_back(ticket);
        revision_tickets_.emplace(ticket);

        for (int i = 0; i < view->columnCount(); ++i) {
            view->item(row, i)->setBackground(status_data[0].second);
        }

    }
}

void MainWindow::getRandomTicket() {
    if (revision_tickets_.empty()) {
        std::cout << "Все билеты повторены" << std::endl;
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::cout << "size: " << revision_tickets_.size() << std::endl;
    std::uniform_int_distribution<> distr(0, revision_tickets_.size() - 1);
    int const random_number = distr(gen);
    tickets_stack_.push(selected_ticket_);
    auto it = revision_tickets_.begin();
    std::advance(it, random_number);
    selected_ticket_ = *it;
    onCellClicked(selected_ticket_.getNumber() - 1);
}

void MainWindow::getPreviousTicket() {
    if (tickets_stack_.empty()) {
        std::cout << "Не один билет не был выбран" << std::endl;
        return;
    }
    selected_ticket_ = tickets_stack_.top();
    std::cout << "chosen: " <<selected_ticket_.getNumber() << std::endl;
    tickets_stack_.pop();
    onCellClicked(selected_ticket_.getNumber() - 1);
}




void MainWindow::createActions()
{
    if (count && view && name_edit) {
        connect(count, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &MainWindow::change_count);
        connect(name_edit, &QLineEdit::returnPressed,
                this, &MainWindow::onEnterPressed);
        connect(view, &QTableWidget::cellClicked,
                this, &MainWindow::chooseTicket);
        connect(view, &QTableWidget::cellDoubleClicked,
                this, &MainWindow::onCellDoubleClicked);
        connect(next_question_button, &QPushButton::clicked,
                this, &MainWindow::getRandomTicket);
        connect(previous_question_button, &QPushButton::clicked,
                this, &MainWindow::getPreviousTicket);
    }
}