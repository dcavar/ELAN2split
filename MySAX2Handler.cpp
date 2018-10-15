/*
 * Created by Damir Cavar on 7/28/15.
 *
 * (C) 2015, 2016 by Damir Cavar <damir@linguistlist.org>
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

//#include <sox.h>
#include "MySAX2Handler.h"


using namespace std;
using namespace xercesc;


MySAX2Handler::MySAX2Handler() {
    text_suffix = ".txt";
}


void MySAX2Handler::startElement(
        const XMLCh *const uri,
        const XMLCh *const localname,
        const XMLCh *const qname,
        const Attributes &attrs) {

    char *message = XMLString::transcode(localname);
    string tag_name = message;
    XMLString::release(&message);

    int attribute_name;
    int tag = myTags_[tag_name];
    char *temp;

    switch (tag) {
        case TIME_SLOT: {
            string tid;
            int time;
            for (int i = 0; i < attrs.getLength(); i++) {
                temp = XMLString::transcode(attrs.getLocalName(i));
                attribute_name = myTags_[temp];
                XMLString::release(&temp);
                temp = XMLString::transcode(attrs.getValue(i));
                if (attribute_name == TIME_SLOT_ID) {
                    tid = temp;
                } else if (attribute_name == TIME_VALUE) {
                    time = atoi(temp);
                }
                XMLString::release(&temp);
            }
            time_slots[tid] = time;
            break;
        }
        case MEDIA_DESCRIPTOR: {
            for (int i = 0; i < attrs.getLength(); i++) {
                temp = XMLString::transcode(attrs.getLocalName(i));
                attribute_name = myTags_[temp];
                XMLString::release(&temp);
                temp = XMLString::transcode(attrs.getValue(i));
                if (attribute_name == RELATIVE_MEDIA_URL) {
                    fs::path fp(file_path);
                    fs::path file_temp(temp);
                    fp = fs::canonical(fp / file_temp);
                    if (fs::is_regular_file(fp)) {
                        rel_media_urls.push_back(fp.string());
                        if (!quiet)
                            cout << "Using media file: " << rel_media_urls[0] << endl;
                    } else {
                        cerr << "Media file not fount: " << fp << endl;
                        // does not get to the correct exception handler!!!
                        throw invalid_argument("Media file " + fp.string() + " cannot be found.");
                    }
                }
                XMLString::release(&temp);
            }
            break;
        }
        case HEADER: {
            // initial tag for new header: start collecting new time_slots, check for attribute MEDIA_FILE
            for (int i = 0; i < attrs.getLength(); i++) {
                temp = XMLString::transcode(attrs.getLocalName(i));
                attribute_name = myTags_[temp];
                XMLString::release(&temp);
                temp = XMLString::transcode(attrs.getValue(i));
                if (attribute_name == TIME_UNITS) {
                    time_units = temp;
                    if (!quiet)
                        cout << "Setting time units of intervals to: " << time_units << endl;
                }
                XMLString::release(&temp);
            }
            break;
        }
        case ANNOTATION:
            break;
        case REF_ANNOTATION: {
            // get the reference for ANNOTATION_REF
            if (parse_tier_annotation) {
                for (int i = 0; i < attrs.getLength(); i++) {
                    temp = XMLString::transcode(attrs.getLocalName(i));
                    attribute_name = myTags_[temp];
                    XMLString::release(&temp);
                    temp = XMLString::transcode(attrs.getValue(i));
                    if (attribute_name == ANNOTATION_ID) {
                        myInt.id = temp;
                    } else if (attribute_name == ANNOTATION_REF) { // get the labels of the time boundaries
                        pair<string, string> idtuple = ref_slots[temp];
                        myInt.start_label = idtuple.first;
                        myInt.start = time_slots[idtuple.first];
                        myInt.stop_label = idtuple.second;
                        myInt.stop = time_slots[idtuple.second];
                    }
                    XMLString::release(&temp);
                }
            }
            break;
        }
        case ALIGNABLE_ANNOTATION: {
            for (int i = 0; i < attrs.getLength(); i++) {
                temp = XMLString::transcode(attrs.getLocalName(i));
                attribute_name = myTags_[temp];
                XMLString::release(&temp);
                temp = XMLString::transcode(attrs.getValue(i));
                if (attribute_name == ANNOTATION_ID) {
                    myInt.id = temp;
                } else if (attribute_name == TIME_SLOT_REF1) {
                    myInt.start_label = temp;
                    myInt.start = time_slots[temp];
                } else if (attribute_name == TIME_SLOT_REF2) {
                    myInt.stop_label = temp;
                    myInt.stop = time_slots[temp];
                    ref_slots[myInt.id] = make_pair(myInt.start_label, myInt.stop_label);
                }
                XMLString::release(&temp);
            }
            break;
        }
        case ANNOTATION_VALUE: {
            if (parse_tier_annotation) {
                collect_annotation_value = true;
            }
            break;
        }
        case TIER: {
            // flag the first TIER, if no parameter given for which tier to segment, use this one
            // else skip and wait for the right tier name to come
            if (!tier_selected) {
                for (int i = 0; i < attrs.getLength(); i++) {
                    temp = XMLString::transcode(attrs.getLocalName(i));
                    attribute_name = myTags_[temp];
                    XMLString::release(&temp);
                    temp = XMLString::transcode(attrs.getValue(i));
                    if (attribute_name == TIER_ID) {
                        if (target_tier_name.size() > 0) {
                            if (target_tier_name.compare(temp) == 0) {
                                tier_selected = true;
                                if (!quiet)
                                    cout << "Processing tier: " << temp << endl;
                                parse_tier_annotation = true;
                            }
                        } else {
                            tier_selected = true;
                            if (!quiet)
                                cout << "Processing tier: " << temp << endl;
                            parse_tier_annotation = true;
                        }
                    }
                    XMLString::release(&temp);
                }
            }
            break;
        }
    }
} // MySAX2Handler::startElement


void MySAX2Handler::endElement(
        const XMLCh *const uri,
        const XMLCh *const localname,
        const XMLCh *const qname) {

    char *message = XMLString::transcode(localname);

    int tag = myTags_[message];
    switch (tag) {
        case HEADER: { // this is the closing tag for header, start collecting new time_slots
            break;
        }
        case TIME_ORDER: // this is the end of the time slot definition
            if (!quiet)
                cout << "Parsed " << time_slots.size() << " time slots." << endl;
            break;
        case TIER: // tier closed, stop parsing, if active
            parse_tier_annotation = false;
            break;
        case ANNOTATION_VALUE: {
            if (parse_tier_annotation) {
                myInt.text = buffer;
                buffer.clear();
                collect_annotation_value = false;
            }
            break;
        }
        case ANNOTATION: {
            if (myInt.text.size() > 0) {
                ostringstream convert;   // stream used for the conversion
                convert << myInt.id << "_" << myInt.start << "_" << myInt.stop; // << ".wav";
                string ofname = convert.str();
                convert.str("");
                convert.clear();
                // wav file is only segment id and time interval
                fs::path owavfile = fs::path(output_folder) / fs::path(ofname + ".wav");
                // additionally append the tier name to the text file
                fs::path otextfile;
                if (add_tier_name)
                    otextfile = fs::path(output_folder) / fs::path(ofname + "_" + target_tier_name + text_suffix);
                else
                    otextfile = fs::path(output_folder) / fs::path(ofname + text_suffix);
                ofname = owavfile.string();

                convert << "sox \"" << rel_media_urls[0] << "\" \"" << ofname
                << "\" trim " << (float) myInt.start / 1000.0 << " "
                << ((float) myInt.stop - (float) myInt.start) / 1000.0;
                string command = convert.str();
                convert.str("");
                convert.clear();

                // create wav file
                if (fs::exists(owavfile) && !force_overwrite) {
                    cerr << "Will not overwrite " << owavfile.string() << endl;
                } else {
                    if (!quiet)
                        cout << "Calling: " << command << endl;
                    system(command.c_str());
                }
                // create transcript file
                if (fs::exists(otextfile) && !force_overwrite) {
                    cerr << "Will not overwrite " << otextfile.string() << endl;
                } else {
                    if (!quiet)
                        cout << "Creating: " << otextfile.string() << endl;
                    ofstream myfile;
                    myfile.open(otextfile.string());
                    myfile << myInt.text << endl;
                    myfile.close();
                }

                myInt.stop = 0;
                myInt.start = 0;
                myInt.text.clear();
                myInt.stop_label.clear();
                myInt.start_label.clear();
            }
            break;
        }
    }
    XMLString::release(&message);
} // MySAX2Handler::endElement


void MySAX2Handler::characters( const XMLCh *const chars, const XMLSize_t length) {
    if (collect_annotation_value) {
        char *temp = XMLString::transcode(chars);
        buffer.append(temp);
        XMLString::release(&temp);
    }
}


void MySAX2Handler::fatalError(const SAXParseException &exception) {
    char *message = XMLString::transcode(exception.getMessage());
    cerr << "Fatal Error: " << message << " at line: " << exception.getLineNumber() << endl;
    XMLString::release(&message);
} // MySAX2Handler::fatalError
