﻿/*************************************************************************************
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
#include "lib_facade.h"

LibFacade::LibFacade(QObject *parent) : QObject(parent)
{
    MyConfiguration.reset(new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player")));
    MyIndexManager.reset(new IndexManager(MyConfiguration.data(), this));
    connect(MyIndexManager.data(), SIGNAL(newIndexDownloaded()), this, SLOT(loadIndex()));
    timer_id = startTimer(300000); // every 300s for ressource monitor

    MyTaskScheduler.reset(new SmilHead::TaskScheduler(MyConfiguration.data(), this));

    connect(MyTaskScheduler.data(), SIGNAL(applyConfiguration()), this, SLOT(loadIndex()));
  //  connect(MyTaskScheduler.data(), SIGNAL(updateFirmware()), this, SLOT(loadIndex()));
   // connect(MyTaskScheduler.data(), SIGNAL(reboot()), this, SLOT(loadIndex()));
}

LibFacade::~LibFacade()
{
    killTimer(timer_id);

    MyIndexManager.data()->deactivateRefresh();
}

void LibFacade::initParser()
{
    initInventoryDataTable();
    loadIndex();
    checkForNewSmilIndex();
}

TConfiguration *LibFacade::getConfiguration() const
{
    return MyConfiguration.data();
}

HeadParser *LibFacade::getHead() const
{
    return MyHeadParser.data();
}

void LibFacade::beginSmilBodyParsing()
{
    connect(MyBodyParser.data(), SIGNAL(startShowMedia(BaseMedia *)), this, SLOT(emitStartShowMedia(BaseMedia *)));
    connect(MyBodyParser.data(), SIGNAL(stopShowMedia(BaseMedia *)), this, SLOT(emitStopShowMedia(BaseMedia *)));
    MyBodyParser->beginSmilParsing(MyIndexManager->getBody());
}

void LibFacade::playNextSmilElement()
{

}

void LibFacade::playPreviousSmilElement()
{

}

void LibFacade::playSmilElement(int position, int zone)
{
    Q_UNUSED(position);
    Q_UNUSED(zone);
}


void LibFacade::initInventoryDataTable()
{
    MyInventoryTable.reset(new DB::InventoryTable(this));
    MyInventoryTable.data()->init(MyConfiguration.data()->getPaths("logs"));
}

void LibFacade::loadIndex()
{
    MyIndexManager.data()->init(MyConfiguration.data()->getIndexUri());
    qDebug(Develop) << "start" << Q_FUNC_INFO;
    if (!MyBodyParser.isNull())
    {
        MyBodyParser.data()->endSmilParsing();
        MyIndexManager.data()->deactivateRefresh();
    }

    // Start with this only when it is absolutly sure that in the player component is no activity anymore.
    if (!MyIndexManager.data()->load())
        return;

    processHeader();

    MyIndexManager.data()->activateRefresh(MyHeadParser->getRefreshTime());
    MyElementsContainer.reset(new ElementsContainer(this));

    initFileManager();

    MyBodyParser.reset(new BodyParser(MyMediaManager.data(), MyElementsContainer.data(), this));
    emit newIndexLoaded();
    qDebug(Develop) << "end " << Q_FUNC_INFO;
}

void LibFacade::processHeader()
{
    MyHeadParser.reset(new HeadParser(MyConfiguration.data(), this));
    MyHeadParser.data()->setInventoryTable(MyInventoryTable.data()); // must set before parse
    MyHeadParser.data()->parse(MyIndexManager->getHead(), MyTaskScheduler.data());
}

void LibFacade::initFileManager()
{
    MyMediaModel.reset(new MediaModel(this));
    MyDownloadQueue.reset(new DownloadQueue(MyConfiguration.data(), this));
    MyDownloadQueue.data()->setInventoryTable(MyInventoryTable.data());
    MyMediaManager.reset(new MediaManager(MyMediaModel.data(), MyDownloadQueue.data(), MyConfiguration.data(), this));
}

void LibFacade::checkForNewSmilIndex()
{
    MyIndexManager.data()->lookUpForUpdatedIndex();
}

void LibFacade::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    MyResourceMonitor.refresh();

    // write to logfile
    qInfo(Develop) << MyResourceMonitor.getTotalMemorySystem() << MyResourceMonitor.getFreeMemorySystem();
    qInfo(Develop) << MyResourceMonitor.getMemoryAppUse() << MyResourceMonitor.getMaxMemoryAppUsed();
    qInfo(Develop) << MyResourceMonitor.getThreadsNumber() << MyResourceMonitor.getMaxThreadsNumber();
}

void LibFacade::emitStartShowMedia(BaseMedia *media)
{
    qDebug(Develop) << "begin" << Q_FUNC_INFO;
    emit startShowMedia(media);
    qDebug(Develop) << "end" << Q_FUNC_INFO;
}

void LibFacade::emitStopShowMedia(BaseMedia *media)
{
    qDebug(Develop) << "begin" << Q_FUNC_INFO;
    emit stopShowMedia(media);
    qDebug(Develop) << "end" << Q_FUNC_INFO;
}

