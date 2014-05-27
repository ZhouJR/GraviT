

#include "Dataset.h"

namespace GVT {
    namespace Dataset {

        bool GVTDataset::init() {
            GVT_DEBUG(DBG_ALWAYS, "Abstract load");
            return false;
        };

        bool GVTDataset::intersect(GVT::Data::ray& r, GVT::Data::isecDomList& inter) {
            //GVT_DEBUG(DBG_ALWAYS, "Try Intersect with the world : " << r << " == " << dataSetBB );
            if (dataSetBB.intersect(r)) {
                //GVT_DEBUG(DBG_ALWAYS, "Intersects with the world");
                r.t = FLT_MAX;
                for (GVT::Domain::Domain* d : domainSet) d->intersect(r, inter);
            }
        };

        GVT::Domain::Domain* GVTDataset::getDomain(int id) {
            GVT_ASSERT_BACKTRACE(id < domainSet.size(),"Getting domain outside bound");
            return domainSet[id];
        }

        GVT::Data::LightSource* GVTDataset::getLightSource(int id) {
            GVT_ASSERT_BACKTRACE(id <lightSet.size(),"Getting light source outside bound");
            return lightSet[id];
        }

        int GVTDataset::addDomain(GVT::Domain::Domain* dom) {
            dataSetBB.merge(dom->getWorldBoundingBox());
            domainSet.push_back(dom);
            dom->setDomainID(domainSet.size() - 1);
            
            GVT_DEBUG(DBG_ALWAYS,"Add domain. World BB : " << dataSetBB);
            
            return domainSet.size() - 1;
        }

        int GVTDataset::addLightSource(GVT::Data::LightSource* ls) {
            dataSetBB.merge(ls->getWorldBoundingBox());
            lightSet.push_back(ls);
            return domainSet.size() - 1;
        }
        
        int GVTDataset::size() {
            return domainSet.size();
        }
    }
}
