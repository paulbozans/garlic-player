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

#include "main_configuration.h"
#include <QXmlStreamReader>
#include <QString>
#include <memory>
#include <mutex>

// need to define static variable
QString MainConfiguration::log_directory = "";


/**
 * @brief TConfiguration::TConfiguration
 * @param UserConfig
 */
MainConfiguration::MainConfiguration(QSettings *uc, QString dun, QString dcu, QObject *parent) : QObject(parent)
{
    UserConfig = uc;
    uuid        = getUserConfigByKey("uuid");
    player_name = getUserConfigByKey("player_name");
    time_zone   = QTimeZone::systemTimeZoneId();
    default_content_url_name = dun;
    default_content_url = dcu;

    determineOS();

    // ugly workaround from https://stackoverflow.com/questions/21976264/qt-isodate-formatted-date-time-including-timezone
    // cause otherwise we get no time zone in date string
    setStartTime(QDateTime::currentDateTime().toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate));
}

void MainConfiguration::setAdditionalVersion(QString value)
{
    version = QString(version_from_git) + value;
}

void MainConfiguration::setUuid(const QString &value)
{
    uuid = value;
    setUserConfigByKey("uuid", value);
}

QString MainConfiguration::getLogDir()
{
    return MainConfiguration::log_directory;
}

QString MainConfiguration::getLastPlayedIndexPath()
{
    return getUserConfigByKey("last_played_index_path");
}

void MainConfiguration::setLastPlayedIndexPath(const QString &value)
{
    setUserConfigByKey("last_played_index_path", value);
}

QString MainConfiguration::getUserConfigByKey(QString key)
{
    return UserConfig->value(key, "").toString();
}

void MainConfiguration::setUserConfigByKey(QString key, QString value)
{
    UserConfig->setValue(key, value);
}

void MainConfiguration::setPlayerName(const QString &value)
{
    player_name = value;
    setUserConfigByKey("player_name", value);
}

void MainConfiguration::determinePlayerName()
{
    setPlayerName(getUserConfigByKey("player_name"));
    if (getPlayerName() == "")
    {
        setPlayerName(getUuid().mid(24,12));
    }
}

QString MainConfiguration::createUuid()
{
    QString id = QUuid::createUuid().toString();
    return id.mid(1, 36);// must start from 1 cause uuid will created as {uuid}
}

void MainConfiguration::setIndexUri(const QString &value)
{
    index_uri = value;
    setUserConfigByKey("index_uri", value);
}

bool MainConfiguration::validateContentUrl(QString url_string)
{
    QUrl    url(url_string, QUrl::StrictMode);
    bool    error = false;
    error_text    = "";
    if (url_string == "") // isEmpty or isValid do not work as expected
    {
        error_text += "A content-url is neccessary\n";
        error = true;
    }

    if (url.scheme() != "") // prevent thingsq like HtTp or httP
    {
        if (url.toString(QUrl::RemoveScheme).mid(0, 2) != "//")
        {
            error_text += "Url Scheme is no valid! Use "+ url.scheme() + "://domain.tld \n";
            error = true;
        }
        else
        {
            if (url.scheme() != "file") // file:// will irritate garlic-lib TODO! Fix this later!
                setValidatedContentUrl(url.toString());
            else
                setValidatedContentUrl(url.toString(QUrl::RemoveScheme));
        }
    }
    else
    {
        if (url_string.mid(0, 1) != "/")
        {
            error_text += "Relative path for Content-Url/SMIL-Index is not allowed! Use /path/to/index, http://domain.tld or file://path/to/index \n";
            error = true;
        }
        else
            setValidatedContentUrl(url.toString());
    }

    if (error)
        return false;

    return true;
}

void MainConfiguration::determineIndexUri(const QString &value)
{
    if (!value.isEmpty())
    {
        setIndexUri(value);
    }
    else if (!getUserConfigByKey("index_uri").isEmpty()) // get from intern config
    {
        setIndexUri(getUserConfigByKey("index_uri"));
    }
    determineIndexPath();
}

void MainConfiguration::setIndexPath(const QString &value)
{
    index_path = value;
}

