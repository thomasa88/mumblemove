#include "mainview.h"

void MainView::contextMenuEvent(QContextMenuEvent *event)
{
    emit contextMenu(event);
}
