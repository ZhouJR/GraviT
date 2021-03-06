/* =======================================================================================
   This file is released as part of GraviT - scalable, platform independent ray tracing
   tacc.github.io/GraviT

   Copyright 2013-2015 Texas Advanced Computing Center, The University of Texas at Austin
   All rights reserved.

   Licensed under the BSD 3-Clause License, (the "License"); you may not use this file
   except in compliance with the License.
   A copy of the License is included with this software in the file LICENSE.
   If your copy does not contain the License, you may obtain a copy of the License at:

       http://opensource.org/licenses/BSD-3-Clause

   Unless required by applicable law or agreed to in writing, software distributed under
   the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
   KIND, either express or implied.
   See the License for the specific language governing permissions and limitations under
   limitations under the License.

   GraviT is funded in part by the US National Science Foundation under awards ACI-1339863,
   ACI-1339881 and ACI-1339840
   ======================================================================================= */
//
// BVH.h
//

#ifndef GVT_RENDER_DATA_ACCEL_BVH_H
#define GVT_RENDER_DATA_ACCEL_BVH_H

#include <gvt/render/data/accel/AbstractAccel.h>
#include <gvt/render/data/primitives/BBox.h>
#include <gvt/core/Math.h>

namespace gvt {
namespace render {
namespace data {
namespace accel {
/// bounding volume hierarchy (BVH) acceleration structure
/** bounding volume hierarchy (BVH) acceleration structure
used to organize high-level data domains within GraviT. GraviT
intersects rays against the BVH to determine traversal order through
the data domains and the work scheduler uses this information as
part of its evaluation process.
*/
class BVH : public AbstractAccel {
public:
  BVH(gvt::core::Vector<gvt::core::DBNodeH> &instanceSet);
  ~BVH();

  /// traverse ray through BVH and return list of leaves hit
  /**
  traverse the given ray against this BVH and return a list of leaves hit
  \param ray the ray to traverse
  \param isect list of leaves intersected
  */
  virtual void intersect(const gvt::render::actor::Ray &ray, gvt::render::actor::isecDomList &isect);

private:
  struct Node {
    Node() : leftChild(NULL), rightChild(NULL), numInstances(0) {}
    Node *leftChild;  /// null for leaf nodes
    Node *rightChild; /// null for leaf nodes
    gvt::render::data::primitives::Box3D bbox;
    int instanceSetIdx; /// base, valid when numInstances>0
    int numInstances;   /// 0 means an internal node
  };

  struct CentroidLessThan {
    CentroidLessThan(float splitPoint, int splitAxis) : splitPoint(splitPoint), splitAxis(splitAxis) {}
    bool operator()(const gvt::core::DBNodeH inst) const {
      gvt::core::DBNodeH i2 = inst;
      gvt::core::math::Point4f centroid = i2["centroid"].value().toPoint4f();
      return (centroid[splitAxis] < splitPoint);
    }

    float splitPoint;
    int splitAxis;
  };

private:
  Node *build(gvt::core::Vector<gvt::core::DBNodeH> &sortedDomainSet, int start, int end, int level);

  float findSplitPoint(int splitAxis, int start, int end);

  /// traverse ray through BVH. Called by intersect().
  void trace(const gvt::render::actor::Ray &ray, const Node *node, ClosestHit &hit,
             gvt::render::actor::isecDomList &isect, int level);

  std::vector<gvt::render::data::primitives::Box3D *> instanceSetBB;
  std::vector<int> instanceSetID;

private:
  std::vector<Node *> nodes;
  Node *root;
};
}
}
}
}

#endif // GVT_RENDER_DATA_ACCEL_BVH_H
