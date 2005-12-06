/***************************************************************************
 $RCSfile$
 -------------------
 cvs         : $Id$
 begin       : Mon Mar 01 2004
 copyright   : (C) 2004 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "w_rdh_import.h"
#include "a_edituser.h"
#include "a_getcert.h"
#include "a_getsysid.h"
#include "a_finished.h"
#include "a_getsysid.h"
#include "a_getaccounts.h"



WizardRdhImport::WizardRdhImport(QBanking *qb,
                                 WizardInfo *wInfo,
                                 QWidget* parent, const char* name,
                                 bool modal)
:Wizard(qb, wInfo,
        tr("Create a new RDH user"),
        parent, name, modal) {
  WizardAction *wa;

  setDescription(tr("<qt>"
                    "This wizard imports users from a <b>RDH</b> medium."
                    "</qt>"));
  wa=new ActionEditUser(this);
  addAction(wa);

  wa=new ActionGetSysId(this);
  addAction(wa);

  wa=new ActionGetAccounts(this);
  addAction(wa);

  wa=new ActionFinished(this);
  addAction(wa);
}



WizardRdhImport::~WizardRdhImport() {

}



int WizardRdhImport::exec() {
  int rv;

  rv=Wizard::exec();
  if (rv==QDialog::Accepted) {
    AH_USER *u;

    u=getWizardInfo()->getUser();
    assert(u);
    /* import, so always activate the user */
    AH_User_SetStatus(u, AH_UserStatusEnabled);
  }

  return rv;
}