/**
 * @brief TConfiguration::determineIndexPath
 * determine the clean index path
 * e.g only the scheme://domain.tld/path_to/
 */
void MainConfiguration::determineIndexPath()
{
    if (index_uri.mid(0, 4) == "http" || index_uri.mid(0, 3) == "ftp")
    {
        QUrl url(index_uri);
        setIndexPath(url.adjusted(QUrl::RemoveFilename |
                                  QUrl::RemoveQuery |
                                  QUrl::RemoveFragment |
                                  QUrl::RemovePort).toString());
        if (index_path.mid(index_path.length()-1, 1) != "/")
            setIndexPath(index_path+"/");

    }
    else
    {
        QFileInfo fi(index_uri);
        setIndexPath(fi.path()+"/");
    }
}



void MainConfiguration::setValidatedContentUrl(const QString &value)
{
    validated_content_url = value;
}

QString MainConfiguration::getStartTime() const
{
    return start_time;
}

void MainConfiguration::setStartTime(const QString &value)
{
    start_time = value;
}

void MainConfiguration::setBasePath(const QString &value)
{
    base_path = value;
}

void MainConfiguration::determineBasePath(QString absolute_path_to_bin)
{
    if (absolute_path_to_bin.contains("/bin"))
        setBasePath(absolute_path_to_bin.mid(0, absolute_path_to_bin.length()-3)); // "/" is set when xyz/bin
    else
        setBasePath(absolute_path_to_bin);

    if (base_path.mid(base_path.length()-1, 1) != "/")
        setBasePath(base_path+"/");
}


/**
 * @brief TConfiguration::getPaths returns paths.
 *        Values can be config, var, media or base
 * @param path_name
 * @return
 */
QString MainConfiguration::getPaths(QString path_name)
{
    QString ret = base_path;
    if (path_name == "cache")
        ret = cache_dir;
    else if (path_name == "logs")
        ret = log_dir;
    else if (path_name == "scripts")
        ret += "scripts/";

    return ret;
}

void MainConfiguration::createDirectories()
{
#if defined Q_OS_WIN32  // QStandardPaths::CacheLocation in windows 7 is set to appdir (bin), which should not be writable after installation in Program Files
    cache_dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/cache/";
    log_dir   = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs/";
#elif defined  Q_OS_ANDROID
    // Using CacheLocation in Android is dangerous, cause that is limited App-Storage which flooding soon and crash the Player
    // GenericDataLocation should be /sdcard
    cache_dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/" +getAppName() + "/cache/";
    log_dir   = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/" + getAppName() + "/logs/";
#else
    cache_dir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+ "/";
    log_dir   = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs/";
#endif
    createDirectoryIfNotExist(cache_dir);
    createDirectoryIfNotExist(log_dir);
    MainConfiguration::log_directory = log_dir;
}

void MainConfiguration::determineUserAgent()
{
    user_agent = "GAPI/1.0 (UUID:"+getUuid()+"; NAME:"+getPlayerName()+") garlic-"+getOS()+"/"+getVersion()+" (MODEL:Garlic)";
}

void MainConfiguration::createDirectoryIfNotExist(QString path)
{
    QDir dir;
    dir.setPath(path);
    if (!dir.exists() && !dir.mkpath("."))
    {
        qCritical(SmilParser) << "Failed to create " << dir.path() << "\r";
    }
    return;
}

void MainConfiguration::determineOS()
{
#if defined  Q_OS_ANDROID
    os = OS_ANDROID;
#elif defined Q_OS_FREEBSD
    os = OS_BSD;
#elif defined Q_OS_HURD
    os = OS_HURD;
#elif defined Q_OS_IOS
    os = OS_IOS;
#elif defined Q_OS_LINUX
    os = OS_LINUX;
#elif defined Q_OS_NETBSD
    os = OS_BSD;
#elif defined Q_OS_OPENBSD
    os = OS_BSD;
#elif defined Q_OS_OSX
    os = OS_OSX;
#elif defined Q_OS_WIN32
    os = OS_WINDOWS;
#elif defined Q_OS_WINRT
    os = OS_WINRT;
#else
    os = OS_UNKNOWN;
#endif
}

