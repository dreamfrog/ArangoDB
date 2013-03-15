/*jslint indent: 2, nomen: true, maxlen: 100, white: true  plusplus: true */
/*global beforeEach, afterEach */
/*global describe, it, expect */
/*global runs, spyOn, waitsFor */
/*global window, eb, loadFixtures, document */
/*global $, _, d3*/
/*global ArangoAdapter*/

////////////////////////////////////////////////////////////////////////////////
/// @brief Graph functionality
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2010-2012 triagens GmbH, Cologne, Germany
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
/// @author Michael Hackstein
/// @author Copyright 2011-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////


(function () {
  "use strict";

  describe('Arango Adapter', function () {
    
    var adapter,
      nodes,
      edges,
      arangodb,
      startNode,
      nodesCollection,
      nodesCollId,
      edgesCollection,
      edgesCollId,
      nodeWithID = function(id) {
        return $.grep(nodes, function(e){
          return e.id === id;
        })[0];
      },
      
      existNode = function(id) {
        var node = nodeWithID(id);
        expect(node).toBeDefined();
        expect(node.id).toEqual(id);
      },
      
      existNodes = function(ids) {
        _.each(ids, existNode);
      },
      
      createCollection = function(name, type, callback) {
        if (type !== "Document" && type !== "Edge") {
          throw "Unsupported type";
        }
        var data = {
          "name": name,
          "type": type,
          "journalSize": 1048576
        };
        $.ajax({
          cache: false,
          type: "POST",
          url: arangodb + "/_api/collection",
          data: JSON.stringify(data),
          contentType: "application/json",
          processData: false,
          async: false,
          success: function(data) {
            callback(data.id);
          },
          error: function(data) {
            throw data.statusText;
          }
        });
      },
      
      dropCollection = function(id) {
        $.ajax({
          cache: false,
          type: 'DELETE',
          url: arangodb + "/_api/collection/" + id,
          async: false,
          success: function (data) {
            
          },
          error: function (data) {
            throw data.statusText;
          }
        });
      },
      insertEdge = function (collectionID, from, to) {
        $.ajax({
          cache: false,
          type: "POST",
          async: false,
          url: arangodb + "/_api/edge?collection=" + collectionID + "&from=" + from + "&to=" + to,
          data: JSON.stringify({}),
          contentType: "application/json",
          processData: false,
          success: function(data) {
            return data._id;
          },
          error: function(data) {
            throw data.statusText;
          }
        });
      },
      insertNode = function (collectionID, nodeId) {
        var id;
        $.ajax({
          cache: false,
          type: "POST",
          async: false,
          url: arangodb + "/_api/document?collection=" + collectionID,
          data: JSON.stringify({id: nodeId}),
          contentType: "application/json",
          processData: false,
          success: function(data) {
            id = data._id;
          },
          error: function(data) {
            throw data.statusText;
          }
        });
        return id;
      },
      
      deleteArangoContent = function() {
        dropCollection(nodesCollection);
        dropCollection(edgesCollection);
      },
      
      setupArangoContent = function() {
        nodesCollection = "TestNodes321";
        edgesCollection = "TestEdges321";
        
        deleteArangoContent();
        
        createCollection(nodesCollection, "Document", function(id) {nodesCollId = id;});
        createCollection(edgesCollection, "Edge", function(id) {edgesCollId = id;});
        
        var c1 = insertNode(nodesCollection, 477),
        c2 = insertNode(nodesCollection, 29),
        c3 = insertNode(nodesCollection, 159),
        c4 = insertNode(nodesCollection, 213),
        c5 = insertNode(nodesCollection, 339),
        c6 = insertNode(nodesCollection, 3),
        c7 = insertNode(nodesCollection, 7),
        c8 = insertNode(nodesCollection, 4),
        c9 = insertNode(nodesCollection, 80);
        
        insertEdge(edgesCollection, c1, c2);
        insertEdge(edgesCollection, c1, c3);
        insertEdge(edgesCollection, c1, c4);
        insertEdge(edgesCollection, c1, c5);
        insertEdge(edgesCollection, c2, c6);
        insertEdge(edgesCollection, c2, c7);
        insertEdge(edgesCollection, c6, c8);
        insertEdge(edgesCollection, c4, c8);
        insertEdge(edgesCollection, c4, c9);
      };
    
    beforeEach(function() {
      
      arangodb = "http://localhost:8529";
      setupArangoContent();
      nodes = [];
      edges = [];
      startNode = 477;
      adapter = new ArangoAdapter(arangodb, nodes, edges, nodesCollection, edgesCollection, 100, 40);
    });
    
    afterEach(function() {
      
    });
    
    it('should be able to load a tree node from ArangoDB', function() {
      var callbackCheck;
      
      runs(function() {
        callbackCheck = false;
        adapter.loadNodeFromTree(startNode, function(res) {
          callbackCheck = true;
        });
      });
      
      waitsFor(function() {
        return callbackCheck;
      });
      
      runs(function() {
        existNodes([477, 29, 159, 213, 339]);
        expect(nodes.length).toEqual(5);
      });
    });
    
    it('should be able to request the number of children centrality', function() {
      var callbackCheck,
      children;
      
      runs(function() {
        callbackCheck = false;
        adapter.requestCentralityChildren(startNode, function(count) {
          callbackCheck = true;
          children = count;
        });
      });
      
      waitsFor(function() {
        return callbackCheck;
      });
      
      runs(function() {
        expect(children).toEqual(4);
      });
    });
    
    describe('that has already loaded one graph', function() {
      
      beforeEach(function() {
        var callbackCheck;
      
        runs(function() {
          callbackCheck = false;
          adapter.loadNodeFromTree(startNode, function() {
            callbackCheck = true;
          });
        });
      
        waitsFor(function() {
          return callbackCheck;
        });        
      });
      
      it('should be able to add nodes from another query', function() {
        var callbackCheck;
        
        runs(function() {
          callbackCheck = false;
          adapter.loadNodeFromTree(29, function() {
            callbackCheck = true;
          });
        });
      
        waitsFor(function() {
          return callbackCheck;
        });
      
        runs(function() {
          existNodes([477, 29, 159, 213, 339, 3, 7]);
          expect(nodes.length).toEqual(7);
        });
      });
      
      it('should be able to change a value of one node permanently', function() {
        throw "Not Implemented";
      });
      
      it('should be able to change a value of one edge permanently', function() {
        throw "Not Implemented";
      });
      
      it('should be able to remove a node permanently', function() {
        throw "Not Implemented";
      });
      
      it('should be able to remove an edge permanently', function() {
        throw "Not Implemented";
      });
      
      it('should be able to add a node permanently', function() {
        throw "Not Implemented";  
      });
      
      it('should be able to add an edge permanently', function() {
        throw "Not Implemented";
      });
    });
    
    describe('that has loaded several queries', function() {
      beforeEach(function() {
        var callbackCheck;
      
        runs(function() {
          callbackCheck = false;
          adapter.loadNodeFromTree(startNode, function() {
            callbackCheck = true;
          });
        });
      
        waitsFor(function() {
          return callbackCheck;
        });
      
        runs(function() {
          callbackCheck = false;
          adapter.loadNodeFromTree(29, function() {
            callbackCheck = true;
          });
        });
      
        waitsFor(function() {
          return callbackCheck;
        });
      
        runs(function() {
          callbackCheck = false;
          adapter.loadNodeFromTree(3, function() {
            callbackCheck = true;
          });
        });
      
        waitsFor(function() {
          return callbackCheck;
        });
        
        it('should not add a node to the list twice', function() {
          var callbackCheck;
      
          runs(function() {
            callbackCheck = false;
            adapter.loadNodeFromTree(213, function() {
              callbackCheck = true;
            });
          });
      
          waitsFor(function() {
            return callbackCheck;
          });  
          
          runs(function() {
            existNodes([477, 29, 159, 213, 339, 3, 7, 4, 80]);
            expect(nodes.length).toEqual(9);
          });
        });
      });
    });
    
  });
  
}());