/***************************************************************************
 begin       : Mon Mar 01 2004
 copyright   : (C) 2018 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 * This file is part of the project "AqBanking".                           *
 * Please see toplevel file COPYING of that project for license details.   *
 ***************************************************************************/


#ifndef AQBANKING_BANKINFOPLUGIN_BE_H
#define AQBANKING_BANKINFOPLUGIN_BE_H

#include <aqbanking/backendsupport/bankinfoplugin.h>


typedef struct AB_BANKINFO_PLUGIN AB_BANKINFO_PLUGIN;

#include <aqbanking/banking.h>
#include <gwenhywfar/inherit.h>
#include <gwenhywfar/misc.h>
#include <gwenhywfar/list2.h>
#include <gwenhywfar/plugin.h>


GWEN_INHERIT_FUNCTION_DEFS(AB_BANKINFO_PLUGIN)
GWEN_LIST2_FUNCTION_DEFS(AB_BANKINFO_PLUGIN, AB_BankInfoPlugin)


typedef AB_BANKINFO_PLUGIN *(*AB_BANKINFO_PLUGIN_FACTORY_FN)(AB_BANKING *ab);



/** @name Prototypes For Virtual Functions
 *
 */
/*@{*/
typedef AB_BANKINFO *(*AB_BANKINFOPLUGIN_GETBANKINFO_FN)(AB_BANKINFO_PLUGIN *bip,
                                                         const char *branchId,
                                                         const char *bankId);

typedef int (*AB_BANKINFOPLUGIN_GETBANKINFOBYTMPLATE_FN)(AB_BANKINFO_PLUGIN *bip,
                                                         AB_BANKINFO *tbi,
                                                         AB_BANKINFO_LIST2 *bl);


typedef AB_BANKINFO_CHECKRESULT(*AB_BANKINFOPLUGIN_CHECKACCOUNT_FN)(AB_BANKINFO_PLUGIN *bip,
                                                                    const char *branchId,
                                                                    const char *bankId,
                                                                    const char *accountId);
/*@}*/



/** @name Constructors, Destructors
 *
 */
/*@{*/
AB_BANKINFO_PLUGIN *AB_BankInfoPlugin_new(const char *country);
void AB_BankInfoPlugin_free(AB_BANKINFO_PLUGIN *bip);
void AB_BankInfoPlugin_Attach(AB_BANKINFO_PLUGIN *bip);
/*@}*/



/** @name Informational Functions
 *
 */
/*@{*/

const char *AB_BankInfoPlugin_GetCountry(const AB_BANKINFO_PLUGIN *bip);
/*@}*/



/** @name Virtual Functions
 *
 */
/*@{*/
AB_BANKINFO *AB_BankInfoPlugin_GetBankInfo(AB_BANKINFO_PLUGIN *bip,
                                           const char *branchId,
                                           const char *bankId);

int AB_BankInfoPlugin_GetBankInfoByTemplate(AB_BANKINFO_PLUGIN *bip,
                                            AB_BANKINFO *tbi,
                                            AB_BANKINFO_LIST2 *bl);

AB_BANKINFO_CHECKRESULT AB_BankInfoPlugin_CheckAccount(AB_BANKINFO_PLUGIN *bip,
                                                       const char *branchId,
                                                       const char *bankId,
                                                       const char *accountId);
/*@}*/


/** @name Setters For Virtual Functions
 *
 */
/*@{*/
void AB_BankInfoPlugin_SetGetBankInfoFn(AB_BANKINFO_PLUGIN *bip, AB_BANKINFOPLUGIN_GETBANKINFO_FN f);
void AB_BankInfoPlugin_SetGetBankInfoByTemplateFn(AB_BANKINFO_PLUGIN *bip, AB_BANKINFOPLUGIN_GETBANKINFOBYTMPLATE_FN f);
void AB_BankInfoPlugin_SetCheckAccountFn(AB_BANKINFO_PLUGIN *bip, AB_BANKINFOPLUGIN_CHECKACCOUNT_FN f);
/*@}*/



typedef AB_BANKINFO_PLUGIN *(*AB_PLUGIN_BANKINFO_FACTORY_FN)(GWEN_PLUGIN *pl, AB_BANKING *ab);


GWEN_PLUGIN *AB_Plugin_BankInfo_new(GWEN_PLUGIN_MANAGER *pm,
                                    const char *name,
                                    const char *fileName);


AB_BANKINFO_PLUGIN *AB_Plugin_BankInfo_Factory(GWEN_PLUGIN *pl, AB_BANKING *ab);

void AB_Plugin_BankInfo_SetFactoryFn(GWEN_PLUGIN *pl, AB_PLUGIN_BANKINFO_FACTORY_FN fn);



#endif /* AQBANKING_BANKINFOPLUGIN_BE_H */




