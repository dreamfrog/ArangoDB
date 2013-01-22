////////////////////////////////////////////////////////////////////////////////
/// @brief tests for query language, simple queries
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
/// @author Jan Steemann
/// @author Copyright 2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

var jsunity = require("jsunity");
var ArangoError = require("org/arangodb/arango-error").ArangoError; 

////////////////////////////////////////////////////////////////////////////////
/// @brief test suite
////////////////////////////////////////////////////////////////////////////////

function ahuacatlQuerySimpleTestSuite () {

  ////////////////////////////////////////////////////////////////////////////////
/// @brief execute a given query
////////////////////////////////////////////////////////////////////////////////

  function executeQuery (query) {
    var cursor = AHUACATL_RUN(query, undefined);
    if (cursor instanceof ArangoError) {
      print(query, cursor.errorMessage);
    }
    assertFalse(cursor instanceof ArangoError);
    return cursor;
  }

////////////////////////////////////////////////////////////////////////////////
/// @brief execute a given query and return the results as an array
////////////////////////////////////////////////////////////////////////////////

  function getQueryResults (query) {
    var result = executeQuery(query).getRows();
    var results = [ ];

    for (var i in result) {
      if (!result.hasOwnProperty(i)) {
        continue;
      }

      results.push(result[i]);
    }

    return results;
  }


  return {

////////////////////////////////////////////////////////////////////////////////
/// @brief set up
////////////////////////////////////////////////////////////////////////////////

    setUp : function () {
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief tear down
////////////////////////////////////////////////////////////////////////////////

    tearDown : function () {
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return null
////////////////////////////////////////////////////////////////////////////////

    testReturnOnlyQuery1 : function () {
      var expected = [ null ];

      var actual = getQueryResults("return null");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a number
////////////////////////////////////////////////////////////////////////////////

    testReturnOnlyQuery2 : function () {
      var expected = [ 0 ];

      var actual = getQueryResults("return 0");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a string
////////////////////////////////////////////////////////////////////////////////

    testReturnOnlyQuery3 : function () {
      var expected = [ "a string value" ];

      var actual = getQueryResults("return \"a string value\"");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a list
////////////////////////////////////////////////////////////////////////////////

    testReturnOnlyQuery4 : function () {
      var expected = [ [ 1, 2, 9 ] ];

      var actual = getQueryResults("return [ 1, 2, 9 ]");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return an array
////////////////////////////////////////////////////////////////////////////////

    testReturnOnlyQuery5 : function () {
      var expected = [ { "name" : "Peter", "id" : 15, "age" : 35, "active" : false, "likes" : [ ] } ];

      var actual = getQueryResults("return { \"name\" : \"Peter\", \"id\" : 15, \"age\" : 35, \"active\" : false, \"likes\" : [ ] }");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a list of arrays
////////////////////////////////////////////////////////////////////////////////

    testReturnOnlyQuery6 : function () {
      var expected = [ [ { "name" : "Max", "id" : 1 }, { "name" : "Vanessa", "id" : 2 }, { "unrelated" : [ "some", "stuff", "goes", "here" ] } ] ];

      var actual = getQueryResults("return [ { \"name\" : \"Max\", \"id\" : 1 }, { \"name\" : \"Vanessa\", \"id\" : 2 }, { \"unrelated\" : [ \"some\", \"stuff\", \"goes\", \"here\" ] } ]");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a computed value
////////////////////////////////////////////////////////////////////////////////

    testReturnOnlyQuery7 : function () {
      var expected = [ 42 ];

      var actual = getQueryResults("return 1 + 40 + 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from let
////////////////////////////////////////////////////////////////////////////////

    testReturnLet1 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("let x = 1 return x");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from let
////////////////////////////////////////////////////////////////////////////////

    testReturnLet2 : function () {
      var expected = [ [ 1, 2, 42 ] ];

      var actual = getQueryResults("let x = [ 1, 2, 42 ] return x");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from let
////////////////////////////////////////////////////////////////////////////////

    testReturnLet3 : function () {
      var expected = [ null ];

      var actual = getQueryResults("let x = null return x");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from let
////////////////////////////////////////////////////////////////////////////////

    testReturnLet4 : function () {
      var expected = [ [ 1, 2, 3 ] ];

      var actual = getQueryResults("let x = (for u in [ 1, 2, 3 ] return u) return x");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from let
////////////////////////////////////////////////////////////////////////////////

    testReturnLet5 : function () {
      var expected = [ [ 2, 3, 4 ] ];

      var actual = getQueryResults("let x = (for u in (for v in [ 1, 2, 3 ] return v + 1) return u) return x");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from let
////////////////////////////////////////////////////////////////////////////////

    testReturnLet6 : function () {
      var expected = [ [ 1 ] ];

      var actual = getQueryResults("let x = (return 1) return x");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter1 : function () {
      var expected = [ ];

      var actual = getQueryResults("filter 1 == 0 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter2 : function () {
      var expected = [ ];

      var actual = getQueryResults("filter 1 == 1 filter 1 == 0 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter3 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("filter 1 == 1 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter4 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("filter 1 == 1 filter \"true\" == \"true\" return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter5 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let x = (filter 1 == 1 return 1) return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter6 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let x = (filter 0 == 1 return 1) return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter7 : function () {
      var expected = [ ];

      var actual = getQueryResults("let x = (filter 0 == 1 return 1) filter x == [ 1 ] return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter8 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let x = (filter 1 == 1 return 1) filter x == [ 1 ] return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter9 : function () {
      var expected = [ ];

      var actual = getQueryResults("filter 1 == 0 filter 2 == 3 return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter10 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 filter a > 1 && a < 0 filter a > 2 && a < 1 return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter11 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 filter a == 1 && a == 2 && a == 3 return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter12 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a == b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter13 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a != b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter14 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a >= b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter15 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a > b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter16 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a <= b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter17 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a < b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter18 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a >= b filter a <= b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter19 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a >= b filter a < b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter20 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a >= b filter a > b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter21 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a >= b filter a >= b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter22 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a >= b filter a == b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter23 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a > b filter a <= b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter24 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a > b filter a < b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter25 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a > b filter a > b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter26 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a > b filter a >= b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter27 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a > b filter a == b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter28 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a == b filter a <= b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter29 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a == b filter a < b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter30 : function () {
      var expected = [ 2 ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a == b filter a >= b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief return a value from a filter
////////////////////////////////////////////////////////////////////////////////

    testReturnFilter31 : function () {
      var expected = [ ];

      var actual = getQueryResults("let a = 1 let b = 1 filter a == b filter a > b return 2");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief sort and return
////////////////////////////////////////////////////////////////////////////////

    testReturnSort1 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("sort null return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief sort and return
////////////////////////////////////////////////////////////////////////////////

    testReturnSort2 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("sort [ 1, 2 ] return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief sort and return
////////////////////////////////////////////////////////////////////////////////

    testReturnSort3 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("sort rand() return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief sort and return
////////////////////////////////////////////////////////////////////////////////

    testReturnSort4 : function () {
      var expected = [ [ 3, 2, 1 ] ];

      var actual = getQueryResults("let a=(for x in [1,2,3] sort x desc return x) return a");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief limit and return
////////////////////////////////////////////////////////////////////////////////

    testReturnLimit1 : function () {
      var expected = [ ];

      var actual = getQueryResults("limit 0 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief limit and return
////////////////////////////////////////////////////////////////////////////////

    testReturnLimit2 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("limit 1 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief limit and return
////////////////////////////////////////////////////////////////////////////////

    testReturnLimit3 : function () {
      var expected = [ ];

      var actual = getQueryResults("limit 0, 0 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief limit and return
////////////////////////////////////////////////////////////////////////////////

    testReturnLimit4 : function () {
      var expected = [ ];

      var actual = getQueryResults("limit 1, 1 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief limit and return
////////////////////////////////////////////////////////////////////////////////

    testReturnLimit5 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("limit 0, 1 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief collect and return
////////////////////////////////////////////////////////////////////////////////

    testReturnCollect1 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("collect city = 1 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief collect and return
////////////////////////////////////////////////////////////////////////////////

    testReturnCollect2 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("collect city = 1 into g return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief collect and return
////////////////////////////////////////////////////////////////////////////////

    testReturnCollect3 : function () {
      var expected = [ 1 ];

      var actual = getQueryResults("collect city = 1, nonsense = 2 return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief comments
////////////////////////////////////////////////////////////////////////////////

    testComments1: function () {
      var expected = [ 1 ];

      var actual = getQueryResults("/* return 2 */ return 1");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief comments
////////////////////////////////////////////////////////////////////////////////

    testComments2: function () {
      var expected = [ [ 4 ] ];

      var actual = getQueryResults("/* return 1 */ return [/*1,2,3,*/4 /*5,6,7*/]");
      assertEqual(expected, actual);
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief comments
////////////////////////////////////////////////////////////////////////////////

    testComments2: function () {
      var expected = [ 1 ];

      var actual = getQueryResults("/* for u in [1,2,3] */ return 1");
      assertEqual(expected, actual);
    }

  };
}

////////////////////////////////////////////////////////////////////////////////
/// @brief executes the test suite
////////////////////////////////////////////////////////////////////////////////

jsunity.run(ahuacatlQuerySimpleTestSuite);

return jsunity.done();

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// @addtogroup\\|// --SECTION--\\|/// @page\\|/// @}\\)"
// End:
