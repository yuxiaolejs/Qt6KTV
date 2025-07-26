#include "queueWidget.hpp"

QueueWidget::QueueWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Media list
    listWidget = new QListWidget(this);

    layout->addWidget(listWidget);
    setLayout(layout);
}

void QueueWidget::renderList()
{
    listWidget->clear();
    int itemCount = 0;
    for (const QString &itemText : allItems)
    {
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(200, 40)); // Adjust size for widget

        // Create a container widget
        QWidget *itemWidget = new QWidget;

        // Create layout and button
        QHBoxLayout *layout = new QHBoxLayout(itemWidget);
        QLabel *label = new QLabel(itemText);
        QPushButton *deleteBtn = new QPushButton("Delete");
        QPushButton *topBtn = new QPushButton("Top");

        layout->addWidget(label);
        layout->addStretch();
        layout->addWidget(deleteBtn);
        layout->addWidget(topBtn);
        layout->setContentsMargins(5, 0, 5, 0);
        itemWidget->setLayout(layout);

        // Add to list
        listWidget->addItem(item);
        listWidget->setItemWidget(item, itemWidget);

        // Optional: connect button
        connect(deleteBtn, &QPushButton::clicked, this, [itemText, this]()
                { qDebug() << "Delete media:" << itemText;
                    int index = indexOf(itemText);
                    if (index != -1) {
                        allItems.removeAt(index);
                        renderList();
                    } });
        connect(topBtn, &QPushButton::clicked, this, [itemText, this]()
                { 
                    qDebug() << "Move to top:" << itemText; 
                    int index = indexOf(itemText);
                    if (index != -1) {
                        allItems.removeAt(index);
                        allItems.prepend(itemText);
                        renderList();
                    } });
    }
}

void QueueWidget::put(const QString &itemText)
{
    allItems.append(itemText);
    renderList();
    emit itemAdded(itemText);
}
QString QueueWidget::get()
{
    if (!allItems.isEmpty())
    {
        QString item = allItems.takeFirst();
        renderList();
        emit itemRemoved(item);
        return item;
    }
    return QString();
}
QString QueueWidget::get(int index) const
{
    if (index >= 0 && index < allItems.size())
    {
        return allItems.at(index);
    }
    return QString();
}
int QueueWidget::indexOf(const QString &itemText) const
{
    return allItems.indexOf(itemText);
}
