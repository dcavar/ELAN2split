/*
 * Created by Damir Cavar on 7/28/15.
 *
 * Part of the elan2split project.
 *
 * Xerces-C++ handler for SAX2 parser events.
 *
 * (C) 2015 by Damir Cavar <damir@linguistlist.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef TUTORIAL1_MYSAX2HANDLER_H
#define TUTORIAL1_MYSAX2HANDLER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <unordered_map>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>

#include "Interval.h"


enum Tags {
    ALIGNABLE_ANNOTATION = 1,
    ANNOTATION,
    ANNOTATION_ID,
    ANNOTATION_REF,
    ANNOTATION_VALUE,
    HEADER,
    LINGUISTIC_TYPE_REF,
    MEDIA_DESCRIPTOR,
    PARTICIPANT,
    REF_ANNOTATION,
    RELATIVE_MEDIA_URL,
    TIER,
    TIER_ANNOTATOR,
    TIER_ID,
    TIME_ORDER,
    TIME_SLOT,
    TIME_SLOT_ID,
    TIME_SLOT_REF1,
    TIME_SLOT_REF2,
    TIME_UNITS,
    TIME_VALUE
};

using namespace std;
using namespace xercesc;
namespace fs = boost::filesystem;


class MySAX2Handler : public DefaultHandler {

public:

    void startElement(
            const XMLCh *const uri,
            const XMLCh *const localname,
            const XMLCh *const qname,
            const Attributes &attrs
    );

    void endElement(
            const XMLCh *const uri,
            const XMLCh *const localname,
            const XMLCh *const qname);

    void characters(const XMLCh *const chars, const XMLSize_t length);

    void fatalError(const SAXParseException &);

    // a map for time slot IDs and time values
    unordered_map<string, int> time_slots;
    // store map for referenced time slot IDs and time values in dependent tiers
    unordered_map<string, pair<string,string>> ref_slots;

    // time units (e.g. milliseconds or other)
    string time_units;

    // relative media URL
    vector<string> rel_media_urls;

    // initializing a hash-map
    unordered_map<string, int> myTags_{
            {"ALIGNABLE_ANNOTATION", ALIGNABLE_ANNOTATION},
            {"ANNOTATION",           ANNOTATION},
            {"ANNOTATION_ID",        ANNOTATION_ID},
            {"ANNOTATION_REF",       ANNOTATION_REF},
            {"ANNOTATION_VALUE",     ANNOTATION_VALUE},
            {"ANNOTATOR",            TIER_ANNOTATOR},
            {"HEADER",               HEADER},
            {"LINGUISTIC_TYPE_REF",  LINGUISTIC_TYPE_REF},
            {"MEDIA_DESCRIPTOR",     MEDIA_DESCRIPTOR},
            {"PARTICIPANT",          PARTICIPANT},
            {"REF_ANNOTATION",       REF_ANNOTATION},
            {"RELATIVE_MEDIA_URL",   RELATIVE_MEDIA_URL},
            {"TIER",                 TIER},
            {"TIER_ID",              TIER_ID},
            {"TIME_ORDER",           TIME_ORDER},
            {"TIME_SLOT",            TIME_SLOT},
            {"TIME_SLOT_ID",         TIME_SLOT_ID},
            {"TIME_SLOT_REF1",       TIME_SLOT_REF1},
            {"TIME_SLOT_REF2",       TIME_SLOT_REF2},
            {"TIME_UNITS",           TIME_UNITS},
            {"TIME_VALUE",           TIME_VALUE}
    };

    // file path
    string file_path;

    // quite mode on/off
    bool quiet = false;
    // add tier name to text-output file names
    bool add_tier_name = false;

    // tier selected is on when the parsed tier is selected,
    // either the first one, or the one named via command line
    bool tier_selected = false;
    // state flag for processing ELAN file
    bool parse_tier_annotation = false;
    // another state flag
    bool collect_annotation_value = false;
    // force overwrite of existing files
    bool force_overwrite = false;

    // tier name to be processed
    string target_tier_name;

    // temporary Interval
    Interval myInt;

    // stringbuf for characters from tag-values
    string buffer;
    // output folder
    string output_folder;
    // text file suffix
    string text_suffix;

    MySAX2Handler();
};

#endif //TUTORIAL1_MYSAX2HANDLER_H
