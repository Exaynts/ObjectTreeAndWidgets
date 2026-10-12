#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

Widget::~Widget()
{
    qDebug() << "[DESTROY] MainWindow";
}

void Widget::setupUi()
{
    setObjectName("MainWindow");
    setWindowTitle("Qt Object Tree Demo");
    resize(520, 640);

    // Создать корневой вертикальный layout для всего окна
    m_rootLayout = new QVBoxLayout(this);

    // Создать заголовок
    m_title = new LogLabel("Дерево объектов Qt: владелец и потомки", this);
    m_rootLayout->addWidget(m_title.data());

    // Создать поле ввода
    m_input = new LogLineEdit(this);
    m_input->setPlaceholderText("Виджет-ребёнок главного окна");
    m_rootLayout->addWidget(m_input.data());

    // Создать статусную строку
    m_status = new LogLabel("Статус: окно создано", this);
    m_status->setWordWrap(true);
    m_rootLayout->addWidget(m_status.data());

    // Создать панель-владельца (LogPanel)
    m_panel = new LogPanel("Панель-владелец", this);
    m_rootLayout->addWidget(m_panel.data());

    // Создать layout внутри панели
    m_panelLayout = new QVBoxLayout(m_panel.data());

    // Создать статическую кнопку внутри панели
    LogButton *staticButton = new LogButton("Статическая кнопка внутри панели", m_panel.data());
    m_panelLayout->addWidget(staticButton);

    // Создать перемещаемый виджет (изначально внутри панели)
    m_movable = new LogButton("Перемещаемый виджет (в панели)", m_panel.data());
    m_panelLayout->addWidget(m_movable.data());

    // Кнопка для перемещения виджета
    m_moveButton = new LogButton("Переместить виджет: панель <-> окно", this);
    connect(m_moveButton.data(), &QPushButton::clicked, this, &Widget::moveMovableWidget);
    m_rootLayout->addWidget(m_moveButton.data());

    // Кнопка удаления панели
    m_deleteButton = new LogButton("Удалить панель (deleteLater)", this);
    connect(m_deleteButton.data(), &QPushButton::clicked, this, &Widget::deletePanel);
    m_rootLayout->addWidget(m_deleteButton.data());

    // Кнопка демонстрации отвязки виджета (setParent(nullptr))
    LogButton *detachButton = new LogButton("Демо setParent(nullptr)", this);
    connect(detachButton, &QPushButton::clicked, this, &Widget::demoDetaching);
    m_rootLayout->addWidget(detachButton);

    // Добавить распорку, чтобы элементы прижались к верху
    m_rootLayout->addStretch();

    setStatus("Готово. Все виджеты созданы с родителями. Удаление родителя автоматически удалит детей.");
}

void Widget::setStatus(const QString &text)
{
    if (!m_status.isNull()) {
        m_status->setText(text);
    }
}

void Widget::moveMovableWidget()
{
    if (m_movable.isNull()) {
        setStatus("Перемещаемый виджет уже уничтожен.");
        return;
    }
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена. Вернуть виджет в неё невозможно.");
        return;
    }

    // Если виджет сейчас в панели -> перемещаем в главное окно
    if (m_movable->parentWidget() == m_panel.data()) {
        if (!m_panelLayout.isNull()) {
            m_panelLayout->removeWidget(m_movable.data());
        }
        m_movable->setParent(this);
        if (!m_rootLayout.isNull()) {
            m_rootLayout->addWidget(m_movable.data());
        }
        m_movable->setText("Перемещаемый виджет (вне панели)");
        setStatus("setParent(this) сделал главное окно новым владельцем. Панель больше не удалит этот виджет автоматически.");
    }
    // Если виджет в главном окне -> возвращаем в панель
    else {
        if (!m_rootLayout.isNull()) {
            m_rootLayout->removeWidget(m_movable.data());
        }
        m_movable->setParent(m_panel.data());
        if (!m_panelLayout.isNull()) {
            m_panelLayout->addWidget(m_movable.data());
        }
        m_movable->setText("Перемещаемый виджет (в панели)");
        setStatus("Виджет снова стал ребёнком панели. Теперь он будет удалён вместе с панелью.");
    }
    m_movable->show();
    qDebug() << "[SET_PARENT] Новый владелец виджета:" << m_movable->parentWidget()->objectName();
}

void Widget::deletePanel()
{
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена.");
        return;
    }

    if (!m_movable.isNull() && m_movable->parentWidget() == m_panel.data()) {
        setStatus("Панель будет удалена. Перемещаемый виджет является её ребёнком, поэтому будет уничтожен автоматически.");
    } else {
        setStatus("Панель будет удалена. Перемещаемый виджет не является её ребёнком, поэтому останется жив.");
    }

    if (!m_deleteButton.isNull()) m_deleteButton->setEnabled(false);
    if (!m_moveButton.isNull()) m_moveButton->setEnabled(false);

    // Безопасное удаление через deleteLater()
    m_panel->deleteLater();
}

void Widget::demoDetaching()
{
    setStatus("Демонстрация setParent(nullptr). Смотрите вывод в консоли.");

    // Создаем временную панель
    LogPanel *tempOwner = new LogPanel("TempOwner", this);

    // Создаем двух детей. child1 не нужен как переменная, он удалится сам.
    new LogLabel("Ребёнок TempOwner", tempOwner);
    LogLabel *child2 = new LogLabel("Отвязанный ребёнок TempOwner", tempOwner);

    qDebug() << "DEMO Количество детей tempOwner до detach:" << tempOwner->children().size();

    // Отвязываем второго ребенка
    child2->setParent(nullptr);

    qDebug() << "DEMO Количество детей tempOwner после detach:" << tempOwner->children().size();

    // Удаляем панель. Первый ребенок удалится автоматически.
    tempOwner->deleteLater();

    // А отвязанного ребенка удаляем вручную, чтобы не было утечки
    child2->deleteLater();

    setStatus("TempOwner удален. 'Ребёнок TempOwner' уничтожен. 'Отвязанный ребёнок' остался жив (утечка памяти).");
}
