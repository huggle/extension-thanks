//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#ifndef THANKS_H
#define THANKS_H

#define HUGGLE_EXTENSION
#include <huggle_core/iextension.hpp>
#include <QAction>

namespace Huggle
{
    class MainWindow;
    class WikiPage;
    class WikiEdit;
    class WikiSite;
}

class huggle_thanks : public QObject, public Huggle::iExtension
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
        Q_PLUGIN_METADATA(IID "org.huggle.extension.qt" FILE "thanks.json")
#endif
    Q_INTERFACES(Huggle::iExtension)
    public:
        static bool WikiCk(Huggle::WikiSite *site);

        huggle_thanks();
        ~huggle_thanks();
        bool Register();
        bool IsWorking();
        void Hook_MainWindowOnLoad(void *window);
        void Hook_GoodEdit(void *edit);
        QString GetExtensionName() { return "Thanks"; }
        QString GetExtensionAuthor() { return "Petr Bena"; }
        QString GetExtensionDescription() { return "WikiLove buttons"; }
        QString GetExtensionVersion() { return "1.0.0"; }
        QAction *menu;
        QAction *toggle;
        bool RequestCore() { return true; }
        bool RequestNetwork() { return true; }
        bool RequestConfiguration() { return true; }
    private slots:
        void Click0();
        void Click1();
    private:
        void Send(Huggle::WikiEdit *edit);
        void ThankForCurrentEdit();
        QAction *menuThanks = nullptr;
        Huggle::MainWindow *Window;
};

#endif // THANKS_H
