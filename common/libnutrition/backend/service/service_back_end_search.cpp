/*
 * service_back_end_search.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 18, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/service/search_messages.pb.h"
#include <stdexcept>

QList<BackEnd::SearchResult> ServiceBackEnd::searchFoods(const BackEnd::SearchRequest& request)
{
  ::SearchRequest req;
  ::SearchResponse resp;

  req.set_searchterms(request.searchTerms.toStdString());

  for (QList<QSharedPointer<const Group> >::const_iterator i = request.excludeGroups.begin();
       i != request.excludeGroups.end(); ++i)
  {
    req.add_excludegroupids((*i)->getId().toStdString());
  }

  req.set_searchsinglefoods(request.searchSingleFoods);
  req.set_searchcomposites(request.searchComposites);
  req.set_searchtemplates(request.searchTemplates);

  req.set_sourceusda(request.sourceUSDA);
  req.set_sourceimport(request.sourceImport);
  req.set_sourceself(request.sourceSelf);
  req.set_sourceothers(request.sourceOthers);
  req.set_includeexpired(request.includeExpired);

  req.set_maxresults(request.maxResults);

  writeMessageAndReadResponse(req, resp);

  QList<BackEnd::SearchResult> results;

  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error(("Error during search: " + resp.error().errormessage()).c_str());
  } else {
    // It is guaranteed that the results will already be sorted by displayName

    for (int i = 0; i < resp.results_size(); ++i) {
      const ::SearchResult& result = resp.results(i);

      results.append(BackEnd::SearchResult
                     (QString::fromStdString(result.displayname()),
                      static_cast<FoodTypes::FoodType>(result.foodtype()),
                      result.id()));
    }
  }

  return results;
}
