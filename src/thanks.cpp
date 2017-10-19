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
#include <QUrl>
#include <query.hpp>
#include <querypool.hpp>
#include <mainwindow.hpp>
#include <generic.hpp>
#include <ui_mainwindow.h>
#include <wikiedit.hpp>
#include <wikisite.hpp>
#include <wikipage.hpp>
#include <wikiuser.hpp>
#include <syslog.hpp>
#include <configuration.hpp>

using namespace Huggle;

bool huggle_thanks::WikiCk(WikiSite *site)
{
    if (hcfg->GlobalConfigurationWikiAddress == "meta.wikimedia.org/w/")
        return true;
    Generic::MessageBox("Error", "You can use this feature only on WMF wikis");
    return false;
}

huggle_thanks::huggle_thanks()
{
    this->menu = nullptr;
}

huggle_thanks::~huggle_thanks()
{
    delete this->menuThanks;
}

bool huggle_thanks::Register()
{
    this->Init();
    return true;
}

bool huggle_thanks::IsWorking()
{
    return true;
}

void huggle_thanks::Send(WikiEdit *edit)
{
    // https://www.mediawiki.org/wiki/Extension:Thanks#API_Documentation

    ApiQuery *query = new ApiQuery(ActionCustom, edit->GetSite());
    query->SetCustomActionPart("thank");
    query->Parameters = "rev=" + QString::number(edit->RevID) + "&source=huggle&token=" + QUrl::toPercentEncoding(edit->GetSite()->GetProjectConfig()->Token_Csrf);
    query->UsingPOST = true;
    query->Target = "Thanking user " + edit->User->Username;
    QueryPool::HugglePool->AppendQuery(query);
    query->Process();
}

void huggle_thanks::Hook_MainWindowOnLoad(void *window)
{
    this->Window = (Huggle::MainWindow*)window;
    this->menuThanks = new QAction("Thank user for this edit", this->Window->ui->menuUser);
    this->toggle = new QAction("Automatically thank users for every good edit", this->Window->ui->menuUser);
    this->toggle->setCheckable(true);
    this->toggle->setChecked(Generic::SafeBool(hcfg->GetExtensionConfig(this->GetExtensionName(), "toggle", "true")));
    this->Window->ui->menuPage->addAction(this->toggle);
    this->Window->ui->menuPage->addAction(this->menuThanks);
    connect(this->toggle, SIGNAL(triggered()), this, SLOT(Click0()));
    connect(this->menuThanks, SIGNAL(triggered()), this, SLOT(Click1()));
}

void huggle_thanks::ThankForCurrentEdit()
{
    if (!this->Window->CheckEditableBrowserPage() || !huggle_thanks::WikiCk(this->Window->GetCurrentWikiSite()))
    {
        Syslog::HuggleLogs->ErrorLog("This feature can't be used for selected wiki edit");
        return;
    }
    if (this->Window->GetCurrentWikiEdit()->User->IsIP())
    {
        Syslog::HuggleLogs->ErrorLog("This feature works on registered users only!!");
        return;
    }
    this->Send(this->Window->GetCurrentWikiEdit());
}

void huggle_thanks::Hook_GoodEdit(void *edit)
{
    if (!this->toggle->isChecked())
        return;
    WikiEdit *Edit = (WikiEdit*)edit;
    if (Edit->User->IsIP())
        return;
    if (!huggle_thanks::WikiCk(Edit->GetSite()))
    {
        Syslog::HuggleLogs->ErrorLog("This feature can't be used for selected wiki edit");
        return;
    }
    this->Send(Edit);
}

void huggle_thanks::Click0()
{
    hcfg->SetExtensionConfig(this->GetExtensionName(), "toggle", Generic::Bool2String(this->toggle->isChecked()));
}

void huggle_thanks::Click1()
{
    this->ThankForCurrentEdit();
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2("org.huggle.extension.qt", huggle_thanks)
#endif


