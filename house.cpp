#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::vector;


class House {
private:
    Member* owner;
    string start_date;
    string end_date;
    double consuming_points;
    float house_rating_score;
    string city; 
    string description;
    Member* occupier;
    float min_occupier_rating;
    float scores;
    string comments;

public: 
    //constructor
    House(Member* owner, string start_date, string end_date, double consuming_points,
    float house_rating_score, string city, string description, Member* occupier,
    float min_occupier_rating, float scores, string comments)
        : owner(owner), start_date(start_date), end_date(end_date), 
        consuming_points(consuming_points), house_rating_score(house_rating_score),
        city(city), description(description), 
        occupier(occupier), min_occupier_rating(min_occupier_rating),
        scores(scores), comments(comments)
        {}
    
    //Getters
    Member* getOwner(){return owner;}
    Member* getOccupier(){return occupier;}

};


