//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "thanks.hpp"
#include <core.hpp>
#include <collectable_smartptr.hpp>
#include <localization.hpp>
#include <QMessageBox>
#include <query.hpp>
#include <querypool.hpp>
#include <mainwindow.hpp>
#include <generic.hpp>
#include <ui_mainwindow.h>
#include <wikiedit.hpp>
#include <wikisite.hpp>
#include <wikipage.hpp>
#include <wikiuser.hpp>
#include <speedyform.hpp>
#include <syslog.hpp>
#include <configuration.hpp>

using namespace Huggle;

bool huggle_thanks::WikiCk(WikiSite *site)
{
    if (site->Name == "huggle_thanks")
        return true;
    Generic::MessageBox("Error", "You can use this feature only on english wikipedia");
    return false;
}

huggle_thanks::huggle_thanks()
{
}

huggle_thanks::~huggle_thanks()
{
    delete this->menuThanks;
}

bool huggle_thanks::Register()
{
    Huggle::Core::HuggleCore = (Huggle::Core*) this->HuggleCore;
    Huggle::QueryPool::HugglePool = Huggle::Core::HuggleCore->HGQP;
    Huggle::Localizations::HuggleLocalizations = (Huggle::Localizations*) this->Localization;
    Huggle::Syslog::HuggleLogs = Huggle::Core::HuggleCore->HuggleSyslog;
    Huggle::GC::gc = Huggle::Core::HuggleCore->gc;
    Huggle::Query::NetworkManager = this->Networking;
    Huggle::Configuration::HuggleConfiguration = (Huggle::Configuration*) this->Configuration;
    return true;
}

bool huggle_thanks::IsWorking()
{
    return true;
}

void huggle_thanks::Hook_MainWindowOnLoad(void *window)
{
    this->Window = (Huggle::MainWindow*)window;
    this->menuThanks = new QAction("Thank this user", this->Window->ui->menuUser);
    this->Window->ui->menuPage->addAction(this->menuThanks);
    connect(this->menuThanks, SIGNAL(triggered()), this, SLOT(Click1()));
}

void huggle_thanks::Click1()
{
    if (!this->Window->CheckEditableBrowserPage() || !huggle_thanks::WikiCk(this->Window->GetCurrentWikiSite()))
        return;
    QString pn = this->Window->CurrentEdit->Page->GetNS()->GetCanonicalName();
    if (pn == "Template")
    {
        Generic::MessageBox("Error", "You can't use PROD for templates or categories");
        return;
    }
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2("org.huggle.extension.qt", huggle_thanks)
#endif


