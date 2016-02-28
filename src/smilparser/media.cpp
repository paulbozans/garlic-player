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

#include "media.h"

TMedia::TMedia(TBaseTiming * parent)
{
    Q_UNUSED(parent);
}

bool TMedia::parse(QDomElement element)
{
    root_element   = element;
    setAttributes();     // special for every media type
    setBaseParameters(); // in this class
    return true;
}

QString TMedia::getRegion()
{
    return region;
}

QString TMedia::getSrc()
{
    return src;
}

QString TMedia::getExec()
{
    return exec;
}

QString TMedia::getFileName()
{
    return filename;
}

QString TMedia::getCacheControl()
{
    return cache_control;
}

void TMedia::setBaseMediaAttributes()
{
    region = title = src = exec = "";
    if (root_element.hasAttribute("region"))
        region = root_element.attribute("region");
    if (root_element.hasAttribute("src"))
        src = root_element.attribute("src");
    if (root_element.hasAttribute("exec"))
        exec = root_element.attribute("exec");
    setTimingAttributes();
    return;
}

void TMedia::emitfinished() // called from finishedActiveDuration() in TBase
{
   qDebug() << getID() << QTime::currentTime().toString() << "finished Media";
   emit finishedMedia(parent_container, this);
   return;
}


void TMedia::setBaseParameters()
{
    cache_control = filename = log_content_id = "";
    if (root_element.hasChildNodes())
    {
        QDomNodeList childs = root_element.childNodes();
        QDomElement param;
        for (int i = 0; i < childs.length(); i++)
        {
            param = childs.item(i).toElement();
            if (param.tagName() == "param")
            {
                if (param.hasAttribute("cacheControl") && param.hasAttribute("value"))
                    cache_control = param.attribute("value");
                else if (param.hasAttribute("filename") && param.hasAttribute("value"))
                    filename = param.attribute("value");
                else if (param.hasAttribute("logContentId") && param.hasAttribute("value"))
                    log_content_id = param.attribute("value");
            }
        }

    }
}

