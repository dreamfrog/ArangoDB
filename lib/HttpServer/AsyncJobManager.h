////////////////////////////////////////////////////////////////////////////////
/// @brief job manager
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
/// @author Copyright 2004-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_HTTP_SERVER_JOB_MANAGER_H
#define TRIAGENS_HTTP_SERVER_JOB_MANAGER_H 1

#include "Basics/Common.h"
#include "Basics/ReadLocker.h"
#include "Basics/WriteLocker.h"
#include "GeneralServer/GeneralServerJob.h"

using namespace std;
using namespace triagens::rest;

namespace triagens {
  namespace rest {

// -----------------------------------------------------------------------------
// --SECTION--                                              class AsyncJobResult
// -----------------------------------------------------------------------------

    struct AsyncJobResult {

// -----------------------------------------------------------------------------
// --SECTION--                                                      public types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup HttpServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief job statuses
////////////////////////////////////////////////////////////////////////////////

        typedef enum {
          JOB_UNDEFINED,
          JOB_PENDING,
          JOB_DONE
        }
        Status;

////////////////////////////////////////////////////////////////////////////////
/// @brief id typedef
////////////////////////////////////////////////////////////////////////////////
        
        typedef uint64_t IdType;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup HttpServer
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor for an unspecified job result
////////////////////////////////////////////////////////////////////////////////

