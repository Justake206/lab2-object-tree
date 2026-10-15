#include "MainWindow.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Task Tracker — Дерево объектов Qt");
    resize(600, 550);
    setupUi();
    qDebug() << "[CREATE] MainWindow";
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

void MainWindow::setupUi()
{
    m_rootLayout = new QVBoxLayout(this);

    // 1. Заголовок — LogLabel, ребёнок главного окна
    m_title = new LogLabel("Трекер задач — дерево объектов QObject", this);
    m_rootLayout->addWidget(m_title.data());

    // 2. Поле ввода — LogLineEdit, ребёнок главного окна
    m_input = new LogLineEdit(this);
    m_input->setPlaceholderText("Название новой задачи");
    m_rootLayout->addWidget(m_input.data());

    // 3. Список задач — QListWidget, ребёнок главного окна
    QListWidget* taskList = new QListWidget(this);
    taskList->setObjectName("taskList");
    m_rootLayout->addWidget(taskList);

    // 4. Панель действий — LogPanel (QFrame), ребёнок главного окна.
    //    Внутри панели будут свои дети — при удалении панели они удалятся автоматически.
    m_panel = new LogPanel("Панель действий", this);
    m_rootLayout->addWidget(m_panel.data());

    m_panelLayout = new QVBoxLayout(m_panel.data());

    // Кнопка «Добавить задачу» — LogButton, ребёнок панели
    LogButton* addButton = new LogButton("Добавить задачу", m_panel.data());
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTask);
    m_panelLayout->addWidget(addButton);

    // Кнопка «Удалить выбранную» — LogButton, ребёнок панели
    LogButton* deleteButton = new LogButton("Удалить выбранную", m_panel.data());
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteSelectedTask);
    m_panelLayout->addWidget(deleteButton);

    // Перемещаемый виджет — для демонстрации setParent()
    m_movable = new LogButton("Перемещаемый виджет (в панели)", m_panel.data());
    m_panelLayout->addWidget(m_movable.data());

    // 5. Кнопки управления демонстрацией — дети главного окна
    m_moveButton = new LogButton("Переместить виджет: панель <-> окно", this);
    connect(m_moveButton.data(), &QPushButton::clicked,
        this, &MainWindow::moveMovableWidget);
    m_rootLayout->addWidget(m_moveButton.data());

    m_deleteButton = new LogButton("Удалить панель (deleteLater)", this);
    connect(m_deleteButton.data(), &QPushButton::clicked,
        this, &MainWindow::deletePanel);
    m_rootLayout->addWidget(m_deleteButton.data());

    LogButton* detachButton = new LogButton("Демо setParent(nullptr)", this);
    connect(detachButton, &QPushButton::clicked, this, &MainWindow::demoDetaching);
    m_rootLayout->addWidget(detachButton);

    // 6. Статус — LogLabel, ребёнок главного окна
    m_status = new LogLabel("Статус: окно создано", this);
    m_status->setWordWrap(true);
    m_rootLayout->addWidget(m_status.data());

    m_rootLayout->addStretch();

    setStatus("Готово. Все виджеты созданы с родителями. "
        "Удаление родителя автоматически удалит детей.");
}

void MainWindow::setStatus(const QString& text)
{
    if (!m_status.isNull()) {
        m_status->setText(text);
    }
}

void MainWindow::addTask()
{
    if (m_input.isNull()) return;
    QString text = m_input->text().trimmed();
    if (text.isEmpty()) {
        setStatus("Введите название задачи.");
        return;
    }
    QListWidget* list = findChild<QListWidget*>("taskList");
    if (list) {
        list->addItem(text);
        m_input->clear();
        setStatus("Задача добавлена: " + text);
    }
}

void MainWindow::deleteSelectedTask()
{
    QListWidget* list = findChild<QListWidget*>("taskList");
    if (!list) return;
    QListWidgetItem* item = list->currentItem();
    if (!item) {
        setStatus("Выберите задачу для удаления.");
        return;
    }
    delete item;
    setStatus("Задача удалена.");
}

void MainWindow::moveMovableWidget()
{
    if (m_movable.isNull()) {
        setStatus("Перемещаемый виджет уже уничтожен.");
        return;
    }
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена. Вернуть виджет в неё невозможно.");
        return;
    }

    if (m_movable->parentWidget() == m_panel.data()) {
        // Из панели → в главное окно
        if (!m_panelLayout.isNull()) {
            m_panelLayout->removeWidget(m_movable.data());
        }
        m_movable->setParent(this);
        if (!m_rootLayout.isNull()) {
            m_rootLayout->addWidget(m_movable.data());
        }
        m_movable->setText("Перемещаемый виджет (вне панели)");
        setStatus("setParent(this): главное окно стало новым владельцем. "
            "Панель больше не удалит этот виджет автоматически.");
    }
    else {
        // Из окна → в панель
        if (!m_rootLayout.isNull()) {
            m_rootLayout->removeWidget(m_movable.data());
        }
        m_movable->setParent(m_panel.data());
        if (!m_panelLayout.isNull()) {
            m_panelLayout->addWidget(m_movable.data());
        }
        m_movable->setText("Перемещаемый виджет (в панели)");
        setStatus("Виджет снова стал ребёнком панели. "
            "Теперь он будет удалён вместе с панелью.");
    }
    m_movable->show();
    qDebug() << "[SET_PARENT] Новый владелец виджета:"
        << m_movable->parentWidget()->objectName();
}

void MainWindow::deletePanel()
{
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена.");
        return;
    }

    if (!m_movable.isNull() && m_movable->parentWidget() == m_panel.data()) {
        setStatus("Панель будет удалена. Перемещаемый виджет — её ребёнок, "
            "поэтому будет уничтожен автоматически.");
    }
    else {
        setStatus("Панель будет удалена. Перемещаемый виджет не её ребёнок, "
            "поэтому останется жив.");
    }

    if (!m_deleteButton.isNull()) m_deleteButton->setEnabled(false);
    if (!m_moveButton.isNull()) m_moveButton->setEnabled(false);

    m_panel->deleteLater();
    m_panel = nullptr;
}

void MainWindow::demoDetaching()
{
    qDebug() << "[DEMO] setParent(nullptr) — отвязка от родителя";

    // Создаём временный LogPanel и двух детей
    LogPanel* tempOwner = new LogPanel("TempOwner", this);
    LogLabel* child1 = new LogLabel("Ребёнок TempOwner", tempOwner);
    LogLabel* child2 = new LogLabel("Отвязанный ребёнок TempOwner", tempOwner);

    qDebug() << "[DEMO] Количество детей tempOwner до detach:"
        << tempOwner->children().size();

    // Отвязываем второго ребёнка от родителя
    child2->setParent(nullptr);

    qDebug() << "[DEMO] Количество детей tempOwner после detach:"
        << tempOwner->children().size();

    // Удаляем владельца — child1 удалится автоматически, child2 останется
    delete tempOwner;

    // child2 теперь без владельца — удаляем вручную
    delete child2;

    setStatus("Демо setParent(nullptr) выполнено. Смотрите лог в консоли.");
}