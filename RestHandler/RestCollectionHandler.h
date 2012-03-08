////////////////////////////////////////////////////////////////////////////////
/// @brief document request handler
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triagens GmbH, Cologne, Germany
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
/// @author Dr. Frank Celler
/// @author Copyright 2010-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_REST_HANDLER_REST_COLLECTION_HANDLER_H
#define TRIAGENS_REST_HANDLER_REST_COLLECTION_HANDLER_H 1

#include "RestHandler/RestVocbaseBaseHandler.h"

////////////////////////////////////////////////////////////////////////////////
/// @page RestDocumentTOC
///
/// <ol>
///   <li>@ref RestCollectionCreate "POST /collection/@FA{<collection-identifier>}"</li>
///   <li>@ref RestCollectionRead "GET /collection/@FA{<collection-identifier>}/@FA{<document-identifier>}"</li>
///   <li>@ref RestCollectionReadAll "GET /collection/@FA{<collection-identifier>}"</li>
///   <li>@ref RestCollectionUpdate "PUT /collection/@FA{<collection-identifier>}/@FA{<document-identifier>}"</li>
///   <li>@ref RestCollectionDelete "DELETE /collection/@FA{<collection-identifier>}/@FA{<document-identifier>}"</li>
///   <li>@ref RestCollectionHead "HEAD /collection/@FA{<collection-identifier>}/@FA{<document-identifier>}"</li>
/// </ol>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @page RestDocument REST Interface for Documents
///
/// The basic operations (create, read, update, delete) for documents are mapped
/// to the standard HTTP methods (POST, GET, PUT, DELETE). An identifier for the
/// revision is returned in the "ETag" field. If you modify a document, you can
/// use the "ETag" field to detect conflicts. The revision of a document can be
/// checking using the HTTP method HEAD.
///
/// <hr>
/// @copydoc RestDocumentTOC
/// <hr>
///
/// @anchor RestCollectionCreate
/// @copydetails triagens::avocado::RestCollectionHandler::createDocument
/// <hr>
///
/// @anchor RestCollectionRead
/// @copydetails triagens::avocado::RestCollectionHandler::readSingleDocument
/// <hr>
///
/// @anchor RestCollectionReadAll
/// @copydetails triagens::avocado::RestCollectionHandler::readAllDocuments
/// <hr>
///
/// @anchor RestCollectionUpdate
/// @copydetails triagens::avocado::RestCollectionHandler::updateDocument
/// <hr>
///
/// @anchor RestCollectionDelete
/// @copydetails triagens::avocado::RestCollectionHandler::deleteDocument
/// <hr>
///
/// @anchor RestCollectionHead
/// @copydetails triagens::avocado::RestCollectionHandler::checkDocument
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                             RestCollectionHandler
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

namespace triagens {
  namespace avocado {

////////////////////////////////////////////////////////////////////////////////
/// @brief collection request handler
////////////////////////////////////////////////////////////////////////////////

    class RestCollectionHandler : public RestVocbaseBaseHandler {

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

        RestCollectionHandler (rest::HttpRequest* request, struct TRI_vocbase_s* vocbase);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                   Handler methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        bool isDirect ();

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        string const& queue ();

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        status_e execute ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup AvocadoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

    private:

////////////////////////////////////////////////////////////////////////////////
/// @brief creates a document
////////////////////////////////////////////////////////////////////////////////

      bool createDocument ();

////////////////////////////////////////////////////////////////////////////////
/// @brief reads a single or all documents
////////////////////////////////////////////////////////////////////////////////

      bool readDocument ();

////////////////////////////////////////////////////////////////////////////////
/// @brief reads a single document
////////////////////////////////////////////////////////////////////////////////

      bool readSingleDocument (bool generateBody);

////////////////////////////////////////////////////////////////////////////////
/// @brief reads all documents
////////////////////////////////////////////////////////////////////////////////

      bool readAllDocuments ();

////////////////////////////////////////////////////////////////////////////////
/// @brief updates a document
////////////////////////////////////////////////////////////////////////////////

      bool updateDocument ();

////////////////////////////////////////////////////////////////////////////////
/// @brief deletes a document
////////////////////////////////////////////////////////////////////////////////

      bool deleteDocument ();

////////////////////////////////////////////////////////////////////////////////
/// @brief reads a single document head
////////////////////////////////////////////////////////////////////////////////

      bool checkDocument ();
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
