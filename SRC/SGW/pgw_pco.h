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

/*! \file pgw_lite_paa.h
* \brief
* \author Lionel Gauthier
* \company Eurecom
* \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_PGW_PCO_SEEN
#define FILE_PGW_PCO_SEEN
#include <stdbool.h>
#include "ProtocolConfigurationOptions.h"

int pgw_pco_push_protocol_or_container_id(protocol_configuration_options_t * const pco, pco_protocol_or_container_id_t * const poc_id);

int pgw_process_pco_request_ipcp(protocol_configuration_options_t * const pco_resp, const pco_protocol_or_container_id_t * const poc_id);

int pgw_process_pco_dns_server_request(protocol_configuration_options_t * const pco_resp, const pco_protocol_or_container_id_t * const poc_id);

int pgw_process_pco_link_mtu_request(protocol_configuration_options_t * const pco_resp, const pco_protocol_or_container_id_t * const poc_id);

int pgw_process_pco_request(const protocol_configuration_options_t * const pco_req, protocol_configuration_options_t * pco_resp, bool * const address_allocation_via_nas_signalling);

#endif
