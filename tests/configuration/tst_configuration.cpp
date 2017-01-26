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
#include <QString>
#include <QtTest>
#include "tools/configuration.h"

class TestTConfiguration : public QObject
{
    Q_OBJECT

public:
    TestTConfiguration(){}

private Q_SLOTS:
    void cleanup();
    void test_determineIndexUriWhenParameter();
    void test_determineIndexUriWhenIni();
    void test_determineIndexUriWhenConfigXML();
    void test_determineIndexUriWhenFallback();
    void test_determineBasePath();
    void test_createDirectories();
    void test_determineUserAgent();
};

void TestTConfiguration::cleanup()
{
    QDir             dir;
    dir.setPath("var");
    dir.removeRecursively(); // clean up

    dir.setPath("configuration");
    dir.removeRecursively(); // clean up

    dir.setPath("logs");
    dir.removeRecursively(); // clean up
    QSettings *Settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    QFile file(Settings->fileName());
    if (file.exists())
        file.remove();
}

void TestTConfiguration::test_determineIndexUriWhenParameter()
{
    TConfiguration *MyConfig = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));

    MyConfig->determineIndexUri("http://domain.tld/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("http://domain.tld/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("http://domain.tld/path_to/"));

    MyConfig->determineIndexUri("ftp://ftpdomain.tld/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("ftp://ftpdomain.tld/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("ftp://ftpdomain.tld/path_to/"));

    MyConfig->setBasePath("/BasePath/");
    MyConfig->determineIndexUri("/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("/path_to/"));

    MyConfig->setBasePath("/BasePath/");
    MyConfig->determineIndexUri("path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("/BasePath/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("/BasePath/path_to/"));
}

void TestTConfiguration::test_determineIndexUriWhenIni()
{
    QSettings *Settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    TConfiguration *MyConfig = new TConfiguration(Settings);
    Settings->setValue("index_uri", "https://smil-index-via-ini-file.tld/a_path/second/path/index.smil");

    MyConfig->determineIndexUri("");
    QCOMPARE(MyConfig->getIndexUri(), QString("https://smil-index-via-ini-file.tld/a_path/second/path/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("https://smil-index-via-ini-file.tld/a_path/second/path/"));

    MyConfig->determineIndexUri("ftps://ftpdomain.tld/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("ftps://ftpdomain.tld/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("ftps://ftpdomain.tld/path_to/"));

}


void TestTConfiguration::test_determineIndexUriWhenConfigXML()
{
    TConfiguration *MyConfig = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
    QDir dir;
    dir.mkdir("configuration");
    QFile::copy("../tests/data/config.xml", "configuration/config.xml");

    MyConfig->determineIndexUri("");
    QCOMPARE(MyConfig->getIndexUri(), QString("http://indexes.of-a-smil-server.com/path_to_a/index.php?site=player_get_index&owner_id=2"));
    QCOMPARE(MyConfig->getIndexPath(), QString("http://indexes.of-a-smil-server.com/path_to_a/"));

    dir.setPath("configuration");
    dir.removeRecursively(); // clean up
}

void TestTConfiguration::test_determineIndexUriWhenFallback()
{
    TConfiguration *MyConfig = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));

    MyConfig->determineIndexUri("");
    QCOMPARE(MyConfig->getIndexUri(), QString("http://indexes.smil-admin.com"));
    QCOMPARE(MyConfig->getIndexPath(), QString("http://indexes.smil-admin.com/"));
}

void TestTConfiguration::test_determineBasePath()
{
    TConfiguration *MyConfig = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));

    MyConfig->determineBasePath("/home/user/garlic/bin");
    QCOMPARE(MyConfig->getBasePath(), QString("/home/user/garlic/"));

    MyConfig->determineBasePath("/home/user2/garlic2/");
    QCOMPARE(MyConfig->getBasePath(), QString("/home/user2/garlic2/"));

    MyConfig->determineBasePath("/home/user3/garlic3");
    QCOMPARE(MyConfig->getBasePath(), QString("/home/user3/garlic3/"));
}

void TestTConfiguration::test_createDirectories()
{
    TConfiguration *MyConfig = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
    QDir             dir;
    MyConfig->createDirectories();
    dir.setPath("var");
    QCOMPARE(dir.exists(), true);
    dir.removeRecursively(); // clean up

    dir.setPath("configuration");
    QCOMPARE(dir.exists(), true);
    dir.removeRecursively(); // clean up

    dir.setPath("logs");
    QCOMPARE(dir.exists(), true);
    dir.removeRecursively(); // clean up
}


void TestTConfiguration::test_determineUserAgent()
{
   TConfiguration *MyConfig = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
   MyConfig->setUuid("the-uuid-is-this");
   MyConfig->setPlayerName("the playername");
   MyConfig->setOS("hurd");
   MyConfig->determineUserAgent();
   QCOMPARE(MyConfig->getUserAgent(), QString("GAPI/1.0 (UUID:the-uuid-is-this; NAME:the playername) garlic-hurd/0.1.0 (MODEL:Garlic)"));

   return;
}


QTEST_APPLESS_MAIN(TestTConfiguration)

#include "tst_configuration.moc"
