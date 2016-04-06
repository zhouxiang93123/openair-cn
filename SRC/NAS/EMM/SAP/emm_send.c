/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*****************************************************************************

  Source      emm_send.c

  Version     0.1

  Date        2013/01/30

  Product     NAS stack

  Subsystem   EPS Mobility Management

  Author      Frederic Maurel

  Description Defines functions executed at the EMMAS Service Access
        Point to send EPS Mobility Management messages to the
        Access Stratum sublayer.

*****************************************************************************/

#include "emm_send.h"
#include "commonDef.h"
#include "log.h"

#include "emm_msgDef.h"
#include "emm_proc.h"
#include "mme_config.h"
#include "assertions.h"

#include <string.h>             // strlen

/****************************************************************************/
/****************  E X T E R N A L    D E F I N I T I O N S  ****************/
/****************************************************************************/

/****************************************************************************/
/*******************  L O C A L    D E F I N I T I O N S  *******************/
/****************************************************************************/

/****************************************************************************/
/******************  E X P O R T E D    F U N C T I O N S  ******************/
/****************************************************************************/

/*
   --------------------------------------------------------------------------
   Functions executed by both the UE and the MME to send EMM messages
   --------------------------------------------------------------------------
*/
/****************************************************************************
 **                                                                        **
 ** Name:    emm_send_status()                                         **
 **                                                                        **
 ** Description: Builds EMM status message                                 **
 **                                                                        **
 **      The EMM status message is sent by the UE or the network   **
 **      at any time to report certain error conditions.           **
 **                                                                        **
 ** Inputs:  emm_cause: EMM cause code                             **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:   The EMM message to be sent                 **
 **      Return:    The size of the EMM message                **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_status (
  const emm_as_status_t * msg,
  emm_status_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_WARNING (LOG_NAS_EMM, "EMMAS-SAP - Send EMM Status message (cause=%d)\n", msg->emm_cause);
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = EMM_STATUS;
  /*
   * Mandatory - EMM cause
   */
  size += EMM_CAUSE_MAXIMUM_LENGTH;
  emm_msg->emmcause = msg->emm_cause;
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:    emm_send_detach_accept()                                  **
 **                                                                        **
 ** Description: Builds Detach Accept message                              **
 **                                                                        **
 **      The Detach Accept message is sent by the UE or the net-   **
 **      work to indicate that the detach procedure has been com-  **
 **      pleted.                                                   **
 **                                                                        **
 ** Inputs:  msg:       The EMMAS-SAP primitive to process         **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:   The EMM message to be sent                 **
 **      Return:    The size of the EMM message                **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_detach_accept (
  const emm_as_data_t * msg,
  detach_accept_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Detach Accept message\n");
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = DETACH_ACCEPT;
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}


