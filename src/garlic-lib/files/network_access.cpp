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
#include "network_access.h"

TNetworkAccess::TNetworkAccess(MainConfiguration *config, QObject *parent) : QObject(parent)
{
    MyConfiguration = config;
    this->thread()->setPriority(QThread::LowestPriority);
}

QByteArray TNetworkAccess::getUserAgent()
{
    return MyConfiguration->getUserAgent().toLocal8Bit();
}

QUrl TNetworkAccess::getRemoteFileUrl()
{
    return remote_file_url;
}

void TNetworkAccess::setRemoteFileUrl(QUrl value)
{
    remote_file_url = value;
}

QNetworkRequest TNetworkAccess::prepareNetworkRequest(QUrl remote_url)
{
    QNetworkRequest request(remote_url);
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setRawHeader(QByteArray("User-Agent"), getUserAgent());
    return request;
}
