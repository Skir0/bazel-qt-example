#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "Ticket.h"
#include <set>

class Ticket;
QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QComboBox;
class QTableWidget;
class QLineEdit;
class QProgressBar;
class QMenu;
class QSpinBox;
class QPushButton;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void updateStatus(QColor color);

    void highlightRow(int row, const QColor &color);

    void onEnterPressed();
    void updateTicket(int row);

    void updateProgress();

private slots:
    void next_question();

    void change_count(int value);

    void chooseTicket(int row);

    void onCellClicked(int row);

    void updateTableSize();

    void getRandomTicket();

    void getPreviousTicket();

    void onCellDoubleClicked();



private:
    void createActions();
    void createMenus();


    QTableWidget *view;
    QLabel *number;
    QLabel *name;
    QComboBox *status;
    QLineEdit *name_edit;
    QProgressBar *total_progress;
    QProgressBar *green_progress;
    QSpinBox *count;
    QPushButton *next_question_button;
    QPushButton *previous_question_button;


    int selected_row;
    int selected_column;
    int grey_count;

    Ticket selected_ticket_;
    int countOfTickets_ = 0;
    std::vector<Ticket> tickets_;
    std::set<Ticket> revision_tickets_;
    std::stack<Ticket> tickets_stack_;
};

#endif