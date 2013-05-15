////////////////////////////////////////////////////////////////////////////////
/// @brief replication logger
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2013 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Jan Steemann
/// @author Copyright 2011-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_VOC_BASE_REPLICATION_H
#define TRIAGENS_VOC_BASE_REPLICATION_H 1

#include "VocBase/document-collection.h"
#include "VocBase/vocbase.h"
#include "VocBase/voc-types.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// --SECTION--                                              forward declarations
// -----------------------------------------------------------------------------

struct TRI_col_info_s;
struct TRI_df_marker_s;
struct TRI_document_collection_s;
struct TRI_doc_mptr_s;
struct TRI_json_s;
struct TRI_transaction_s;
struct TRI_vocbase_col_s;

// -----------------------------------------------------------------------------
// --SECTION--                                                REPLICATION LOGGER
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                     private types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup VocBase
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief context information for replication logging
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_replication_logger_s {
  TRI_mutex_t           _lock;
  int64_t               _logSize;
  TRI_vector_pointer_t  _logs;
  bool                  _active;
  bool                  _waitForSync;
  char*                 _path;
}
TRI_replication_logger_t;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup VocBase
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief create a replication logger
////////////////////////////////////////////////////////////////////////////////

TRI_replication_logger_t* TRI_CreateReplicationLogger (char const*,
                                                       int64_t,
                                                       bool);

////////////////////////////////////////////////////////////////////////////////
/// @brief destroy a replication logger
////////////////////////////////////////////////////////////////////////////////

void TRI_DestroyReplicationLogger (TRI_replication_logger_t*);

////////////////////////////////////////////////////////////////////////////////
/// @brief free a replication logger
////////////////////////////////////////////////////////////////////////////////

void TRI_FreeReplicationLogger (TRI_replication_logger_t*);

////////////////////////////////////////////////////////////////////////////////
/// @brief start the replication logger
////////////////////////////////////////////////////////////////////////////////

int TRI_StartReplicationLogger (TRI_replication_logger_t*);

////////////////////////////////////////////////////////////////////////////////
/// @brief stop the replication logger
////////////////////////////////////////////////////////////////////////////////

int TRI_StopReplicationLogger (TRI_replication_logger_t*);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                       REPLICATION
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup VocBase
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief replicate a transaction
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_REPLICATION

int TRI_TransactionReplication (struct TRI_transaction_s const*);

#else

#define TRI_TransactionReplication(p1)

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief replicate a "create collection" operation
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_REPLICATION

int TRI_CreateCollectionReplication (TRI_voc_cid_t, 
                                     struct TRI_json_s const*);

#else

#define TRI_CreateCollectionReplication(p1, p2)

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief replicate a "drop collection" operation
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_REPLICATION

int TRI_DropCollectionReplication (TRI_voc_cid_t);

#else

#define TRI_DropCollectionReplication(p1)

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief replicate a "rename collection" operation
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_REPLICATION

int TRI_RenameCollectionReplication (TRI_voc_cid_t,
                                     char const*);

#else

#define TRI_RenameCollectionReplication(p1, p2)

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief replicate a "change collection properties" operation
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_REPLICATION

int TRI_ChangePropertiesCollectionReplication (TRI_voc_cid_t,
                                               struct TRI_json_s const*);

#else

#define TRI_ChangePropertiesCollectionReplication(p1, p2)

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief replicate a "create index" operation
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_REPLICATION

int TRI_CreateIndexReplication (TRI_voc_cid_t,
                                TRI_idx_iid_t,
                                struct TRI_json_s const*);

#else

#define TRI_CreateIndexReplication(p1, p2, p3)

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief replicate a "drop index" operation
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_REPLICATION

int TRI_DropIndexReplication (TRI_voc_cid_t,
                              TRI_idx_iid_t iid);

#else

#define TRI_DropIndexReplication(p1, p2)

#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief replicate a document operation
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_REPLICATION

int TRI_DocumentReplication (struct TRI_document_collection_s*,
                             TRI_voc_document_operation_e,
                             struct TRI_df_marker_s const*,
                             struct TRI_doc_mptr_s const*);

#else

#define TRI_DocumentReplication(p1, p2, p3, p4)

#endif

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End: