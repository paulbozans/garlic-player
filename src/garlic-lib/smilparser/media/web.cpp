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

#include "web.h"


TWeb::TWeb(Files::MediaManager *mm, MainConfiguration *config, SmilHead::PlaceHolder *ph, QObject *parent) : BaseMedia(mm, config, ph, parent)
{
    setObjectName("TWeb");
    is_media = true;
}

TWeb::~TWeb()
{
}

void TWeb::prepareDurationTimers()
{
    if (!MyExpr.executeQuery())
    {
        skipElement();
        return;
    }

    if (status == _active && !isRestartable())
        return;

    // do not mind if cached
    if (startDurTimer() || hasEndTimer())
    {
        resetInternalRepeatCount();
        emitStartElementSignal(this);
    }
    else
    {
        skipElement();
    }
}

void TWeb::setAttributes()
{
    parseBaseMediaAttributes();
    src        = getAttributeFromRootElement("src", "");    
}

void TWeb::prepareDurationTimersForRepeat()
{
    startDurTimer();
}
