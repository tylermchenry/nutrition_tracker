#include "servers/search_server.h"

namespace SearchServer
{
  SearchResultListing searchFoods(const SearchRequest& req, int loggedInUserId)
  {
    BackEnd::SearchRequest be_req;
    SearchResultListing resultListing;

    be_req.searchTerms = QString::fromStdString(req.searchterms());

    for (int i = 0; i < req.excludegroupids_size(); ++i) {
      be_req.excludeGroups.append
        (Group::getGroup(QString::fromStdString(req.excludegroupids(i))));
    }

    be_req.searchSingleFoods = req.searchsinglefoods();
    be_req.searchComposites = req.searchcomposites();
    be_req.searchTemplates = req.searchtemplates();

    be_req.sourceUSDA = req.sourceusda();
    be_req.sourceImport = req.sourceimport();
    be_req.sourceSelf = req.sourceself();
    be_req.sourceOthers = req.sourceothers();
    be_req.includeExpired = req.includeexpired();

    be_req.maxResults = req.maxresults();

    be_req.userId = loggedInUserId;

    resultListing.addObjects(BackEnd::getBackEnd()->searchFoods(be_req));

    return resultListing;
  }
}
