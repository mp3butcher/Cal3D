#include "cal3d/tinybind.h"

namespace cal3d{
	// explain to TinyBind that these vectors will be handled by a special
	// Stl container binder that handles vectors of pointers
	TiXmlBinding<CalCoreMesh::CalCoreSubmeshVector> const *
		GetTiXmlBinding(CalCoreMesh::CalCoreSubmeshVector const &, Identity<CalCoreMesh::CalCoreSubmeshVector>);

	TiXmlBinding<CalCoreSubmesh::CoreSubMorphTargetVector> const *
		GetTiXmlBinding(CalCoreSubmesh::CoreSubMorphTargetVector const &, Identity<CalCoreSubmesh::CoreSubMorphTargetVector>);

	TiXmlBinding<CalVector> const *
		GetTiXmlBinding(CalVector const &, IdentityBase);

	TiXmlBinding<CalCoreSubmesh::TextureCoordinate> const *
		GetTiXmlBinding(CalCoreSubmesh::TextureCoordinate const &, IdentityBase);

	TiXmlBinding<std::vector<CalCoreSubMorphTarget::BlendVertex> > const *
		GetTiXmlBinding(std::vector<CalCoreSubMorphTarget::BlendVertex> const &, IdentityBase);

	TiXmlBinding<CalCoreSubMorphTarget::BlendVertex> const *
		GetTiXmlBinding(CalCoreSubMorphTarget::BlendVertex const &, IdentityBase);
}