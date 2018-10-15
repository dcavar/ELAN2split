/*
 * Created by Damir Cavar on 7/29/15.
 *
 * Parse ELAN Annotation Files (in XML) and extract all tier labels and print them out.
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

#ifndef ELAN2SPLIT_MYMINISAX2HANDLER_H
#define ELAN2SPLIT_MYMINISAX2HANDLER_H


#include "MySAX2Handler.h"


class MyMiniSAX2Handler : public MySAX2Handler {

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

    MyMiniSAX2Handler();

};


#endif //ELAN2SPLIT_MYMINISAX2HANDLER_H
