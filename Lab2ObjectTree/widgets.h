#ifndef WIDGETS_H
#define WIDGETS_H

#include <QDebug>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

// LogLabel — QLabel, который пишет в консоль при создании и уничтожении.
// Нужен, чтобы наглядно показать работу дерева объектов QObject.
class LogLabel : public QLabel
{
public:
    explicit LogLabel(const QString& text, QWidget* parent = nullptr)
        : QLabel(text, parent)
    {
        qDebug() << "[CREATE] LogLabel" << text;
    }

    ~LogLabel() override
    {
        qDebug() << "[DESTROY] LogLabel" << text();
    }
};

// LogLineEdit — QLineEdit с логированием создания и уничтожения.
class LogLineEdit : public QLineEdit
{
public:
    explicit LogLineEdit(QWidget* parent = nullptr)
        : QLineEdit(parent)
    {
        qDebug() << "[CREATE] LogLineEdit";
    }

    ~LogLineEdit() override
    {
        qDebug() << "[DESTROY] LogLineEdit";
    }
};

// LogButton — QPushButton с логированием создания и уничтожения.
class LogButton : public QPushButton
{
public:
    explicit LogButton(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent)
    {
        qDebug() << "[CREATE] LogButton" << text;
    }

    ~LogButton() override
    {
        qDebug() << "[DESTROY] LogButton" << text();
    }
};

// LogPanel — QFrame-контейнер с логированием создания и уничтожения.
// Используется как «родитель» для других виджетов, чтобы показать,
// как удаление родителя автоматически удаляет всех его детей.
class LogPanel : public QFrame
{
public:
    explicit LogPanel(const QString& name, QWidget* parent = nullptr)
        : QFrame(parent)
    {
        setObjectName(name);
        qDebug() << "[CREATE] LogPanel" << name;
    }

    ~LogPanel() override
    {
        qDebug() << "[DESTROY] LogPanel" << objectName();
    }
};

#endif // WIDGETS_H