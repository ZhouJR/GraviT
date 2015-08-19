#ifndef GVT_CORE_TYPES_H
#define GVT_CORE_TYPES_H

#include <gvt/core/Math.h>
//#include <gvt/render/data/Dataset.h>

#include <gvt/render/data/Primitives.h>

#include <boost/variant.hpp>
#include <boost/container/allocator.hpp>
#include <boost/container/map.hpp>
#include <boost/container/vector.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>

namespace gvt {
	namespace core {
// render stuff ya I know it doesnt belong here. 
		typedef std::string String;
                typedef boost::uuids::uuid Uuid;
                typedef boost::variant<int,long,float,double,bool,String,Uuid,gvt::core::math::Vector4f,gvt::core::math::Point4f,gvt::core::math::Vector3f,
                        /*gvt::render::data::Dataset*,*/
                        gvt::render::data::primitives::Mesh*,
                        gvt::render::data::primitives::Box3D*,
                        gvt::core::math::AffineTransformMatrix<float>*,
                        gvt::core::math::Matrix3f*,
                        void*> Variant;
                template <class T> using Vector = boost::container::vector<T>;
                template <class K, class V> using Map = boost::container::map<K, V>;

                inline Uuid nil_uuid() { return boost::uuids::nil_uuid(); }
                inline Uuid make_uuid() { return Uuid(boost::uuids::random_generator()()); }
                inline String uuid_toString(const Uuid& u) { return boost::uuids::to_string(u); }

                inline int variant_toInteger(Variant v) { return boost::get<int>(v); }
                inline long variant_toLong(Variant v) { return boost::get<long>(v); }
                inline float variant_toFloat(Variant v) { return boost::get<float>(v); }
                inline double variant_toDouble(Variant v) { return boost::get<double>(v); }
                inline bool variant_toBoolean(Variant v) { return boost::get<bool>(v); }
                inline String variant_toString(Variant v) { return boost::get<String>(v); }
                inline Uuid variant_toUuid(Variant v) { return boost::get<Uuid>(v); }
                //inline gvt::render::data::Dataset* variant_toDatasetPointer(Variant v) { return boost::get<gvt::render::data::Dataset*>(v);}
                inline gvt::core::math::Vector4f variant_toVector4f(Variant v) { return boost::get<gvt::core::math::Vector4f>(v); }
                inline gvt::core::math::Vector3f variant_toVector3f(Variant v) { return boost::get<gvt::core::math::Vector3f>(v); }
                inline gvt::core::math::Point4f variant_toPoint4f(Variant v) { return boost::get<gvt::core::math::Point4f>(v); }
                inline gvt::render::data::primitives::Box3D* variant_toBox3DPtr(Variant v) { return boost::get<gvt::render::data::primitives::Box3D*>(v);}
                inline gvt::render::data::primitives::Mesh* variant_toMeshPtr(Variant v) { return boost::get<gvt::render::data::primitives::Mesh*>(v);}
                inline gvt::core::math::AffineTransformMatrix<float>* variant_toAffineTransformMatPtr(Variant v) { return boost::get<gvt::core::math::AffineTransformMatrix<float>*>(v);}
                inline gvt::core::math::Matrix3f* variant_toMatrix3fPtr(Variant v) { return boost::get<gvt::core::math::Matrix3f*>(v);}
}
}

#endif // GVT_CORE_TYPES_H