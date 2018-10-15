/*
 * Created by Damir Cavar on 7/30/15.
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

#include "MyMediaSAX2Handler.h"


void MyMediaSAX2Handler::startElement(
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

    if (tag == MEDIA_DESCRIPTOR) {
        for (int i = 0; i < attrs.getLength(); i++) {
            temp = XMLString::transcode(attrs.getLocalName(i));
            attribute_name = myTags_[temp];
            XMLString::release(&temp);
            temp = XMLString::transcode(attrs.getValue(i));
            if (attribute_name == RELATIVE_MEDIA_URL) {
                fs::path fp(file_path);
                fs::path file_temp(temp);
                fp = fs::canonical(fp / file_temp);
                ++counter;
                if (fs::is_regular_file(fp)) {
                    rel_media_urls.push_back(fs::canonical(fp).string());
                    cout << counter << ": " << fp.string() << " OK" << endl;
                } else {
                    cout << counter << ": " << fp.string() << " File not found" << endl;
                }
            }
            XMLString::release(&temp);
        }
    }
}


void MyMediaSAX2Handler::endElement(
        const XMLCh *const uri,
        const XMLCh *const localname,
        const XMLCh *const qname) {
}


MyMediaSAX2Handler::MyMediaSAX2Handler() {
}

