#ifndef SEARCH_SERVER_H_
#define SEARCH_SERVER_H_

#include "libnutrition/proto/service/search_messages.pb.h"
#include "libnutrition/backend/back_end.h"
#include "servers/listing.h"
#include <QString>
#include <QPair>

class SearchResultListing
  : public Listing<BackEnd::SearchResult, SearchResponse,
                    QPair<int, BackEnd::FoodTypes::FoodType>,
                    BackEnd::SearchResult>
{
  protected:

    virtual bool isValid
      (const BackEnd::SearchResult&) const
        { return true; }

    virtual QPair<int, BackEnd::FoodTypes::FoodType> getId
      (const BackEnd::SearchResult& result) const
        { return qMakePair(result.id, result.foodType); }

    virtual QString getName
      (const BackEnd::SearchResult& result) const
        { return result.displayName; }

    virtual void addListingToResponse
      (SearchResponse& resp, const QPair<int, BackEnd::FoodTypes::FoodType>& id,
       const QString& name) const
    {
      SearchResult* result = resp.add_results();
      result->set_displayname(name.toStdString());
      result->set_foodtype(static_cast<SearchResult::FoodType>(id.second));
      result->set_id(id.first);
    }
};

namespace SearchServer
{
  SearchResultListing searchFoods(const SearchRequest& req);
}

#endif /* SEARCH_SERVER_H_ */
