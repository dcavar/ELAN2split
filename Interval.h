/*
 * Created by Damir Cavar on 7/28/15.
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

#ifndef TUTORIAL1_INTERVAL_H
#define TUTORIAL1_INTERVAL_H

#include <iostream>
#include <string.h>

using namespace std;


class Interval {
public:
    string start_label;
    string stop_label;
    string id;
    int start;
    int stop;
    string text;
    Interval();
};


#endif //TUTORIAL1_INTERVAL_H
