#ifndef BRUSH_H
#define BRUSH_H

#include <QObject>
#include "player_base_media.h"
#include "smilparser/media/brush.h"

class Brush : public PlayerBaseMedia
{
        Q_OBJECT
    public:
        Brush(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent = nullptr);
        ~Brush();
        void          init(BaseMedia *media);
        void          deinit();
        void          setParentItem(QQuickItem *parent);
    protected:
        TBrush        *MyBrush;
        QScopedPointer<QQuickItem>  brush_item;
};

#endif // BRUSH_H
