/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/
#include "media/image.h"

PlayerImage::PlayerImage(QObject *parent) : PlayerBaseMedia(parent)
{
    ImageWidget.reset(new QLabel);
}

PlayerImage::~PlayerImage()
{
}

void PlayerImage::init(BaseMedia *media)
{
    SmilMedia = media;
    QString path = SmilMedia->getLoadablePath();
    if (isFileExists(path))
    {
        // because sometimes people name a png with jpg extension or vice versa
        // in this case loaded_image.load(path)
        QFile CurrentFile(path);
        if(!CurrentFile.open(QIODevice::ReadOnly))
        {
            SmilMedia->finishedNotFound();
            return;
        }

        if (loaded_image.loadFromData(CurrentFile.readAll()))
        {
            SmilMedia->finishedNotFound();
            return;
        }

        ImageWidget.data()->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ImageWidget.data()->setPixmap(loaded_image);
        if (SmilMedia->getLogContentId() != "")
            setStartTime();
    }
    else
    {
         SmilMedia->finishedNotFound();
    }
}

void PlayerImage::deinit()
{
    loaded_image.load("");
    ImageWidget.data()->setPixmap(loaded_image);
    if (SmilMedia->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();
}

void PlayerImage::changeSize(int w, int h)
{
    if (!exists)
        return;

    QString fit = SmilMedia->getFit().toLower();    

    if (fit == "fill")
       ImageWidget.data()->setPixmap(loaded_image.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else if (fit == "meet")
        ImageWidget.data()->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else if (fit == "meetbest")
    {
        if (loaded_image.width() >= w || loaded_image.height() > h)
            ImageWidget.data()->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        else
            ImageWidget.data()->setPixmap(loaded_image);
    }
    else if (fit == "slice")
        ImageWidget.data()->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else
        ImageWidget.data()->setPixmap(loaded_image);

    QString mediaAlign = SmilMedia->getMediaAlign().toLower();
    if(mediaAlign == "center")
        ImageWidget.data()->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    else if(mediaAlign == "topleft")
        ImageWidget.data()->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    else if(mediaAlign == "topmid")
        ImageWidget.data()->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    else if(mediaAlign == "topright")
        ImageWidget.data()->setAlignment(Qt::AlignTop | Qt::AlignRight);
    else if(mediaAlign == "midleft")
        ImageWidget.data()->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    else if(mediaAlign == "midright")
        ImageWidget.data()->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    else if(mediaAlign == "bottomleft")
        ImageWidget.data()->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    else if(mediaAlign == "bottommid")
        ImageWidget.data()->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    else if(mediaAlign == "bottomright")
        ImageWidget.data()->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    else
        ImageWidget.data()->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

QWidget *PlayerImage::getView()
{
    if (!exists)
        return Q_NULLPTR;

    return ImageWidget.data();
}
