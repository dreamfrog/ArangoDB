////////////////////////////////////////////////////////////////////////////////
/// @brief arango shell client base
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
/// @author Copyright 2012-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_ARANGO_SHELL_ARANGO_CLIENT_H
#define TRIAGENS_ARANGO_SHELL_ARANGO_CLIENT_H 1

#include "Basics/Common.h"

#include "Rest/Endpoint.h"

// -----------------------------------------------------------------------------
// --SECTION--                                              forward declarations
// -----------------------------------------------------------------------------

namespace triagens {
  namespace basics {
    class ProgramOptionsDescription;
    class ProgramOptions;
  }

  namespace rest {
    class Endpoint;
  }

// -----------------------------------------------------------------------------
// --SECTION--                                                class ArangoClient
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoShell
/// @{
////////////////////////////////////////////////////////////////////////////////

  namespace arango {

////////////////////////////////////////////////////////////////////////////////
/// @brief arango shell client base
////////////////////////////////////////////////////////////////////////////////

    class ArangoClient {
      private:
        ArangoClient (const ArangoClient&);
        ArangoClient operator= (const ArangoClient&);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public constants
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoShell
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief default request timeout
////////////////////////////////////////////////////////////////////////////////

        static double const DEFAULT_REQUEST_TIMEOUT;

////////////////////////////////////////////////////////////////////////////////
/// @brief default number of retries
////////////////////////////////////////////////////////////////////////////////

        static size_t const  DEFAULT_RETRIES;

////////////////////////////////////////////////////////////////////////////////
/// @brief default connection timeout
////////////////////////////////////////////////////////////////////////////////

        static double const DEFAULT_CONNECTION_TIMEOUT;

////////////////////////////////////////////////////////////////////////////////
/// @brief ignore sequence used for prompt length calculation (starting point)
///
/// This sequence must be used before any non-visible characters in the prompt.
///
/// From readline/display.c:
/// \\001 (^A) start non-visible characters
/// \\002 (^B) end non-visible characters
///
/// All characters except \\001 and \\002 (following a \\001) are copied to
/// the returned string; all characters except those between \001 and
/// \\002 are assumed to be `visible'.
////////////////////////////////////////////////////////////////////////////////

        static char const * PROMPT_IGNORE_START;

////////////////////////////////////////////////////////////////////////////////
/// @brief ignore sequence used for prompt length calculation (end point)
///
/// This sequence must be used behind any non-visible characters in the prompt.
////////////////////////////////////////////////////////////////////////////////

        static char const * PROMPT_IGNORE_END;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoShell
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

        ArangoClient ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoShell
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief sets up the general and logging options
////////////////////////////////////////////////////////////////////////////////

        void setupGeneral (triagens::basics::ProgramOptionsDescription& description);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets up the color options
////////////////////////////////////////////////////////////////////////////////

        void setupColors (triagens::basics::ProgramOptionsDescription& description);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets up the auto-complete options
////////////////////////////////////////////////////////////////////////////////

        void setupAutoComplete (triagens::basics::ProgramOptionsDescription& description);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets up the pretty-printing options
////////////////////////////////////////////////////////////////////////////////

        void setupPrettyPrint (triagens::basics::ProgramOptionsDescription& description);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets up the pager options
////////////////////////////////////////////////////////////////////////////////

        void setupPager (triagens::basics::ProgramOptionsDescription& description);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets up the log options
////////////////////////////////////////////////////////////////////////////////

        void setupLog (triagens::basics::ProgramOptionsDescription& description);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets up the server options
////////////////////////////////////////////////////////////////////////////////

        void setupServer (triagens::basics::ProgramOptionsDescription& description);

////////////////////////////////////////////////////////////////////////////////
/// @brief parses command line and config file and prepares logging
////////////////////////////////////////////////////////////////////////////////

        void parse (triagens::basics::ProgramOptions&,
                    triagens::basics::ProgramOptionsDescription& description,
                    int argc,
                    char* argv[],
                    string const& initFilename);

////////////////////////////////////////////////////////////////////////////////
/// @brief print a string and a newline to stderr, necessary for Windows
////////////////////////////////////////////////////////////////////////////////

        void printErrLine (const std::string&);

////////////////////////////////////////////////////////////////////////////////
/// @brief print a string and a newline to stdout, necessary for Windows
////////////////////////////////////////////////////////////////////////////////

