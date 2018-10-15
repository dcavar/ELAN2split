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

#ifndef ELAN2SPLIT_MYMEDIASAX2HANDLER_H
#define ELAN2SPLIT_MYMEDIASAX2HANDLER_H


#include "MySAX2Handler.h"


class MyMediaSAX2Handler : public MySAX2Handler {

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

    int counter = 0;

    MyMediaSAX2Handler();

};


#endif //ELAN2SPLIT_MYMEDIASAX2HANDLER_H
