#ifndef GVT_RENDER_ADAPTER_EMBREE_DATA_EMBREE_MESH_ADAPTER_H
#define GVT_RENDER_ADAPTER_EMBREE_DATA_EMBREE_MESH_ADAPTER_H

#include "gvt/render/Adapter.h"

#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#include <string>

namespace gvt {
namespace render {
namespace adapter {
namespace embree {
namespace data {

class EmbreeMeshAdapter : public gvt::render::Adapter
{
public:
    /**
     * Construct the Embree mesh adapter.  Convert the mesh
     * at the given node to Embree's format.
     *
     * Initializes Embree the first time it is called.
     */
    EmbreeMeshAdapter(gvt::core::DBNodeH node);

    /**
     * Release Embree copy of the mesh.
     */
    virtual ~EmbreeMeshAdapter();

    /**
     * Return the Embree scene handle;
     */
    RTCScene getScene() { return scene; }

    /**
     * Return the geometry id.
     */
    unsigned getGeomId() { return geomId; }

    /**
     * Trace rays using the Embree adapter.
     *
     * Creates threads and traces rays in packets defined by GVT_EMBREE_PACKET_SIZE
     * (currently set to 4).
     *
     * \param rayList incoming rays
     * \param moved_rays outgoing rays [rays that did not hit anything]
     * \param instNode instance db node containing dataRef and transforms
     */
    virtual void trace(gvt::render::actor::RayVector& rayList,
            gvt::render::actor::RayVector& moved_rays,
            gvt::core::DBNodeH instNode);

protected:
    /**
     * Static bool to initialize Embree (calling rtcInit) before use.
     * 
     * // TODO: this will need to move in the future when we have different types of Embree adapters (ex: mesh + volume)
     */
    static bool init;

    /**
     * Currently selected packet size flag.
     */
    RTCAlgorithmFlags packetSize;

    /**
     * Handle to Embree scene.
     */
    RTCScene scene;

    /**
     * Handle to the Embree triangle mesh.
     */
    unsigned geomId;
};

}
}
}
}
}


#endif // GVT_RENDER_ADAPTER_EMBREE_DATA_EMBREE_MESH_ADAPTER_H