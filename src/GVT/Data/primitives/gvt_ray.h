/* 
 * File:   ray.h
 * Author: jbarbosa
 *
 * Created on March 28, 2014, 1:29 PM
 */

#ifndef RAY_H
#define	RAY_H

#include <GVT/common/debug.h>
#include <boost/container/vector.hpp>
#include <GVT/Data/scene/Color.h>
#include <GVT/Math/GVTMath.h>


#include <boost/container/set.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

namespace GVT {
    namespace Data {

        typedef boost::tuple<float, int> isecDom;
        typedef boost::container::set<isecDom> isecDomList;

        class ray {
        public:

            enum RayType {
                PRIMARY,
                SHADOW,
                SECUNDARY
            };



            //GVT_CONVERTABLE_OBJ(GVT::Data::ray);

            ray(GVT::Math::Point4f origin = GVT::Math::Point4f(0, 0, 0, 1), GVT::Math::Vector4f direction = GVT::Math::Vector4f(0, 0, 0, 0), float contribution = 1.f, RayType type = PRIMARY, int depth = 10);
            ray(ray &ray, GVT::Math::AffineTransformMatrix<float> &m);
            ray(const ray& orig);
            ray(const unsigned char* buf);

            virtual ~ray();


            void setDirection(GVT::Math::Vector4f dir);
            void setDirection(double *dir);
            void setDirection(float *dir);

            int packedSize();

            int pack(unsigned char* buffer);

            friend ostream& operator<<(ostream& stream, GVT::Data::ray const& ray) {
                stream << ray.origin << "-->" << ray.direction << "[" << ray.type << "]";
                return stream;
            }


            mutable GVT::Math::Point4f origin;
            mutable GVT::Math::Vector4f direction;
            mutable GVT::Math::Vector4f inverseDirection;
            mutable int sign[3];


            int id; ///<! index into framebuffer
            int b; ///<! bounce count for ray 
            float r; ///<! sample rate
            float w; ///<! weight of image contribution
            mutable float t;
            mutable float tmin, tmax;
            mutable float tprim;
            mutable float origin_domain;
            COLOR_ACCUM color;
            isecDomList domains;
            boost::container::vector<int> visited;
            int type;

            const static float RAY_EPSILON;
            int depth;

        };

        typedef boost::container::vector<GVT::Data::ray> RayVector;


    };
};
#endif	/* RAY_H */

