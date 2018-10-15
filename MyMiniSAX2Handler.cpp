/*
 * Created by Damir Cavar on 7/29/15.
 *
 * Parse ELAN Annotation Files (in XML) and extract all tier labels and print them out.
 *
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


#include "MyMiniSAX2Handler.h"


void MyMiniSAX2Handler::startElement(
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

    if (tag == TIER) {
        for (int i = 0; i < attrs.getLength(); i++) {
            temp = XMLString::transcode(attrs.getLocalName(i));
            attribute_name = myTags_[temp];
            XMLString::release(&temp);
            temp = XMLString::transcode(attrs.getValue(i));
            if (attribute_name == TIER_ID)
                cout << temp << endl;
            XMLString::release(&temp);
        }
    }
}


void MyMiniSAX2Handler::endElement(
        const XMLCh *const uri,
        const XMLCh *const localname,
        const XMLCh *const qname) {
}


MyMiniSAX2Handler::MyMiniSAX2Handler() {
}
