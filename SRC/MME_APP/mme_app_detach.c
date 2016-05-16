/*******************************************************************************
    OpenAirInterface
    Copyright(c) 1999 - 2014 Eurecom

    OpenAirInterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    OpenAirInterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenAirInterface.The full GNU General Public License is
   included in this distribution in the file called "COPYING". If not,
   see <http://www.gnu.org/licenses/>.

  Contact Information
  OpenAirInterface Admin: openair_admin@eurecom.fr
  OpenAirInterface Tech : openair_tech@eurecom.fr
  OpenAirInterface Dev  : openair4g-devel@eurecom.fr

  Address      : Eurecom, Compus SophiaTech 450, route des chappes, 06451 Biot, France.

 *******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "msc.h"
#include "intertask_interface.h"
#include "mme_app_itti_messaging.h"

#include "mme_config.h"

#include "mme_app_ue_context.h"
#include "mme_app_defs.h"
#include "mcc_mnc_itu.h"

#include "assertions.h"



//------------------------------------------------------------------------------
void
mme_app_send_delete_session_request (
  struct ue_context_s                    *ue_context_p)
//------------------------------------------------------------------------------
{
  MessageDef                             *message_p = NULL;
  task_id_t                               to_task = TASK_S11;

#if EPC_BUILD
  to_task = TASK_SPGW_APP;
#endif
  message_p = itti_alloc_new_message (TASK_MME_APP, SGW_DELETE_SESSION_REQUEST);
  AssertFatal (message_p , "itti_alloc_new_message Failed");
  memset ((void *)&message_p->ittiMsg.sgw_delete_session_request, 0, sizeof (itti_sgw_delete_session_request_t));
  SGW_DELETE_SESSION_REQUEST (message_p).teid = ue_context_p->sgw_s11_teid;
  SGW_DELETE_SESSION_REQUEST (message_p).lbi = ue_context_p->default_bearer_id;
  /*
   * S11 stack specific parameter. Not used in standalone epc mode
   */
  SGW_DELETE_SESSION_REQUEST  (message_p).trxn = NULL;
/*  MSC_LOG_TX_MESSAGE (MSC_MMEAPP_MME,
                      (to_task == TASK_S11) ? MSC_S11_MME : MSC_SP_GWAPP_MME,
                      NULL, 0, "0  SGW_DELETE_SESSION_REQUEST teid %u ebi %u", SGW_DELETE_SESSION_REQUEST  (message_p).teid, SGW_DELETE_SESSION_REQUEST  (message_p).bearer_context_to_modify.eps_bearer_id);*/
  itti_send_msg_to_task (to_task, INSTANCE_DEFAULT, message_p);
  OAILOG_FUNC_OUT (LOG_MME_APP);
}


//------------------------------------------------------------------------------
void
mme_app_handle_detach_req (
  const itti_nas_detach_req_t * const detach_req_p)
//------------------------------------------------------------------------------
{
  struct ue_context_s *ue_context    = NULL;

  DevAssert(detach_req_p != NULL);
  ue_context = mme_ue_context_exists_mme_ue_s1ap_id (&mme_app_desc.mme_ue_contexts, detach_req_p->ue_id);
  if (ue_context == NULL) {
    OAILOG_ERROR (LOG_MME_APP, "UE context doesn't exist -> Nothing to do :-) \n");
    OAILOG_FUNC_OUT (LOG_MME_APP);
  }
  else {
    // Send a DELETE_SESSION_REQUEST message to the SGW
    mme_app_send_delete_session_request  (ue_context);
    // CAROLE il vaut miex attendre de recevoir le delete session response pour effacer le contexte
    // mme_remove_ue_context(&mme_app_desc.mme_ue_contexts, ue_context);
    }
  OAILOG_FUNC_OUT (LOG_MME_APP);
}