/*
   --------------------------------------------------------------------------
   Functions executed by the MME to send EMM messages to the UE
   --------------------------------------------------------------------------
*/
/****************************************************************************
 **                                                                        **
 ** Name:    emm_send_attach_accept()                                  **
 **                                                                        **
 ** Description: Builds Attach Accept message                              **
 **                                                                        **
 **      The Attach Accept message is sent by the network to the   **
 **      UE to indicate that the corresponding attach request has  **
 **      been accepted.                                            **
 **                                                                        **
 ** Inputs:  msg:       The EMMAS-SAP primitive to process         **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:   The EMM message to be sent                 **
 **      Return:    The size of the EMM message                **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_attach_accept (
  const emm_as_establish_t * msg,
  attach_accept_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;
  int                                     i = 0;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Attach Accept message\n");
  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size = EMM_HEADER_MAXIMUM_LENGTH(%d)\n", size);
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = ATTACH_ACCEPT;
  /*
   * Mandatory - EPS attach result
   */
  size += EPS_ATTACH_RESULT_MAXIMUM_LENGTH;
  emm_msg->epsattachresult = EPS_ATTACH_RESULT_EPS_IMSI;
  /*
   * Mandatory - T3412 value
   */
  size += GPRS_TIMER_MAXIMUM_LENGTH;
  if (mme_config.nas_config.t3412_min <= 31) {
    emm_msg->t3412value.unit = GPRS_TIMER_UNIT_60S;
    emm_msg->t3412value.timervalue = mme_config.nas_config.t3412_min;
  } else  {
    emm_msg->t3412value.unit = GPRS_TIMER_UNIT_360S;
    emm_msg->t3412value.timervalue = mme_config.nas_config.t3412_min / 6;
  }
  //emm_msg->t3412value.unit = GPRS_TIMER_UNIT_0S;
  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += GPRS_TIMER_MAXIMUM_LENGTH(%d)  (%d)\n", GPRS_TIMER_MAXIMUM_LENGTH, size);
  /*
   * Mandatory - Tracking area identity list
   */
  size += TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH;
  emm_msg->tailist.typeoflist = msg->tai_list.list_type;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - msg->tai_list.list_type = %d\n", msg->tai_list.list_type);

  emm_msg->tailist.numberofelements = msg->tai_list.n_tais - 1;
  emm_msg->tailist.mccdigit1[0] = msg->tai_list.tai[0].plmn.mcc_digit1;
  emm_msg->tailist.mccdigit2[0] = msg->tai_list.tai[0].plmn.mcc_digit2;
  emm_msg->tailist.mccdigit3[0] = msg->tai_list.tai[0].plmn.mcc_digit3;
  emm_msg->tailist.mncdigit1[0] = msg->tai_list.tai[0].plmn.mnc_digit1;
  emm_msg->tailist.mncdigit2[0] = msg->tai_list.tai[0].plmn.mnc_digit2;
  emm_msg->tailist.mncdigit3[0] = msg->tai_list.tai[0].plmn.mnc_digit3;
  emm_msg->tailist.tac[0] = msg->tai_list.tai[0].tac;
  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "TRACKING_AREA_IDENTITY_LIST_LENGTH(%d)  (%d)\n", TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH, size);
  AssertFatal(msg->tai_list.n_tais <= 16, "Twoo many TAIs in TAI list");
  if (TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS == emm_msg->tailist.typeoflist) {
    for (i = 1; i < msg->tai_list.n_tais; i++) {
      emm_msg->tailist.tac[i] = msg->tai_list.tai[i].tac;
      size += 2;
      OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "TRACKING AREA CODE LENGTH(%d)  (%d)\n", 2, size);
    }
  } else if (TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS == emm_msg->tailist.typeoflist) {
    for (i = 1; i < msg->tai_list.n_tais; i++) {
      emm_msg->tailist.mccdigit1[i] = msg->tai_list.tai[i].plmn.mcc_digit1;
      emm_msg->tailist.mccdigit2[i] = msg->tai_list.tai[i].plmn.mcc_digit2;
      emm_msg->tailist.mccdigit3[i] = msg->tai_list.tai[i].plmn.mcc_digit3;
      emm_msg->tailist.mncdigit1[i] = msg->tai_list.tai[i].plmn.mnc_digit1;
      emm_msg->tailist.mncdigit2[i] = msg->tai_list.tai[i].plmn.mnc_digit2;
      emm_msg->tailist.mncdigit3[i] = msg->tai_list.tai[i].plmn.mnc_digit3;
      emm_msg->tailist.tac[i] = msg->tai_list.tai[i].tac;
      size += 5;
      OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "TRACKING AREA IDENTITY LENGTH(%d)  (%d)\n", 5, size);
    }
  }
  /*
   * Mandatory - ESM message container
   */
  size += ESM_MESSAGE_CONTAINER_MINIMUM_LENGTH + msg->nas_msg.length;
  emm_msg->esmmessagecontainer.esmmessagecontainercontents = msg->nas_msg;
  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "ESM_MESSAGE_CONTAINER_MINIMUM_LENGTH(%d)  (%d)\n", ESM_MESSAGE_CONTAINER_MINIMUM_LENGTH, size);

  /*
   * Optional - GUTI
   */
  if (msg->new_guti) {
    size += EPS_MOBILE_IDENTITY_MAXIMUM_LENGTH;
    emm_msg->presencemask |= ATTACH_ACCEPT_GUTI_PRESENT;
    emm_msg->guti.guti.typeofidentity = EPS_MOBILE_IDENTITY_GUTI;
    emm_msg->guti.guti.oddeven = EPS_MOBILE_IDENTITY_EVEN;
    emm_msg->guti.guti.mmegroupid = msg->new_guti->gummei.mme_gid;
    emm_msg->guti.guti.mmecode = msg->new_guti->gummei.mme_code;
    emm_msg->guti.guti.mtmsi = msg->new_guti->m_tmsi;
    emm_msg->guti.guti.mccdigit1 = msg->new_guti->gummei.plmn.mcc_digit1;
    emm_msg->guti.guti.mccdigit2 = msg->new_guti->gummei.plmn.mcc_digit2;
    emm_msg->guti.guti.mccdigit3 = msg->new_guti->gummei.plmn.mcc_digit3;
    emm_msg->guti.guti.mncdigit1 = msg->new_guti->gummei.plmn.mnc_digit1;
    emm_msg->guti.guti.mncdigit2 = msg->new_guti->gummei.plmn.mnc_digit2;
    emm_msg->guti.guti.mncdigit3 = msg->new_guti->gummei.plmn.mnc_digit3;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "EPS_MOBILE_IDENTITY_MAXIMUM_LENGTH(%d)  (%d)\n", EPS_MOBILE_IDENTITY_MAXIMUM_LENGTH, size);
  }

  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:    emm_send_attach_reject()                                  **
 **                                                                        **
 ** Description: Builds Attach Reject message                              **
 **                                                                        **
 **      The Attach Reject message is sent by the network to the   **
 **      UE to indicate that the corresponding attach request has  **
 **      been rejected.                                            **
 **                                                                        **
 ** Inputs:  msg:       The EMMAS-SAP primitive to process         **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:   The EMM message to be sent                 **
 **      Return:    The size of the EMM message                **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_attach_reject (
  const emm_as_establish_t * msg,
  attach_reject_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Attach Reject message (cause=%d)\n", msg->emm_cause);
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = ATTACH_REJECT;
  /*
   * Mandatory - EMM cause
   */
  size += EMM_CAUSE_MAXIMUM_LENGTH;
  emm_msg->emmcause = msg->emm_cause;

  /*
   * Optional - ESM message container
   */
  if (msg->nas_msg.length > 0) {
    size += ESM_MESSAGE_CONTAINER_MINIMUM_LENGTH + msg->nas_msg.length;
    emm_msg->presencemask |= ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT;
    emm_msg->esmmessagecontainer.esmmessagecontainercontents = msg->nas_msg;
  }

  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:        emm_send_tracking_area_update_accept()                    **
 **                                                                        **
 ** Description: Builds Tracking Area Update Accept message                **
 **                                                                        **
 **              The Tracking Area Update Accept message is sent by the    **
 **              network to the UE to indicate that the corresponding      **
 **              tracking area update has been accepted.                   **
 **                                                                        **
 ** Inputs:      msg:           The EMMAS-SAP primitive to process         **
 **              Others:        None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:       The EMM message to be sent                 **
 **              Return:        The size of the EMM message                **
 **              Others:        None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_tracking_area_update_accept (
  const emm_as_establish_t * msg,
  tracking_area_update_accept_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;
  int                                     i = 0;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Tracking Area Update Accept message (cause=%d)\n", msg->emm_cause);
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = TRACKING_AREA_UPDATE_ACCEPT;
  /*
   * Mandatory - EMM cause
   */
  size += EPS_UPDATE_RESULT_MAXIMUM_LENGTH;
  emm_msg->epsupdateresult = msg->eps_update_result;
  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += EPS_UPDATE_RESULT_MAXIMUM_LENGTH(%d)  (%d)\n", EPS_UPDATE_RESULT_MAXIMUM_LENGTH, size);

  // Optional - GPRS Timer T3412
  if (*msg->t3412) {
    size += GPRS_TIMER_MAXIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT;
    if (*msg->t3412 <= 31) {
      emm_msg->t3412value.unit = GPRS_TIMER_UNIT_60S;
      emm_msg->t3412value.timervalue = *msg->t3412;
    } else  {
      emm_msg->t3412value.unit = GPRS_TIMER_UNIT_360S;
      emm_msg->t3412value.timervalue = *msg->t3412 / 6;
    }
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "GPRS_TIMER_MAXIMUM_LENGTH(%d)  (%d)\n", GPRS_TIMER_MAXIMUM_LENGTH, size);
  }
  // Optional - GUTI
  if (msg->guti) {
    size += EPS_MOBILE_IDENTITY_MAXIMUM_LENGTH;
    emm_msg->presencemask |= ATTACH_ACCEPT_GUTI_PRESENT;
    emm_msg->guti.guti.typeofidentity = EPS_MOBILE_IDENTITY_GUTI;
    emm_msg->guti.guti.oddeven = EPS_MOBILE_IDENTITY_EVEN;
    emm_msg->guti.guti.mmegroupid = msg->guti->gummei.mme_gid;
    emm_msg->guti.guti.mmecode = msg->guti->gummei.mme_code;
    emm_msg->guti.guti.mtmsi = msg->guti->m_tmsi;
    emm_msg->guti.guti.mccdigit1 = msg->guti->gummei.plmn.mcc_digit1;
    emm_msg->guti.guti.mccdigit2 = msg->guti->gummei.plmn.mcc_digit2;
    emm_msg->guti.guti.mccdigit3 = msg->guti->gummei.plmn.mcc_digit3;
    emm_msg->guti.guti.mncdigit1 = msg->guti->gummei.plmn.mnc_digit1;
    emm_msg->guti.guti.mncdigit2 = msg->guti->gummei.plmn.mnc_digit2;
    emm_msg->guti.guti.mncdigit3 = msg->guti->gummei.plmn.mnc_digit3;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "EPS_MOBILE_IDENTITY_MAXIMUM_LENGTH(%d)  (%d)\n", EPS_MOBILE_IDENTITY_MAXIMUM_LENGTH, size);
  }
  /* Optional - TAI list
   * This IE may be included to assign a TAI list to a UE.
   */
  if (msg->tai_list.n_tais > 0) {
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT;
    emm_msg->tailist.numberofelements = msg->tai_list.n_tais - 1;
    switch (msg->tai_list.list_type) {
    case TRACKING_AREA_IDENTITY_LIST_TYPE_ONE_PLMN_NON_CONSECUTIVE_TACS:
      size += TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH + emm_msg->tailist.numberofelements*2;
      OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH(%d) + size N extra TACs (%d)  (%d)\n",
          TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH, emm_msg->tailist.numberofelements*2, size);
      emm_msg->tailist.typeoflist = TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS;
      emm_msg->tailist.mccdigit1[0] = msg->tai_list.tai[0].plmn.mcc_digit1;
      emm_msg->tailist.mccdigit2[0] = msg->tai_list.tai[0].plmn.mcc_digit2;
      emm_msg->tailist.mccdigit3[0] = msg->tai_list.tai[0].plmn.mcc_digit3;
      emm_msg->tailist.mncdigit1[0] = msg->tai_list.tai[0].plmn.mnc_digit1;
      emm_msg->tailist.mncdigit2[0] = msg->tai_list.tai[0].plmn.mnc_digit2;
      emm_msg->tailist.mncdigit3[0] = msg->tai_list.tai[0].plmn.mnc_digit3;
      for (i=0; i < msg->tai_list.n_tais; i++) {
        emm_msg->tailist.tac[i]       = msg->tai_list.tai[i].tac;
      }

      break;
    case TRACKING_AREA_IDENTITY_LIST_TYPE_ONE_PLMN_CONSECUTIVE_TACS:
      size += TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH;
      OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH(%d)  (%d)\n", TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH, size);
      emm_msg->tailist.typeoflist = TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_CONSECUTIVE_TACS;
      emm_msg->tailist.mccdigit1[0] = msg->tai_list.tai[0].plmn.mcc_digit1;
      emm_msg->tailist.mccdigit2[0] = msg->tai_list.tai[0].plmn.mcc_digit2;
      emm_msg->tailist.mccdigit3[0] = msg->tai_list.tai[0].plmn.mcc_digit3;
      emm_msg->tailist.mncdigit1[0] = msg->tai_list.tai[0].plmn.mnc_digit1;
      emm_msg->tailist.mncdigit2[0] = msg->tai_list.tai[0].plmn.mnc_digit2;
      emm_msg->tailist.mncdigit3[0] = msg->tai_list.tai[0].plmn.mnc_digit3;
      emm_msg->tailist.tac[0]       = msg->tai_list.tai[0].tac;
      break;
    case TRACKING_AREA_IDENTITY_LIST_TYPE_MANY_PLMNS:
      size += TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH + emm_msg->tailist.numberofelements*5;
      OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH(%d) + size N extra TAIs (%d)  (%d)\n",
          TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH, emm_msg->tailist.numberofelements*5, size);
      emm_msg->tailist.typeoflist = TRACKING_AREA_IDENTITY_LIST_MANY_PLMNS;
      for (i=0; i < msg->tai_list.n_tais; i++) {
        emm_msg->tailist.mccdigit1[i] = msg->tai_list.tai[i].plmn.mcc_digit1;
        emm_msg->tailist.mccdigit2[i] = msg->tai_list.tai[i].plmn.mcc_digit2;
        emm_msg->tailist.mccdigit3[i] = msg->tai_list.tai[i].plmn.mcc_digit3;
        emm_msg->tailist.mncdigit1[i] = msg->tai_list.tai[i].plmn.mnc_digit1;
        emm_msg->tailist.mncdigit2[i] = msg->tai_list.tai[i].plmn.mnc_digit2;
        emm_msg->tailist.mncdigit3[i] = msg->tai_list.tai[i].plmn.mnc_digit3;
        emm_msg->tailist.tac[i]       = msg->tai_list.tai[i].tac;
      }
      size += emm_msg->tailist.numberofelements*5;
      break;
    default:
      ;
    }
  }
  // Optional - EPS Bearer context status
  if (msg->eps_bearer_context_status) {
    size += EPS_BEARER_CONTEXT_STATUS_MAXIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT;
    emm_msg->epsbearercontextstatus = *(msg->eps_bearer_context_status);
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "EPS_BEARER_CONTEXT_STATUS_MAXIMUM_LENGTH(%d)  (%d)\n", EPS_BEARER_CONTEXT_STATUS_MAXIMUM_LENGTH, size);
  }
  /* Useless actually, Optional - Location Area Identification
  if (msg->location_area_identification) {
    size += LOCATION_AREA_IDENTIFICATION_MAXIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
    emm_msg->locationareaidentification = ;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "LOCATION_AREA_IDENTIFICATION_MAXIMUM_LENGTH(%d)  (%d)", LOCATION_AREA_IDENTIFICATION_MAXIMUM_LENGTH, size);
  }*/
  /* Useless actually, Optional - Mobile Identity
  if (msg->mobile_identity) {
    size += MOBILE_IDENTITY_MINIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_PRESENT;
    emm_msg->msidentity = ;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "MOBILE_IDENTITY_MINIMUM_LENGTH(%d)  (%d)", MOBILE_IDENTITY_MINIMUM_LENGTH, size);
  }*/
  // Optional - EMM cause
  if (msg->emm_cause) {
    size += EMM_CAUSE_MAXIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_PRESENT;
    emm_msg->emmcause = msg->emm_cause;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "MOBILE_IDENTITY_MINIMUM_LENGTH(%d)  (%d)\n", EMM_CAUSE_MAXIMUM_LENGTH, size);
  }
  // Optional - GPRS Timer T3402
  if (msg->t3402) {
    size += GPRS_TIMER_MAXIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT;
    if (*msg->t3402 <= 31) {
      emm_msg->t3402value.unit = GPRS_TIMER_UNIT_60S;
      emm_msg->t3402value.timervalue = *msg->t3402;
    } else  {
      emm_msg->t3402value.unit = GPRS_TIMER_UNIT_360S;
      emm_msg->t3402value.timervalue = *msg->t3402 / 6;
    }
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "GPRS_TIMER_MAXIMUM_LENGTH(%d)  (%d)\n", GPRS_TIMER_MAXIMUM_LENGTH, size);
  }
  // Optional - GPRS Timer T3423
  if (msg->t3423) {
    size += GPRS_TIMER_MAXIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT;
    if (*msg->t3423 <= 31) {
      emm_msg->t3423value.unit = GPRS_TIMER_UNIT_60S;
      emm_msg->t3423value.timervalue = *msg->t3423;
    } else  {
      emm_msg->t3423value.unit = GPRS_TIMER_UNIT_360S;
      emm_msg->t3423value.timervalue = *msg->t3423 / 6;
    }
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "GPRS_TIMER_MAXIMUM_LENGTH(%d)  (%d)\n", GPRS_TIMER_MAXIMUM_LENGTH, size);
  }
  // Useless actually, Optional - Equivalent PLMNs
  /*if (msg->equivalent_plmns) {
    size += PLMN_LIST_MINIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_PRESENT;
    emm_msg->equivalentplmns.       = msg->;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "PLMN_LIST_MINIMUM_LENGTH(%d)  (%d)", PLMN_LIST_MINIMUM_LENGTH, size);
  }*/
  /* Useless actually, Optional - Emergency number list
  if (msg->emergency_number_list) {
    size += EMERGENCY_NUMBER_LIST_MINIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT;
    emm_msg->emergencynumberlist.       = msg->;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "EMERGENCY_NUMBER_LIST_MINIMUM_LENGTH(%d)  (%d)", EMERGENCY_NUMBER_LIST_MINIMUM_LENGTH, size);
  }*/
  // Optional - EPS network feature support
  if (msg->eps_network_feature_support) {
    size += EPS_NETWORK_FEATURE_SUPPORT_MAXIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
    emm_msg->epsnetworkfeaturesupport       = *msg->eps_network_feature_support;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "EPS_NETWORK_FEATURE_SUPPORT_MAXIMUM_LENGTH(%d)  (%d)\n", EPS_NETWORK_FEATURE_SUPPORT_MAXIMUM_LENGTH, size);
  }
  /* Useless actually, Optional - Additional update result
  if (msg->additional_update_result) {
    size += ADDITIONAL_UPDATE_RESULT_MAXIMUM_LENGTH;
    emm_msg->presencemask |= TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT;
    emm_msg->additionalupdateresult.       = msg->;
    OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - size += " "ADDITIONAL_UPDATE_RESULT_MAXIMUM_LENGTH(%d)  (%d)", ADDITIONAL_UPDATE_RESULT_MAXIMUM_LENGTH, size);
  }*/
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:        emm_send_tracking_area_update_reject()                    **
 **                                                                        **
 ** Description: Builds Tracking Area Update Reject message                **
 **                                                                        **
 **              The Tracking Area Update Reject message is sent by the    **
 **              network to the UE to indicate that the corresponding      **
 **              tracking area update has been rejected.                   **
 **                                                                        **
 ** Inputs:      msg:           The EMMAS-SAP primitive to process         **
 **              Others:        None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:       The EMM message to be sent                 **
 **              Return:        The size of the EMM message                **
 **              Others:        None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_tracking_area_update_reject (
  const emm_as_establish_t * msg,
  tracking_area_update_reject_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Tracking Area Update Reject message (cause=%d)\n", msg->emm_cause);
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = TRACKING_AREA_UPDATE_REJECT;
  /*
   * Mandatory - EMM cause
   */
  size += EMM_CAUSE_MAXIMUM_LENGTH;
  emm_msg->emmcause = msg->emm_cause;
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:        emm_send_service_reject()                                 **
 **                                                                        **
 ** Description: Builds Service Reject message                             **
 **                                                                        **
 **              The Tracking Area Update Reject message is sent by the    **
 **              network to the UE to indicate that the corresponding      **
 **              tracking area update has been rejected.                   **
 **                                                                        **
 ** Inputs:      msg:           The EMMAS-SAP primitive to process         **
 **              Others:        None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:       The EMM message to be sent                 **
 **              Return:        The size of the EMM message                **
 **              Others:        None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_service_reject (
  const emm_as_establish_t * msg,
  service_reject_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Service Reject message (cause=%d)\n", msg->emm_cause);
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = SERVICE_REJECT;
  /*
   * Mandatory - EMM cause
   */
  size += EMM_CAUSE_MAXIMUM_LENGTH;
  emm_msg->emmcause = msg->emm_cause;
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:    emm_send_identity_request()                               **
 **                                                                        **
 ** Description: Builds Identity Request message                           **
 **                                                                        **
 **      The Identity Request message is sent by the network to    **
 **      the UE to request the UE to provide the specified identi- **
 **      ty.                                                       **
 **                                                                        **
 ** Inputs:  msg:       The EMMAS-SAP primitive to process         **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:   The EMM message to be sent                 **
 **      Return:    The size of the EMM message                **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_identity_request (
  const emm_as_security_t * msg,
  identity_request_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Identity Request message\n");
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = IDENTITY_REQUEST;
  /*
   * Mandatory - Identity type 2
   */
  size += IDENTITY_TYPE_2_MAXIMUM_LENGTH;

  if (msg->ident_type == EMM_IDENT_TYPE_IMSI) {
    emm_msg->identitytype = IDENTITY_TYPE_2_IMSI;
  } else if (msg->ident_type == EMM_IDENT_TYPE_TMSI) {
    emm_msg->identitytype = IDENTITY_TYPE_2_TMSI;
  } else if (msg->ident_type == EMM_IDENT_TYPE_IMEI) {
    emm_msg->identitytype = IDENTITY_TYPE_2_IMEI;
  } else {
    /*
     * All other values are interpreted as "IMSI"
     */
    emm_msg->identitytype = IDENTITY_TYPE_2_IMSI;
  }

  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:    emm_send_authentication_request()                         **
 **                                                                        **
 ** Description: Builds Authentication Request message                     **
 **                                                                        **
 **      The Authentication Request message is sent by the network **
 **      to the UE to initiate authentication of the UE identity.  **
 **                                                                        **
 ** Inputs:  msg:       The EMMAS-SAP primitive to process         **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:   The EMM message to be sent                 **
 **      Return:    The size of the EMM message                **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_authentication_request (
  const emm_as_security_t * msg,
  authentication_request_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Authentication Request message\n");
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = AUTHENTICATION_REQUEST;
  /*
   * Mandatory - NAS key set identifier
   */
  size += NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH;
  emm_msg->naskeysetidentifierasme.tsc = NAS_KEY_SET_IDENTIFIER_NATIVE;
  emm_msg->naskeysetidentifierasme.naskeysetidentifier = msg->ksi;
  /*
   * Mandatory - Authentication parameter RAND
   */
  size += AUTHENTICATION_PARAMETER_RAND_MAXIMUM_LENGTH;
  emm_msg->authenticationparameterrand.rand = *msg->rand;
  /*
   * Mandatory - Authentication parameter AUTN
   */
  size += AUTHENTICATION_PARAMETER_AUTN_MAXIMUM_LENGTH;
  emm_msg->authenticationparameterautn.autn = *msg->autn;
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:    emm_send_authentication_reject()                          **
 **                                                                        **
 ** Description: Builds Authentication Reject message                      **
 **                                                                        **
 **      The Authentication Reject message is sent by the network  **
 **      to the UE to indicate that the authentication procedure   **
 **      has failed and that the UE shall abort all activities.    **
 **                                                                        **
 ** Inputs:  None                                                      **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:   The EMM message to be sent                 **
 **      Return:    The size of the EMM message                **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_authentication_reject (
  authentication_reject_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Authentication Reject message\n");
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = AUTHENTICATION_REJECT;
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:    emm_send_security_mode_command()                          **
 **                                                                        **
 ** Description: Builds Security Mode Command message                      **
 **                                                                        **
 **      The Security Mode Command message is sent by the network  **
 **      to the UE to establish NAS signalling security.           **
 **                                                                        **
 ** Inputs:  msg:       The EMMAS-SAP primitive to process         **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     emm_msg:   The EMM message to be sent                 **
 **      Return:    The size of the EMM message                **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
emm_send_security_mode_command (
  const emm_as_security_t * msg,
  security_mode_command_msg * emm_msg)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     size = EMM_HEADER_MAXIMUM_LENGTH;

  OAILOG_INFO (LOG_NAS_EMM, "EMMAS-SAP - Send Security Mode Command message\n");
  /*
   * Mandatory - Message type
   */
  emm_msg->messagetype = SECURITY_MODE_COMMAND;
  /*
   * Selected NAS security algorithms
   */
  size += NAS_SECURITY_ALGORITHMS_MAXIMUM_LENGTH;
  emm_msg->selectednassecurityalgorithms.typeofcipheringalgorithm = msg->selected_eea;
  emm_msg->selectednassecurityalgorithms.typeofintegrityalgorithm = msg->selected_eia;
  /*
   * NAS key set identifier
   */
  size += NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH;
  emm_msg->naskeysetidentifier.tsc = NAS_KEY_SET_IDENTIFIER_NATIVE;
  emm_msg->naskeysetidentifier.naskeysetidentifier = msg->ksi;
  /*
   * Replayed UE security capabilities
   */
  size += UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH;
  emm_msg->replayeduesecuritycapabilities.eea = msg->eea;
  emm_msg->replayeduesecuritycapabilities.eia = msg->eia;
  emm_msg->replayeduesecuritycapabilities.umts_present = msg->umts_present;
  emm_msg->replayeduesecuritycapabilities.gprs_present = msg->gprs_present;
  emm_msg->replayeduesecuritycapabilities.uea = msg->uea;
  emm_msg->replayeduesecuritycapabilities.uia = msg->uia;
  emm_msg->replayeduesecuritycapabilities.gea = msg->gea;
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, size);
}


/****************************************************************************/
/*********************  L O C A L    F U N C T I O N S  *********************/
/****************************************************************************/
