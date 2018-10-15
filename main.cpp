/*
 * This is a small tutorial code example...
 *
 * Author: Damir Cavar
 * Date: 07/15/2015, 09/05/2016
 * Purpose:
 *
 * Extract segments of transcription and corresponding audio sequence from ELAN annotated audio files.
 *
 * The extractor uses the first tier of annotation for the segmentation, otherwise the tier name can be
 * provided using the -t parameter.
 *
 *
 * (C) 2015-2016 by Damir Cavar <dcavar@iu.edu>
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


#include <iostream>
#include <fstream>
#include <vector>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "MySAX2Handler.h"
#include "MyMiniSAX2Handler.h"
#include "MyMediaSAX2Handler.h"


using namespace std;

namespace {
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_XML = 2;
    const size_t ERROR_UNHANDLED_EXCEPTION = 3;
    const size_t ERROR_MISSING_FILE = 4;
} // namespace

using namespace xercesc;
namespace fs = boost::filesystem;
namespace po = boost::program_options;


int getMediaNames(string xmlFile) {
    if (xmlFile.substr(xmlFile.find_last_of(".") + 1) != "eaf") {
        cout << "Warning: File " << xmlFile << " does not have the expected .eaf ending." << endl;
        cout << "Trying to parse as ELAN Annotation File (XML)." << endl;
    }
    cout << "Media files in ELAN Annotation File: " << xmlFile << ":" << endl;

    SAX2XMLReader *parser = XMLReaderFactory::createXMLReader();
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true); // optional

    MyMediaSAX2Handler *handler = new MyMediaSAX2Handler();
    parser->setContentHandler(handler);
    parser->setErrorHandler(handler);
    fs::path p(xmlFile);
    handler->file_path = p.parent_path().string();

    try {
        parser->parse((char *) xmlFile.c_str());
    }
    catch (const XMLException &toCatch) {
        char *message = XMLString::transcode(toCatch.getMessage());
        cerr << "Exception message is:" << endl << message << endl;
        XMLString::release(&message);
        return ERROR_XML;
    }
    catch (const SAXParseException &toCatch) {
        char *message = XMLString::transcode(toCatch.getMessage());
        cerr << "Exception message is:" << endl << message << endl;
        XMLString::release(&message);
        return ERROR_XML;
    }
    catch (const std::invalid_argument &e) {
        cerr << "Exception message is:" << endl << e.what() << endl;
        return ERROR_MISSING_FILE;
    }
    catch (...) {
        cerr << "Unexpected Exception" << endl;
        return ERROR_UNHANDLED_EXCEPTION;
    }

    delete parser;
    delete handler;
    return SUCCESS;
}


int getTierLabels(string xmlFile) {
    if (xmlFile.substr(xmlFile.find_last_of(".") + 1) != "eaf") {
        cout << "Warning: File " << xmlFile << " does not have the expected .eaf ending." << endl;
        cout << "Trying to parse as ELAN Annotation File (XML)." << endl;
    }
    cout << "Tiers in ELAN Annotation File: " << xmlFile << ":" << endl;

    SAX2XMLReader *parser = XMLReaderFactory::createXMLReader();
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true); // optional

    MyMiniSAX2Handler *handler = new MyMiniSAX2Handler();
    parser->setContentHandler(handler);
    parser->setErrorHandler(handler);
    fs::path p(xmlFile);
    handler->file_path = p.parent_path().string();

    try {
        parser->parse((char *) xmlFile.c_str());
    }
    catch (const XMLException &toCatch) {
        char *message = XMLString::transcode(toCatch.getMessage());
        cerr << "Exception message is:" << endl << message << endl;
        XMLString::release(&message);
        return ERROR_XML;
    }
    catch (const SAXParseException &toCatch) {
        char *message = XMLString::transcode(toCatch.getMessage());
        cerr << "Exception message is:" << endl << message << endl;
        XMLString::release(&message);
        return ERROR_XML;
    }
    catch (const std::invalid_argument &e) {
        cerr << "Exception message is:" << endl << e.what() << endl;
        return ERROR_MISSING_FILE;
    }
    catch (...) {
        cerr << "Unexpected Exception" << endl;
        return ERROR_UNHANDLED_EXCEPTION;
    }

    delete parser;
    delete handler;
    return SUCCESS;
} // getTierLabels


int parseXML(string xmlFile, string tier_name, string output_folder, string text_suffix, bool quiet,
             bool force_overwrite,
             bool add_tier_name) {
    if (xmlFile.substr(xmlFile.find_last_of(".") + 1) != "eaf") {
        if (!quiet) {
            cout << "Warning: File " << xmlFile << " does not have the expected .eaf ending." << endl;
            cout << "Trying to parse as ELAN Annotation File (XML)." << endl;
        }
    }
    if (!quiet)
        cout << "Parsing ELAN Annotation File: " << endl << xmlFile << endl;

    SAX2XMLReader *parser = XMLReaderFactory::createXMLReader();
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true); // optional

    MySAX2Handler *handler = new MySAX2Handler();
    handler->quiet = quiet; // -q from command line
    handler->target_tier_name = tier_name; // -t from command line
    handler->force_overwrite = force_overwrite; // -f from command line
    parser->setContentHandler(handler);
    parser->setErrorHandler(handler);
    fs::path p(xmlFile);
    handler->file_path = p.parent_path().string();
    handler->output_folder = output_folder;
    handler->add_tier_name = add_tier_name;
    if (text_suffix.size() > 0) {
        handler->text_suffix = text_suffix;
    }

    if (!quiet)
        cout << "Setting file path to:" << endl << handler->file_path << endl;

    try {
        parser->parse((char *) xmlFile.c_str());
        if (!handler->tier_selected) {
            if (handler->target_tier_name.size() > 0)
                cerr << "Could not find tier " << handler->target_tier_name << endl;
            else
                cerr << "No tier for segmentation found!" << endl;
            throw invalid_argument("Tier for segmentation not found.");
        }
    }
    catch (const XMLException &toCatch) {
        char *message = XMLString::transcode(toCatch.getMessage());
        cerr << "Exception message is:" << endl << message << endl;
        XMLString::release(&message);
        return ERROR_XML;
    }
    catch (const SAXParseException &toCatch) {
        char *message = XMLString::transcode(toCatch.getMessage());
        cerr << "Exception message is:" << endl << message << endl;
        XMLString::release(&message);
        return ERROR_XML;
    }
    catch (const std::invalid_argument &e) {
        cerr << "Exception message is:" << endl << e.what() << endl;
        return ERROR_MISSING_FILE;
    }
    catch (...) {
        cerr << "Unexpected Exception" << endl;
        return ERROR_UNHANDLED_EXCEPTION;
    }
    delete parser;
    delete handler;
    return SUCCESS;
} // parseXML


void help() {
    cout << "Usage: elan2split [OPTION]... [FILE]..." << endl
         << "For help: elan2split --help" << endl << endl
         << "Split ELAN Annotation files and audio recordings into single file pairs for" << endl
         << "each annotation segment at a specific tier, or using the first annotation" << endl
         << "tier in the ELAN .eaf file." << endl << endl
         << "(C) 2015-2016 by Damir Cavar <dcavar@iu.edu>" << endl << endl;
}


int main(int argc, char *argv[]) {
    bool quiet = false;
    bool list_only = false;
    bool media_only = false;
    bool force_overwrite = false;
    bool add_tier_name = false;
    string tier_name;
    string output_folder;
    string text_suffix = "";

    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException &toCatch) {
        char *message = XMLString::transcode(toCatch.getMessage());
        cerr << "Error during initialization!" << endl;
        cerr << "Exception message is:" << endl << message << endl;
        XMLString::release(&message);
        help();
        return ERROR_XML;
    }

    try {
        po::options_description desc("elan2split options");
        desc.add_options()
                ("force,f", "Force overwrite output files")
                ("help,h",  "Print help messages")
                ("quiet,q", "Quiet operations")
                ("list,l",  "List all tiers in the ELAN Annotation File")
                ("media,m", "List all attached media files (the first one is chosen for segmentation)")
                ("name,n",  "Add tier name to text-filename")
                ("suffix,s",       po::value<string>(), "Suffix for text-file")
                ("tier,t",         po::value<string>(), "Name of tier to segment")
                ("outputfolder,o", po::value<string>(), "Path of output folder (will be created, if it does not exist)")
                ("elan-files",     po::value<vector<string>>()->required(), "ELAN input files");

        po::positional_options_description p;
        p.add("elan-files", -1);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);

        if (vm.count("help")) {
            help();
            XMLPlatformUtils::Terminate();
            return SUCCESS;
        }
        po::notify(vm);

        if (vm.count("force")) {
            force_overwrite = true;
        }
        if (vm.count("quiet")) {
            quiet = true;
        }
        if (vm.count("tier")) {
            tier_name = vm["tier"].as<string>();
            if (!quiet)
                cout << "Processing tier: " << tier_name << endl;
        }
        if (vm.count("suffix")) {
            text_suffix = vm["suffix"].as<string>();
            if (!quiet)
                cout << "Using suffix for text-files: " << text_suffix << endl;
        }
        if (vm.count("name")) {
            add_tier_name = true;
        }
        if (vm.count("list")) {
            list_only = true;
            cout << "Available tiers: " << endl;
        }
        if (vm.count("media")) {
            media_only = true;
            cout << "Available media files: " << endl;
        }
        if (vm.count("outputfolder")) { // determine output folder
            fs::path of = fs::path(vm["outputfolder"].as<string>());
            if (!fs::exists(of)) { // create folders, if they do not exist
                fs::create_directories(of);
            } else if (fs::is_regular_file(of)) {
                cerr << "The specified output folder is an existing file." << endl
                << "Please specify an existing or new folder for the output." << endl;
                throw invalid_argument("Invalid output folder option.");
            }
            output_folder = fs::canonical(of).string();
            if (!quiet)
                cout << "Output folder: " << output_folder << endl;
        } else { // default output folder is getcwd
            output_folder = fs::canonical(fs::current_path()).string();
            if (!quiet)
                cout << "Output folder: " << output_folder << endl;
        }
        if (vm.count("elan-files")) {
            vector<string> files = vm["elan-files"].as<vector<string>>();
            for (string file : files) {
                if (list_only)
                    getTierLabels(fs::canonical(file).string());
                if (media_only)
                    getMediaNames(fs::canonical(file).string());
                if (!list_only && !media_only)
                    parseXML(fs::canonical(file).string(), tier_name, output_folder, text_suffix, quiet,
                             force_overwrite,
                             add_tier_name);
            }
        }
    }
    catch (std::exception &e) {
        cerr << "Error: " << e.what() << endl;
        XMLPlatformUtils::Terminate();
        help();
        return ERROR_IN_COMMAND_LINE;
    }
    catch (...) {
        cerr << "Unknown error!" << endl;
        XMLPlatformUtils::Terminate();
        help();
        return ERROR_UNHANDLED_EXCEPTION;
    }
    XMLPlatformUtils::Terminate();
    return SUCCESS;
} // main