        void printLine (const std::string&);

////////////////////////////////////////////////////////////////////////////////
/// @brief print a string to stdout, without a newline, necessary for Windows
////////////////////////////////////////////////////////////////////////////////

        void printContinuous (const std::string&);

////////////////////////////////////////////////////////////////////////////////
/// @brief starts pager
////////////////////////////////////////////////////////////////////////////////

        void startPager ();

////////////////////////////////////////////////////////////////////////////////
/// @brief stops pager
////////////////////////////////////////////////////////////////////////////////

        void stopPager ();

////////////////////////////////////////////////////////////////////////////////
/// @brief print to pager
////////////////////////////////////////////////////////////////////////////////

        void internalPrint (const char* format, const char* str = 0);

////////////////////////////////////////////////////////////////////////////////
/// @brief open log
////////////////////////////////////////////////////////////////////////////////

        void openLog ();

////////////////////////////////////////////////////////////////////////////////
/// @brief close log
////////////////////////////////////////////////////////////////////////////////

        void closeLog ();

////////////////////////////////////////////////////////////////////////////////
/// @brief print info message
////////////////////////////////////////////////////////////////////////////////

        void printWelcomeInfo ();

////////////////////////////////////////////////////////////////////////////////
/// @brief print bye-bye
////////////////////////////////////////////////////////////////////////////////

        void printByeBye ();

////////////////////////////////////////////////////////////////////////////////
/// @brief log output, without prompt
////////////////////////////////////////////////////////////////////////////////

