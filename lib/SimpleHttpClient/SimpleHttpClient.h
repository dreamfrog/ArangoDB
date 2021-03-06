////////////////////////////////////////////////////////////////////////////////
/// @brief simple http client
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
/// @author Achim Brandt
/// @author Copyright 2009-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_SIMPLE_HTTP_CLIENT_SIMPLE_HTTP_CLIENT_H
#define TRIAGENS_SIMPLE_HTTP_CLIENT_SIMPLE_HTTP_CLIENT_H 1

#include "Basics/Common.h"

#include "Basics/StringBuffer.h"
#include "Logger/Logger.h"
#include "Rest/HttpRequest.h"

namespace triagens {
  namespace httpclient {

    class SimpleHttpResult;
    class GeneralClientConnection;

////////////////////////////////////////////////////////////////////////////////
/// @brief simple http client
////////////////////////////////////////////////////////////////////////////////

    class SimpleHttpClient {

    private:
      SimpleHttpClient (SimpleHttpClient const&);
      SimpleHttpClient& operator= (SimpleHttpClient const&);

    public:

////////////////////////////////////////////////////////////////////////////////
/// @brief state of the connection
////////////////////////////////////////////////////////////////////////////////

      enum request_state {
        IN_CONNECT,
        IN_WRITE,
        IN_READ_HEADER,
        IN_READ_BODY,
        IN_READ_CHUNKED_HEADER,
        IN_READ_CHUNKED_BODY,
        FINISHED,
        DEAD
      };

////////////////////////////////////////////////////////////////////////////////
/// @brief constructs a new http client
////////////////////////////////////////////////////////////////////////////////

      SimpleHttpClient (GeneralClientConnection*,
                        double,
                        bool);

////////////////////////////////////////////////////////////////////////////////
/// @brief destructs a http client
////////////////////////////////////////////////////////////////////////////////

      ~SimpleHttpClient ();

////////////////////////////////////////////////////////////////////////////////
/// @brief close connection
////////////////////////////////////////////////////////////////////////////////

      bool close (); 

////////////////////////////////////////////////////////////////////////////////
/// @brief make a http request
/// the caller has to delete the result object
////////////////////////////////////////////////////////////////////////////////

      SimpleHttpResult* request (rest::HttpRequest::HttpRequestType,
                                 const string&,
                                 const char*,
                                 size_t,
                                 const map<string, string>&);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets username and password
///
/// @param prefix                         prefix for sending username and password
/// @param username                       username
/// @param password                       password
////////////////////////////////////////////////////////////////////////////////

      void setUserNamePassword (const string& prefix,
                                const string& username,
                                const string& password);

////////////////////////////////////////////////////////////////////////////////
/// @brief allows rewriting locations
////////////////////////////////////////////////////////////////////////////////

      void setLocationRewriter (void* data, 
                                std::string (*func)(void*, const std::string&)) {
        _locationRewriter.data = data;
        _locationRewriter.func = func;
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief enable or disable keep-alive
////////////////////////////////////////////////////////////////////////////////

      void setKeepAlive (bool value) {
        _keepAlive = value;
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the current error message
////////////////////////////////////////////////////////////////////////////////

      const string& getErrorMessage () const {
        return _errorMessage;
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief register and dump an error message
////////////////////////////////////////////////////////////////////////////////

      void setErrorMessage (const string& message, bool forceWarn = false) {
        _errorMessage = message;

        if (_warn || forceWarn) {
          LOGGER_WARNING(_errorMessage);
        }
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief register an error message
////////////////////////////////////////////////////////////////////////////////

      void setErrorMessage (const string& message, int error) {
        if (error != 0) {
          _errorMessage = message + ": " + strerror(error);
        }
        else {
          setErrorMessage(message);
        }
      }
    
    private:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns true if the request is in progress
////////////////////////////////////////////////////////////////////////////////

      bool isWorking () const {
        return _state < FINISHED;
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief initialise the connection
////////////////////////////////////////////////////////////////////////////////

      void handleConnect ();

////////////////////////////////////////////////////////////////////////////////
/// @brief get timestamp
////////////////////////////////////////////////////////////////////////////////

      static double now () {
        struct timeval tv;
        gettimeofday(&tv, 0);

        double sec = (double) tv.tv_sec; // seconds
        double usc = (double) tv.tv_usec; // microseconds

        return sec + usc / 1000000.0;
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief reset state
////////////////////////////////////////////////////////////////////////////////

      void reset ();

////////////////////////////////////////////////////////////////////////////////
/// @brief rewrite a location URL
////////////////////////////////////////////////////////////////////////////////

      string rewriteLocation (const string& location) {
        if (_locationRewriter.func != 0) {
          return _locationRewriter.func(_locationRewriter.data, location);
        }

        return location;
      }

////////////////////////////////////////////////////////////////////////////////
/// @brief get the result
/// the caller has to delete the result object
////////////////////////////////////////////////////////////////////////////////

      SimpleHttpResult* getResult ();

////////////////////////////////////////////////////////////////////////////////
/// @brief set the request
///
/// @param method                         request method
/// @param location                       request uri
/// @param body                           request body
/// @param bodyLength                     size of body
/// @param headerFields                   list of header fields
////////////////////////////////////////////////////////////////////////////////

      void setRequest (rest::HttpRequest::HttpRequestType method,
                       const string& location,
                       const char* body,
                       size_t bodyLength,
                       const map<string, string>& headerFields);

////////////////////////////////////////////////////////////////////////////////
/// @brief read the http header
////////////////////////////////////////////////////////////////////////////////

      bool readHeader ();

////////////////////////////////////////////////////////////////////////////////
/// @brief read the http body by content length
////////////////////////////////////////////////////////////////////////////////

      bool readBody ();

////////////////////////////////////////////////////////////////////////////////
/// @brief read the chunk size
////////////////////////////////////////////////////////////////////////////////

      bool readChunkedHeader ();

////////////////////////////////////////////////////////////////////////////////
/// @brief read the net chunk
////////////////////////////////////////////////////////////////////////////////

      bool readChunkedBody ();

    private:

////////////////////////////////////////////////////////////////////////////////
/// @brief connection used (TCP or SSL connection)
////////////////////////////////////////////////////////////////////////////////

      GeneralClientConnection* _connection;

////////////////////////////////////////////////////////////////////////////////
/// @brief write buffer
////////////////////////////////////////////////////////////////////////////////

      triagens::basics::StringBuffer _writeBuffer;

////////////////////////////////////////////////////////////////////////////////
/// @brief read buffer
////////////////////////////////////////////////////////////////////////////////

      triagens::basics::StringBuffer _readBuffer;

      double _requestTimeout;

      bool _warn;

      request_state _state;

      size_t _written;

      string _errorMessage;

////////////////////////////////////////////////////////////////////////////////
/// @brief struct for rewriting location URLs
////////////////////////////////////////////////////////////////////////////////

      struct {
        void* data;
        std::string (*func)(void*, const std::string&);
      }
      _locationRewriter;
    
      uint32_t _nextChunkedSize;

      SimpleHttpResult* _result;

      std::vector<std::pair<std::string, std::string> >_pathToBasicAuth;

      const size_t _maxPacketSize;

      bool _keepAlive;
      
      rest::HttpRequest::HttpRequestType _method;

    };
  }
}

#endif
