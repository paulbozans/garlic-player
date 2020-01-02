#ifndef MPIMAGE_H
#define MPIMAGE_H

#include <QLabel>
#include <QPixmap>
#include "player_base_media.h"
#include "smilparser/media/image.h"

    class PlayerImage : public PlayerBaseMedia
    {
            Q_OBJECT
        public:            explicit      PlayerImage(QObject *parent = nullptr);
            ~PlayerImage();
            void          init(BaseMedia *media);
            void          deinit();
            void          changeSize(int w, int h);
            QWidget      *getView();
        protected:
            QPixmap                 loaded_image;
            QScopedPointer<QLabel, QScopedPointerDeleteLater>  ImageWidget;
    };


#endif // MPIMAGE_H