        void log (const char*, const char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief log output, with prompt
////////////////////////////////////////////////////////////////////////////////

        void log (const char*, const char*, const char*);

////////////////////////////////////////////////////////////////////////////////
/// @brief flush log
////////////////////////////////////////////////////////////////////////////////

        void flushLog ();

////////////////////////////////////////////////////////////////////////////////
/// @brief creates an new endpoint
////////////////////////////////////////////////////////////////////////////////

        void createEndpoint ();

////////////////////////////////////////////////////////////////////////////////
/// @brief creates an new endpoint
////////////////////////////////////////////////////////////////////////////////

        void createEndpoint (string const&);

////////////////////////////////////////////////////////////////////////////////
/// @brief quiet start
////////////////////////////////////////////////////////////////////////////////

        bool quiet () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief shut up arangosh
////////////////////////////////////////////////////////////////////////////////

        void shutup ();

////////////////////////////////////////////////////////////////////////////////
/// @brief deactivate colors
////////////////////////////////////////////////////////////////////////////////

        bool colors () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief gets the auto completion flag
////////////////////////////////////////////////////////////////////////////////

        bool autoComplete () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief use pretty print
////////////////////////////////////////////////////////////////////////////////

        bool prettyPrint () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief gets the output pager
////////////////////////////////////////////////////////////////////////////////

        string const& outputPager () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief gets use pager
////////////////////////////////////////////////////////////////////////////////

        bool usePager () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets use pager
////////////////////////////////////////////////////////////////////////////////

        void setUsePager (bool);

////////////////////////////////////////////////////////////////////////////////
/// @brief gets endpoint to connect to as string
////////////////////////////////////////////////////////////////////////////////

        string const& endpointString () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets endpoint to connect to as string
////////////////////////////////////////////////////////////////////////////////

        void setEndpointString (string const&);

////////////////////////////////////////////////////////////////////////////////
/// @brief endpoint
////////////////////////////////////////////////////////////////////////////////

        triagens::rest::Endpoint* endpointServer() const;

////////////////////////////////////////////////////////////////////////////////
/// @brief database name
////////////////////////////////////////////////////////////////////////////////

        string const& databaseName () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief set database name
////////////////////////////////////////////////////////////////////////////////

        void setDatabaseName (string const&);

////////////////////////////////////////////////////////////////////////////////
/// @brief user to send to endpoint
////////////////////////////////////////////////////////////////////////////////

        string const& username () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief password to send to endpoint
////////////////////////////////////////////////////////////////////////////////

        string const& password () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief set username
////////////////////////////////////////////////////////////////////////////////

        void setUsername (string const&);

////////////////////////////////////////////////////////////////////////////////
/// @brief set password
////////////////////////////////////////////////////////////////////////////////

        void setPassword (string const&);

////////////////////////////////////////////////////////////////////////////////
/// @brief connect timeout (in seconds)
////////////////////////////////////////////////////////////////////////////////

        double connectTimeout () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief request timeout (in seconds)
////////////////////////////////////////////////////////////////////////////////

        double requestTimeout () const;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoShell
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief configuration file
////////////////////////////////////////////////////////////////////////////////

        string _configFile;

////////////////////////////////////////////////////////////////////////////////
/// @brief temporary path
////////////////////////////////////////////////////////////////////////////////

        string _tempPath;

////////////////////////////////////////////////////////////////////////////////
/// @brief log level
////////////////////////////////////////////////////////////////////////////////

        string _logLevel;

////////////////////////////////////////////////////////////////////////////////
/// @brief quiet start
////////////////////////////////////////////////////////////////////////////////

        bool _quiet;

////////////////////////////////////////////////////////////////////////////////
/// @brief use color options
////////////////////////////////////////////////////////////////////////////////

        bool _colorOptions;

////////////////////////////////////////////////////////////////////////////////
/// @brief deactivate colors
////////////////////////////////////////////////////////////////////////////////

        bool _noColors;

////////////////////////////////////////////////////////////////////////////////
/// @brief use auto-complete options
////////////////////////////////////////////////////////////////////////////////

        bool _autoCompleteOptions;

////////////////////////////////////////////////////////////////////////////////
/// @brief disable auto completion
////////////////////////////////////////////////////////////////////////////////

        bool _noAutoComplete;

////////////////////////////////////////////////////////////////////////////////
/// @brief use pretty-print options
////////////////////////////////////////////////////////////////////////////////

        bool _prettyPrintOptions;

////////////////////////////////////////////////////////////////////////////////
/// @brief use pretty print
////////////////////////////////////////////////////////////////////////////////

        bool _prettyPrint;

////////////////////////////////////////////////////////////////////////////////
/// @brief use pager options
////////////////////////////////////////////////////////////////////////////////

        bool _pagerOptions;

////////////////////////////////////////////////////////////////////////////////
/// @brief the output pager
////////////////////////////////////////////////////////////////////////////////

        string _outputPager;

////////////////////////////////////////////////////////////////////////////////
/// @brief the pager FILE
////////////////////////////////////////////////////////////////////////////////

        FILE* _pager;

////////////////////////////////////////////////////////////////////////////////
/// @brief use pager
////////////////////////////////////////////////////////////////////////////////

        bool _usePager;

////////////////////////////////////////////////////////////////////////////////
/// @brief log filename
////////////////////////////////////////////////////////////////////////////////

        string _logFile;

////////////////////////////////////////////////////////////////////////////////
/// @brief the log FILE
////////////////////////////////////////////////////////////////////////////////

        FILE* _log;

////////////////////////////////////////////////////////////////////////////////
/// @brief use log options
////////////////////////////////////////////////////////////////////////////////

        bool _logOptions;

////////////////////////////////////////////////////////////////////////////////
/// @brief use server options
////////////////////////////////////////////////////////////////////////////////

        bool _serverOptions;

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not to turn off authentication
////////////////////////////////////////////////////////////////////////////////

        bool _disableAuthentication;

////////////////////////////////////////////////////////////////////////////////
/// @brief endpoint to connect to as string
////////////////////////////////////////////////////////////////////////////////

        string _endpointString;

////////////////////////////////////////////////////////////////////////////////
/// @brief endpoint
////////////////////////////////////////////////////////////////////////////////

        triagens::rest::Endpoint* _endpointServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief database name
////////////////////////////////////////////////////////////////////////////////

        string _databaseName;

////////////////////////////////////////////////////////////////////////////////
/// @brief user to send to endpoint
////////////////////////////////////////////////////////////////////////////////

        string _username;

////////////////////////////////////////////////////////////////////////////////
/// @brief password to send to endpoint
////////////////////////////////////////////////////////////////////////////////

        string _password;

////////////////////////////////////////////////////////////////////////////////
/// @brief whether or not a password was specified on the command line
////////////////////////////////////////////////////////////////////////////////

        bool _hasPassword;

////////////////////////////////////////////////////////////////////////////////
/// @brief connect timeout (in seconds)
////////////////////////////////////////////////////////////////////////////////

        double _connectTimeout;

////////////////////////////////////////////////////////////////////////////////
/// @brief request timeout (in seconds)
////////////////////////////////////////////////////////////////////////////////

        double _requestTimeout;
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
