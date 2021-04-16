#include "unknown.h"

Unknown::Unknown(TContainer *pc, Files::MediaManager *mm, MainConfiguration *config, QObject *parent) : BaseMedia(mm, config, parent)
{
    parent_container = pc;
    setParentTag(pc->getRootElement().nodeName());
    setObjectName("Unknown");
    is_media = false;

}

void Unknown::prepareDurationTimerBeforePlay()
{
    qWarning(Develop) << src << " Unknown tag";
    skipElement();
}

void Unknown::setAttributes()
{
    parseBaseMediaAttributes();
}
