/* 
 * File:   RayWeightedSpread.h
 * Author: jbarbosa
 *
 * Created on January 21, 2014, 4:16 PM
 */

#ifndef RAYWEIGHTEDSPREAD_H
#define	RAYWEIGHTEDSPREAD_H

#include "HybridBaseSchedule.h"

struct RayWeightedSpreadSchedule : public HybridBaseSchedule {

    RayWeightedSpreadSchedule(int * newMap, int &size, int *map_size_buf, int **map_recv_bufs, int *data_send_buf) : HybridBaseSchedule(newMap, size, map_size_buf, map_recv_bufs, data_send_buf) {

    }

    virtual ~RayWeightedSpreadSchedule() {

    }

    virtual void operator()() {
        DEBUG(cerr << "in adaptive schedule" << endl);
        for (int i = 0; i < size; ++i)
            newMap[i] = -1;

        std::map<int, int> data2proc;
        std::map<int, int> data2size;
        std::map<int, int> size2data;
        for (int s = 0; s < size; ++s) {
            if (map_recv_bufs[s]) {
                // add currently loaded data
                data2proc[map_recv_bufs[s][0]] = s; // this will evict previous entries. that's okay since we don't want to dup data
                DEBUG(cerr << "    noting currently " << s << " -> " << map_recv_bufs[s][0] << endl);

                // add ray counts
                for (int d = 1; d < map_size_buf[s]; d += 2) {
                    data2size[map_recv_bufs[s][d]] += map_recv_bufs[s][d + 1];
                    DEBUG(cerr << "        " << s << " has " << map_recv_bufs[s][d + 1] << " rays for data " << map_recv_bufs[s][d] << endl);
                }
            }
        }

        // convert data2size into size2data,
        // use data id to pseudo-uniqueify, since only need ordering
        for (std::map<int, int>::iterator it = data2size.begin(); it != data2size.end(); ++it) {
            size2data[(it->second << 7) + it->first] = it->first;
        }

        // iterate over queued data, find which are already loaded somewhere
        // since size2data is sorted in increasing key order,
        // homeless data with most rays will end up at top of homeless list
        std::vector<int> homeless;
        for (std::map<int, int>::iterator d2sit = size2data.begin(); d2sit != size2data.end(); ++d2sit) {
            map<int, int>::iterator d2pit = data2proc.find(d2sit->second);
            if (d2pit != data2proc.end()) {
                newMap[d2pit->second] = d2pit->first;
                DEBUG(cerr << "    adding " << d2pit->second << " -> " << d2pit->first << " to map" << endl);
            } else {
                homeless.push_back(d2sit->second);
                DEBUG(cerr << "    noting " << d2sit->second << " is homeless" << endl);
            }
        }

        // iterate over newMap, fill as many procs as possible with homeless data
        // could be dupes in the homeless list, so keep track of what's added
        for (int i = 0; (i < size) & (!homeless.empty()); ++i) {
            if (newMap[i] < 0) {
                while (!homeless.empty()
                        && data2proc.find(homeless.back()) != data2proc.end())
                    homeless.pop_back();
                if (!homeless.empty()) {
                    newMap[i] = homeless.back();
                    data2proc[newMap[i]] = i;
                    homeless.pop_back();
                }
            }
        }

        DEBUG(cerr << "new map size is " << size << endl;
        for (int i = 0; i < size; ++i)
                cerr << "    " << i << " -> " << newMap[i] << endl;
                );
    }

};


#endif	/* RAYWEIGHTEDSPREAD_H */
