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

#include "base_timings.h"
// remarks ... delete after refactoring
// set end here cause begin and end depending on start of the par/excl parent. With seq it depends on last element
// end attribute has priority over dur (simple duration

BaseTimings::BaseTimings(QObject * parent) : TBase(parent)
{
    Q_UNUSED(parent);
}

BaseTimings::~BaseTimings()
{
    if (BeginTimer != Q_NULLPTR)
    {
        BeginTimer->stop();
        delete BeginTimer;
    }
    if (DurTimer != Q_NULLPTR)
    {
       DurTimer->stop();
       delete DurTimer;
    }
    if (EndTimer != Q_NULLPTR)
    {
        EndTimer->stop();
        delete EndTimer;
    }
    if (InternalTimer != Q_NULLPTR)
    {
        InternalTimer->stop();
        delete InternalTimer;
    }
}


void BaseTimings::prepareTimingsBeforePlaying()
{
    if (EndTimer != Q_NULLPTR)
    {
        EndTimer->start();
    }

    if (BeginTimer != Q_NULLPTR)
    {
        BeginTimer->start();
        status = _waiting;
    }
    else
    {
        prepareDurationTimerBeforePlay();
    }
}

void BaseTimings::prepareTimingsBeforePausing()
{
    if (BeginTimer != Q_NULLPTR)
    {
        BeginTimer->pause();
    }
    if (DurTimer != Q_NULLPTR)
    {
       DurTimer->pause();
    }
    if (EndTimer != Q_NULLPTR)
    {
        EndTimer->pause();
    }
    return;
}

void BaseTimings::prepareTimingsBeforeStop()
{
    if (BeginTimer != Q_NULLPTR)
    {
        BeginTimer->stop();
    }
    if (EndTimer != Q_NULLPTR)
    {
        EndTimer->stop();
    }
    if (DurTimer != Q_NULLPTR)
    {
        DurTimer->stop();
    }
}

void BaseTimings::prepareTimingsBeforeResume()
{
    if (EndTimer != Q_NULLPTR && !EndTimer->resume())
    {
        finishedActiveDuration();
        return; // not go further when ended
    }

    if (BeginTimer != Q_NULLPTR && BeginTimer->resume())
    {
        status          = _waiting;
        return; // not go further when a begin trigger is set
    }

    if (DurTimer != Q_NULLPTR)
    {
        DurTimer->resume();
    }
}

void BaseTimings::finishedNotFound()
{
    // to fix https://github.com/sagiadinos/garlic-player/issues/12
    // when a lonely element (e.g video without dur) in a repeatcount=indefinite playlist not found,
    // a finishedActiveDuration will cause segmentation fault (heap crash) after some some hundreds runs
    // so we need to wait some milliseconds when timer is set.
    if (!isBeginTimerActive() && isEndTimerActive() && isDurTimerActive())
    {
        skipElement();
    }
}

// ========================= protected methods ======================================================

/**
 * @brief TBaseTiming::skipElement
 * this is needed to prevent a recursion stack overflow when download is not complete
 */
void BaseTimings::skipElement()
{
    if (InternalTimer == Q_NULLPTR)
    {
        InternalTimer = new QTimer(this);
        connect(InternalTimer, SIGNAL(timeout()), this, SLOT(emitfinished()));
        InternalTimer->setSingleShot(true);
        InternalTimer->setTimerType(Qt::PreciseTimer);
    }
    InternalTimer->start(100);
}


void BaseTimings::finishedActiveDuration()
{
    if (BeginTimer != Q_NULLPTR && BeginTimer->remainingRepeats())
    {
        BeginTimer->start();
    }
    emitfinished();
}

bool BaseTimings::startDurTimer()
{
    if (DurTimer == Q_NULLPTR)
    {
        return false;
    }

    if (DurTimer->getType() == DurTimer->TYPE_CLOCKVALUE)
    {
        DurTimer->start();
        return true;
    }

    if (DurTimer->getType() == DurTimer->TYPE_INDEFINITE ||
             (EndTimer != Q_NULLPTR && EndTimer->getType() == EndTimer->TYPE_INDEFINITE))
    {
        return true; // no timer needed on indefinite
    }

    return false;
}

void BaseTimings::parseTimingAttributes()
{
    setBaseAttributes();
    if (root_element.hasAttribute("begin"))
    {
        BeginTimer = new EnhancedTimer(this);
        connect(BeginTimer, SIGNAL(timeout()), this, SLOT(releasePlay()));
        BeginTimer->parse(root_element.attribute("begin"));
    }
    if (root_element.hasAttribute("end"))
    {
        EndTimer = new EnhancedTimer(this);
        connect(EndTimer, SIGNAL(timeout()), this, SLOT(finishedActiveDuration()));
        EndTimer->parse(root_element.attribute("end"));
    }
    if (root_element.hasAttribute("dur"))
    {
        DurTimer = new SimpleTimer(this);
        connect(DurTimer, SIGNAL(timeout()), this, SLOT(finishedDuration()));
        DurTimer->parse(root_element.attribute("dur"));
    }
    if (root_element.hasAttribute("repeatCount"))
    {
        setRepeatCount(root_element.attribute("repeatCount"));
    }
}

/**
 * @brief  TBaseTiming::checkRepeatCountStatus returns true, when playlist can be repeated
 * @return bool
 */
bool BaseTimings::checkRepeatCountStatus()
{
    bool ret = false;
    if (indefinite)
    {
        ret = true;
    }
    else if (repeatCount > 0 && internal_count < repeatCount)
    {
        internal_count++;
        ret = true;
    }
    return ret;
}

void BaseTimings::resetInternalRepeatCount()
{
    internal_count = 1;
}

bool BaseTimings::isBeginTimerActive()
{
    if (BeginTimer == Q_NULLPTR)
    {
        return false;
    }
    else
    {
        return BeginTimer->isActive();
    }
}


bool BaseTimings::isEndTimerActive()
{
    if (EndTimer == Q_NULLPTR)
    {
        return false;
    }
    else
    {
        return EndTimer->isActive();
    }
}

bool BaseTimings::isDurTimerActive()
{
    if (DurTimer == Q_NULLPTR)
    {
        return false;
    }
    else
    {
        return DurTimer->isActive();
    }
}

// ========================= private methods ======================================================

void BaseTimings::setRepeatCount(QString rC)
{
    if (rC == "indefinite")
        indefinite = true;
    else
       indefinite = false;
    repeatCount = rC.toInt();
    return;
}

void BaseTimings::releasePlay()
{
    prepareDurationTimerBeforePlay();
}

