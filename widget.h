#ifndef WIDGET_H
#define WIDGET_H

#include <QPointer>
#include <QBoxLayout>
#include <QWidget>

#include "widgets.h"

class Widget : public QWidget // Используем имя Widget, как создал Qt Creator
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void moveMovableWidget();
    void deletePanel();
    void demoDetaching();

private:
    void setupUi();
    void setStatus(const QString &text);

    QPointer<QBoxLayout> m_rootLayout;
    QPointer<QBoxLayout> m_panelLayout;
    QPointer<LogLabel> m_title;
    QPointer<LogLineEdit> m_input;
    QPointer<LogLabel> m_status;
    QPointer<LogPanel> m_panel;
    QPointer<LogButton> m_movable;
    QPointer<LogButton> m_moveButton;
    QPointer<LogButton> m_deleteButton;
};

#endif // WIDGET_H
