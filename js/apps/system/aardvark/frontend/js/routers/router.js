/*jslint indent: 2, nomen: true, maxlen: 100, sloppy: true, vars: true, white: true, plusplus: true, newcap: true */
/*global window, $, Backbone, document, arangoCollection,arangoHelper,dashboardView,arangoDatabase*/

$(document).ready(function() {

  window.Router = Backbone.Router.extend({
    routes: {
      ""                                    : "dashboard",
      "collection/:colid"                   : "collection",
      "collections"                         : "collections",
      "collectionInfo/:colid"               : "collectionInfo",
      "new"                                 : "newCollection",
      "login"                               : "login",
      "collection/:colid/documents/:pageid" : "documents",
      "collection/:colid/:docid"            : "document",
      "collection/:colid/:docid/source"     : "source",
      "shell"                               : "shell",
      "query"                               : "query",
      "logs"                                : "logs",
      "about"                               : "about",
      "api"                                 : "api",
      "databases"                           : "databases",
      "application/installed/:key"          : "applicationEdit",
      "application/available/:key"          : "applicationInstall",
      "applications/installed"              : "applicationsInstalled",
      "applications/available"              : "applicationsAvailable",
      "applications"                        : "applications",
      "application/documentation/:key"      : "appDocumentation",
      "graph"                               : "graph"
    },

    initialize: function () {
      window.activeSession = new window.ArangoSession();

      window.arangoDatabase = new window.ArangoDatabase();

      window.arangoCollectionsStore = new window.arangoCollections();
      window.arangoDocumentsStore = new window.arangoDocuments();
      window.arangoDocumentStore = new window.arangoDocument();

      window.collectionsView = new window.collectionsView({
        collection: window.arangoCollectionsStore
      });
      window.arangoCollectionsStore.fetch();

      window.collectionView = new window.collectionView({
        model: arangoCollection
      });

      window.collectionInfoView = new window.collectionInfoView({
        model: arangoCollection
      });

      window.documentsView = new window.documentsView({
        collection: window.arangoDocuments
      });
      window.documentView = new window.documentView({
        collection: window.arangoDocument
      });
      window.documentSourceView = new window.documentSourceView({
        collection: window.arangoDocument
      });

      window.arangoLogsStore = new window.arangoLogs();
      window.arangoLogsStore.fetch({
        success: function () {
          window.logsView = new window.logsView({
            collection: window.arangoLogsStore
          });
        }
      });

      this.footerView = new window.footerView();
      this.naviView = new window.navigationView();
      this.footerView.render();
      this.naviView.render();
      this.graphView = new window.graphView({
        collection: window.arangoCollectionsStore
      });


      var self = this;
      $(window).resize(function() {
        self.handleResize();
      });
      this.handleResize();
    },

    logsAllowed: function () {
      return (window.databaseName === '_system');
    },

    checkSession: function () {
      if (window.activeSession.models.length === 0) {
        window.App.navigate("login", {trigger: true});
        return false;
      }
      return true;
    },
    login: function () {
      if (!this.loginView) {
        this.loginView = new window.loginView({
          collection: window.activeSession
        });
      }
      this.loginView.render();
      this.naviView.selectMenuItem('');
    },
    collections: function() {
      var naviView = this.naviView;

      //DEV
      //var currentSession = this.checkSession();
      //if (currentSession === false) {
      //  return;
      //}

      window.arangoCollectionsStore.fetch({
        success: function () {
          window.collectionsView.render();
          naviView.selectMenuItem('collections-menu');
        }
      });
    },

    collection: function(colid) {
      window.collectionView.options.colId = colid;
      window.collectionView.render();
    },
    collectionInfo: function(colid) {
      window.collectionInfoView.options.colId = colid;
      window.collectionInfoView.render();
    },
    newCollection: function() {
      if (!this.newCollectionView) {
        this.newCollectionView = new window.newCollectionView({});
      }
      this.newCollectionView.render();
    },

    documents: function(colid, pageid) {
      if (!window.documentsView) {
        window.documentsView.initTable(colid, pageid);
      }
      window.documentsView.collectionID = colid;
      var type = arangoHelper.collectionApiType(colid);
      window.documentsView.colid = colid;
      window.documentsView.pageid = pageid;
      window.documentsView.type = type;
      window.documentsView.render();
      window.arangoDocumentsStore.getDocuments(colid, pageid);
    },

    document: function(colid, docid) {
      if (!window.documentView) {
        window.documentView.initTable();
      }
      window.documentView.colid = colid;
      window.documentView.docid = docid;
      window.documentView.render();
      var type = arangoHelper.collectionApiType(colid);
      window.documentView.type = type;
      window.documentView.typeCheck(type);
    },

    source: function(colid, docid) {
      window.documentSourceView.render();
      window.documentSourceView.colid = colid;
      window.documentSourceView.docid = docid;
      var type = arangoHelper.collectionApiType(colid);
      window.documentSourceView.type = type;
      window.documentSourceView.typeCheck(type);
    },

    shell: function() {
      if (!this.shellView) {
        this.shellView = new window.shellView();
      }
      this.shellView.render();
      this.naviView.selectMenuItem('shell-menu');
    },

    query: function() {
      if (!this.queryView) {
        this.queryView = new window.queryView();
      }
      this.queryView.render();
      this.naviView.selectMenuItem('query-menu');
    },

    api: function() {
      if (!this.apiView) {
        this.apiView = new window.apiView();
      }
      this.apiView.render();
      this.naviView.selectMenuItem('api-menu');
    },

    databases: function() {
      if (arangoHelper.databaseAllowed() === true) {
        if (!this.databaseView) {
          this.databaseView = new window.databaseView({
            collection: arangoDatabase
          });
        }
        this.databaseView.render();
        this.naviView.selectMenuItem('databases-menu');
      }
      else {
        window.App.navigate("#", {trigger: true});
        this.naviView.selectMenuItem('dashboard-menu');
        $('#databaseNavi').css('display','none');
        $('#databaseNaviSelect').css('display','none');
      }
    },

    about: function() {
      if (!this.aboutView) {
        this.aboutView = new window.aboutView();
      }
      this.aboutView.render();
      this.naviView.selectMenuItem('about-menu');
    },

    logs: function() {
      if (! this.logsAllowed()) {
        this.navigate('', { trigger: true });
        return;
      }

      var self = this;
      window.arangoLogsStore.fetch({
        success: function () {
          window.logsView.render();
          $('#logNav a[href="#all"]').tab('show');
          window.logsView.initLogTables();
          window.logsView.drawTable();
          $('#all-switch').click();
        }
      });
      this.naviView.selectMenuItem('logs-menu');
    },

    dashboard: function() {
      this.naviView.selectMenuItem('dashboard-menu');
      if (this.statisticsDescription === undefined) {
        this.statisticsDescription = new window.StatisticsDescription();
        this.statisticsDescription.fetch({
          async:false
        });
      }
      if (this.statistics === undefined) {
        this.statisticsCollection = new window.StatisticsCollection();
      }
      if (this.dashboardView === undefined) {
        this.dashboardView = new dashboardView({
          collection: this.statisticsCollection,
          description: this.statisticsDescription
        });
         }
        this.dashboardView.render();
    },

    graph: function() {
      var self = this;
      window.arangoCollectionsStore.fetch({
        success: function () {
          self.graphView.render();
          self.naviView.selectMenuItem('graphviewer-menu');
        }
      });
    },

    applications: function() {
      if (this.foxxList === undefined) {
        this.foxxList = new window.FoxxCollection();
      }
      if (this.applicationsView === undefined) {
        this.applicationsView = new window.ApplicationsView({
          collection: this.foxxList
        });
      }
      this.applicationsView.reload();
      this.naviView.selectMenuItem('applications-menu');
    },

    applicationsAvailable: function() {
      if (this.foxxList === undefined) {
        this.foxxList = new window.FoxxCollection();
      }
      if (this.applicationsInstalledView === undefined) {
        this.applicationsInstalledView = new window.FoxxInstalledListView({
          collection: this.foxxList
        });
      }
      this.applicationsInstalledView.reload();
      this.naviView.selectMenuItem('applications-menu');
    },

    applicationsInstalled: function() {
      if (this.foxxList === undefined) {
        this.foxxList = new window.FoxxCollection();
      }
      if (this.applicationsActiveView === undefined) {
        this.applicationsActiveView = new window.FoxxActiveListView({
          collection: this.foxxList
        });
      }
      this.applicationsActiveView.reload();
      this.naviView.selectMenuItem('applications-menu');
    },

    applicationEdit: function(appkey) {
      if (this.foxxList === undefined) {
        var self = this;
        this.foxxList = new window.FoxxCollection();
        this.foxxList.fetch({
          success: function() {
            var editAppView = new window.foxxEditView({
              model: self.foxxList.findWhere({_key: appkey})
            });
            editAppView.render();
          }
        });
      } else {
        var editAppView = new window.foxxEditView({model: this.foxxList.findWhere({_key: appkey})});
        editAppView.render();
      }
    },

    applicationInstall: function(appkey) {
      if (this.foxxList === undefined) {
        var self = this;
        this.foxxList = new window.FoxxCollection();
        this.foxxList.fetch({
          success: function() {
            var installAppView = new window.foxxMountView({
              model: self.foxxList.findWhere({_key: appkey})
            });
            installAppView.render();
          }
        });
      } else {
        var installAppView = new window.foxxMountView({
          model: this.foxxList.findWhere({_key: appkey})
        });
        installAppView.render();
      }

    },

    appDocumentation: function(key) {
      var docuView = new window.AppDocumentationView({key: key});
      docuView.render();
      this.naviView.selectMenuItem('applications-menu');
    },

    handleSelectDatabase: function () {
      this.naviView.handleSelectDatabase(); 
    },

    handleResize: function () {
      var oldWidth = $('#content').width();
      var containerWidth = $(window).width() - 70;
      /*var spanWidth = 242;*/
      var spanWidth = 243;
      var divider = containerWidth / spanWidth;
      var roundDiv = parseInt(divider, 10);
      var newWidth = roundDiv*spanWidth -2;
      var marginWidth = ((containerWidth+30) - newWidth)/2;
      this.footerView.handleResize(marginWidth + 20);
      this.naviView.handleResize(marginWidth);
      $('#content').width(newWidth)
      .css('margin-left', marginWidth)
      .css('margin-right', marginWidth);
      // $('.footer-right p').css('margin-right', marginWidth + 20);
      // $('.footer-left p').css('margin-left', marginWidth + 20);
      if (newWidth !== oldWidth && window.App) {
        window.App.graphView.handleResize(newWidth);
      }
    }

  });

  window.App = new window.Router();
  Backbone.history.start();
  window.App.handleResize();

});

