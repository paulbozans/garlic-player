#ifndef DEBUGINFOS_H
#define DEBUGINFOS_H

#include "../player-common/configdialog.h"
#include <QDialog>
#include <QMap>
#include "lib_facade.h"
#include "base_media.h"

namespace Ui
{
    class DebugInfos;
}

#include "ui_debug_infos.h"

class DebugInfos : public QDialog
{
        Q_OBJECT
    public:
        explicit DebugInfos(LibFacade *lib_facade, QWidget *parent = Q_NULLPTR);
        void     setLibFacade(LibFacade *lib_facade);
        ~DebugInfos();
        void outputResourcesUsage();

    protected:
        qint64                 max_memory_used = 0;
        qint64                 max_threads_used = 0;
        QMap<QString, QString> played_media;
        int                    timer_id;
        LibFacade             *MyLibFacade;
        ResourceMonitor       *MyResourceMonitor;
        void                   timerEvent(QTimerEvent *event);
        QString                preparePlayedMediaText(BaseMedia *media);

    private slots:
        int on_open_config_clicked();
        void on_close_dialog_clicked();
        void on_close_player_clicked();

    private:
        Ui::DebugInfos *ui;
};

#endif // DEBUGINFOS_H
