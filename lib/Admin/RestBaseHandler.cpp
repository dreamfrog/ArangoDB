////////////////////////////////////////////////////////////////////////////////
/// @brief default handler for error handling and json in-/output
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
/// @author Dr. Frank Celler
/// @author Copyright 2011-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include "RestBaseHandler.h"

#include "BasicsC/tri-strings.h"
#include "Basics/StringUtils.h"
#include "Logger/Logger.h"
#include "Rest/HttpRequest.h"
#include "Rest/HttpResponse.h"

using namespace std;
using namespace triagens::basics;
using namespace triagens::rest;
using namespace triagens::admin;

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RestServer
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

RestBaseHandler::RestBaseHandler (HttpRequest* request)
  : HttpHandler(request) {
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                               HttpHandler methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RestServer
/// @{
////////////////////////////////////////////////////////////////////////////////

void RestBaseHandler::handleError (TriagensError const& error) {
  generateError(HttpResponse::SERVER_ERROR,
                TRI_ERROR_INTERNAL,
                DIAGNOSTIC_INFORMATION(error));
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RestServer
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief generates a result from JSON
////////////////////////////////////////////////////////////////////////////////

void RestBaseHandler::generateResult (TRI_json_t const* json) {
  generateResult(HttpResponse::OK, json);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief generates a result from JSON
////////////////////////////////////////////////////////////////////////////////

void RestBaseHandler::generateResult (HttpResponse::HttpResponseCode code,
                                      TRI_json_t const* json) {
  _response = createResponse(code);
  _response->setContentType("application/json; charset=utf-8");

  int res = TRI_StringifyJson(_response->body().stringBuffer(), json);

  if (res != TRI_ERROR_NO_ERROR) {
    generateError(HttpResponse::SERVER_ERROR,
                  TRI_ERROR_INTERNAL,
                  "cannot generate output");
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief generates an error
////////////////////////////////////////////////////////////////////////////////

void RestBaseHandler::generateError (HttpResponse::HttpResponseCode code, 
                                     int errorCode) {
  char const* message = TRI_errno_string(errorCode);

  if (message) {
    generateError(code, errorCode, string(message));
  }
  else {
    generateError(code, errorCode, string("unknown error"));
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief generates an error
////////////////////////////////////////////////////////////////////////////////

void RestBaseHandler::generateError (HttpResponse::HttpResponseCode code, int errorCode, string const& message) {
  _response = createResponse(code);
  _response->setContentType("application/json; charset=utf-8");

  _response->body().appendText("{\"error\":true,\"errorMessage\":\"");
  _response->body().appendText(StringUtils::escapeUnicode(message));
  _response->body().appendText("\",\"code\":");
  _response->body().appendInteger(code);
  _response->body().appendText(",\"errorNum\":");
  _response->body().appendInteger(errorCode);
  _response->body().appendText("}");
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
