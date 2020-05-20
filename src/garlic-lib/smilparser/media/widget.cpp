#include "widget.h"

TWidget::TWidget(TContainer *pc, QObject *parent) : BaseMedia(parent)
{
    parent_container = pc;
    setObjectName("TWidget");
    is_media = true;

}


TWidget::~TWidget()
{
}

void TWidget::registerInMediaManager(Files::MediaManager *mm)
{
    MyMediaManager = mm;
    MyMediaManager->registerFile(src);
}


void TWidget::prepareDurationTimerBeforePlay()
{
    if (getLoadablePath().isEmpty())
    {
        skipElement();
        return;
    }

    if (startDurTimer() || isEndTimerActive())
    {
        emit startedMedia(parent_container, this);
    }
    else
    {
        skipElement();
    }
}

// ====================  protected methods =================================

void TWidget::setAttributes()
{
    parseBaseMediaAttributes();
}

// ====================  private methods =================================
