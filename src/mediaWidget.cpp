#include "mediaWidget.hpp"

MediaWidget::MediaWidget(QWidget *parent, QueueWidget *queueWidget)
    : QWidget(parent)
{
    if (!queueWidget)
    {
        // alert user that queueWidget is null
        qDebug() << "QueueWidget is null, cannot initialize MediaWidget.";
        return;
    }
    this->queueWidget = queueWidget;
    // First let's load items
    mediaProvider = new LocalMediaProvider(basePath);
    // this->allItems = mediaProvider->listMedia();
    qDebug() << "Loaded media items:" << allItems.size();

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Search bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search media...");

    // Media list
    listWidget = new QListWidget(this);

    layout->addWidget(searchBar);
    layout->addWidget(listWidget);
    setLayout(layout);

    connect(searchBar, &QLineEdit::textChanged, this, &MediaWidget::filterList);
}

void MediaWidget::filterList(const QString &text)
{
    listWidget->clear();
    int itemCount = 0;
    allItems = mediaProvider->searchMedia(text);
    for (const QString &itemText : allItems)
    {
        itemCount++;
        if (itemCount > 1000)
            break;
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(200, 40));

        // Create a container widget
        QWidget *itemWidget = new QWidget;

        // Create layout and button
        QHBoxLayout *layout = new QHBoxLayout(itemWidget);
        QLabel *label = new QLabel(itemText);
        QPushButton *btn = new QPushButton("Add");

        layout->addWidget(label);
        layout->addStretch();
        layout->addWidget(btn);
        layout->setContentsMargins(5, 0, 5, 0);
        itemWidget->setLayout(layout);

        // Add to list
        listWidget->addItem(item);
        listWidget->setItemWidget(item, itemWidget);

        // Optional: connect button
        connect(btn, &QPushButton::clicked, this, [itemText, this]()
                { qDebug() << "Add media:" << basePath + "/" + itemText; 
                    queueWidget->put(mediaProvider->getLocalMediaPath(itemText)); });
    }
}

