#pragma once

#include <cstdlib>
#include <vector>
#include <string>

#include "pugixml.hpp"


struct TCX {
    std::vector<double> LatitudeDegrees;
    std::vector<double> LongitudeDegrees;
    std::vector<double> AltitudeMeters;
    std::vector<double> DistanceMeters;


    void push_back(const double latitude, const double longitude, const double alt, const double dist) {
        LatitudeDegrees.push_back(latitude);
        LongitudeDegrees.push_back(longitude);
        AltitudeMeters.push_back(alt);
        DistanceMeters.push_back(dist);
    }


    std::size_t size() const { return DistanceMeters.size(); }
};


TCX parseString2TCX(const std::string &s) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(s.c_str());

    TCX out;

    if (!result)
        return out;

    pugi::xpath_node_set trackpoints = doc.select_nodes("TrainingCenterDatabase/Courses/Course/Track/Trackpoint");

    for (pugi::xpath_node node: trackpoints) {
        pugi::xml_node trackpoint = node.node();
        out.push_back(
            std::atof(trackpoint.child("Position").child_value("LatitudeDegrees")),
            std::atof(trackpoint.child("Position").child_value("LongitudeDegrees")),
            std::atof(trackpoint.child_value("AltitudeMeters")),
            std::atof(trackpoint.child_value("DistanceMeters"))
        );
    }

    return out;
}