        AsyncJobResult () : 
          _jobId(0),
          _response(0),
          _stamp(0.0),
          _status(JOB_UNDEFINED) {
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor for a specific job result
////////////////////////////////////////////////////////////////////////////////
        
        AsyncJobResult (IdType jobId, 
                        HttpResponse* response,
                        double stamp,
                        Status status) :
          _jobId(jobId),
          _response(response),
          _stamp(stamp),
          _status(status) {
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

        ~AsyncJobResult () {
        }

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup HttpServer
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief job id
////////////////////////////////////////////////////////////////////////////////

        IdType        _jobId;

////////////////////////////////////////////////////////////////////////////////
/// @brief the full HTTP response object of the job, can be 0
////////////////////////////////////////////////////////////////////////////////

        HttpResponse* _response;

////////////////////////////////////////////////////////////////////////////////
/// @brief job creation stamp
////////////////////////////////////////////////////////////////////////////////

        double        _stamp;

////////////////////////////////////////////////////////////////////////////////
/// @brief job status
////////////////////////////////////////////////////////////////////////////////

        Status        _status;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

    };

// -----------------------------------------------------------------------------
// --SECTION--                                             class AsyncJobManager
// -----------------------------------------------------------------------------

    class AsyncJobManager {

      private:
        AsyncJobManager (AsyncJobManager const&);
        AsyncJobManager& operator= (AsyncJobManager const&);

// -----------------------------------------------------------------------------
// --SECTION--                                        constructors / destructors 
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup HttpServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

        AsyncJobManager (uint64_t (*idFunc)()) 
          : _lock(),
            _jobs(),
            generate(idFunc) {
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

        ~AsyncJobManager () {
        }

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                      public types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup HttpServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief joblist typedef
////////////////////////////////////////////////////////////////////////////////
        
        typedef std::map<AsyncJobResult::IdType, AsyncJobResult> JobList;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup HttpServer
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief get the result of an async job, and remove it from the list
////////////////////////////////////////////////////////////////////////////////

        HttpResponse* getJobResult (AsyncJobResult::IdType jobId,
                                    AsyncJobResult::Status& status) { 
          WRITE_LOCKER(_lock);

          JobList::iterator it = _jobs.find(jobId); 

          if (it == _jobs.end()) {
            status = AsyncJobResult::JOB_UNDEFINED;
            return 0;
          }

          HttpResponse* response = (*it).second._response;
          status = (*it).second._status;

          // remove the job from the list
          _jobs.erase(it);
          return response;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief delete the result of an async job, without returning it
////////////////////////////////////////////////////////////////////////////////

        bool deleteJobResult (AsyncJobResult::IdType jobId) {
          WRITE_LOCKER(_lock);

          JobList::iterator it = _jobs.find(jobId); 

          if (it == _jobs.end()) {
            return false;
          }

          HttpResponse* response = (*it).second._response;

          if (response != 0) {
            delete response;
          }

          // remove the job from the list
          _jobs.erase(it);
          return true;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief delete all results
////////////////////////////////////////////////////////////////////////////////

        void deleteJobResults () {
          WRITE_LOCKER(_lock);

          JobList::iterator it = _jobs.begin();

          while (it != _jobs.end()) {
            HttpResponse* response = (*it).second._response;

            if (response != 0) {
              delete response;
            }

            ++it;
          }

          _jobs.clear();
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief delete expired results
////////////////////////////////////////////////////////////////////////////////

        void deleteExpiredJobResults (double stamp) {
          WRITE_LOCKER(_lock);

          JobList::iterator it = _jobs.begin();

          while (it != _jobs.end()) {
            AsyncJobResult ajr = (*it).second;

            if (ajr._stamp < stamp) {
              HttpResponse* response = ajr._response;

              if (response != 0) {
                delete response;
              }

              _jobs.erase(it++);
            }
            else {
              ++it;
            }
          }
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the list of pending jobs
////////////////////////////////////////////////////////////////////////////////

        const std::vector<AsyncJobResult::IdType> pending (size_t maxCount) { 
          return byStatus(AsyncJobResult::JOB_PENDING, maxCount);
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the list of done jobs
////////////////////////////////////////////////////////////////////////////////

        const std::vector<AsyncJobResult::IdType> done (size_t maxCount) { 
          return byStatus(AsyncJobResult::JOB_DONE, maxCount);
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief return the list of jobs by status
////////////////////////////////////////////////////////////////////////////////

        const std::vector<AsyncJobResult::IdType> byStatus (AsyncJobResult::Status status, 
                                                            size_t maxCount) {
          vector<AsyncJobResult::IdType> jobs;
          size_t n = 0;

          {
            READ_LOCKER(_lock);
            JobList::iterator it = _jobs.begin();

            // iterate the list. the list is sorted by id
            while (it != _jobs.end()) {
              AsyncJobResult::IdType jobId = (*it).first;

              if ((*it).second._status == status) {
                jobs.push_back(jobId);

                if (++n >= maxCount) {
                  break;
                }
              }

              ++it;
            }
          }

          return jobs;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief initialise an async job
////////////////////////////////////////////////////////////////////////////////

        template<typename S, typename HF>
        void initAsyncJob (GeneralServerJob<S, typename HF::GeneralHandler>* job, 
                           uint64_t* jobId) { 
          if (jobId == 0) {
            return;
          }

          *jobId = (AsyncJobResult::IdType) generate();
          job->assignId((uint64_t) *jobId);

          AsyncJobResult ajr(*jobId, 0, TRI_microtime(), AsyncJobResult::JOB_PENDING);

          WRITE_LOCKER(_lock);

          _jobs[*jobId] = ajr;
        }

////////////////////////////////////////////////////////////////////////////////
/// @brief finish the execution of an async job
////////////////////////////////////////////////////////////////////////////////

        template<typename S, typename HF>
        void finishAsyncJob (GeneralServerJob<S, typename HF::GeneralHandler>* job) { 
          assert(job != 0);
        
          typename HF::GeneralHandler* handler = job->getHandler();
          assert(handler != 0);

          AsyncJobResult::IdType jobId = job->id();

          if (jobId == 0) {
            return;
          }
            
          double now = TRI_microtime();

          WRITE_LOCKER(_lock);
          JobList::iterator it = _jobs.find(jobId); 
          
          if (it == _jobs.end()) {
            // job is already deleted.
            // do nothing here. the dispatcher will throw away the handler, which
            // will also dispose the response
          }
          else {
            (*it).second._response = handler->stealResponse();
            (*it).second._status = AsyncJobResult::JOB_DONE;
            (*it).second._stamp = now; 
          }
        }

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup HttpServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief lock to protect the _jobs map
////////////////////////////////////////////////////////////////////////////////
       
        basics::ReadWriteLock _lock;   

////////////////////////////////////////////////////////////////////////////////
/// @brief list of pending/done async jobs
////////////////////////////////////////////////////////////////////////////////
   
        JobList _jobs;     

////////////////////////////////////////////////////////////////////////////////
/// @brief function pointer for id generation
////////////////////////////////////////////////////////////////////////////////

        uint64_t (*generate)();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

    };

  }
}

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
