# $Id$

# The purpose of this module is to provide methods by which the caller can easily
# create sequences of bcobject instances for their own use. These don't necessarily
# have to be used in a way specific to Cal3D.

# TODO: I'm using too many list traversals here. It could probably be condensed
# down into "one interation to rule them all."

import Blender
import blendercal
import bcobject
import bcconf
import os

def __yieldBlenderObj(t, objlist=None):
	# This function yields the Blender Object and the result of getData() on
	# that Object for each instance that is a type of t. Futher, you can specify
	# a list of objects to traverse rather than Blender.Object.Get().

	for obj in objlist or Blender.Object.Get():
		# Ignore anything that begins with an underscore as a kind
		# of "agreed-upon" technique.
		if obj.getType() == t and not obj.getName().startswith("_"):
			yield obj, obj.getData()

@blendercal.exception
def SkeletonData():
	# This function returns a single blendercal.bcobject.Skeleton instance
	# and sets blendercal.bcobject.Skeleton.ARMATURE to the appropriate Blender
	# object. For the time being we ony support a single skeleton (although, this
	# could be changed later), so it only makes sense to return a single Skeleton.
	# The ARMATURE variable is set for use later in retrieving the animations
	# tied to the Armature.

	skeleton = bcobject.Skeleton()

	# A recursive function that operates on a single root bone and creates
	# bcobject.Bone instances appropriately. It might be possible to totally
	# get rid of this function and just use bone.getAllChildren() instead.
	def __recurseBone(bone, matrix, parent=None):
		cbone = bcobject.Bone(skeleton, parent, bone, matrix)

		bone.hasChildren() and [__recurseBone(c, matrix, cbone) for c in bone.children]

	for obj, data in __yieldBlenderObj("Armature"):
		rootbones = [b for b in data.bones.values() if not b.hasParent()]

		# Use the first rootbone we find, even if there are multiples.
		len(rootbones) and __recurseBone(rootbones[0], obj.getMatrix())
		
		# Set the ARMATURE variable for use later in Animations.
		bcobject.Skeleton.ARMATURE = obj
	
	return skeleton

@blendercal.exception
def MeshData():
	# This function returns a list of blender.bcobject.Mesh objects, one for
	# each mesh in your Blender scene. The Cal3D notion of a Mesh is actually more
	# like a container whose purpose it is to hold other SubMeshes, which are
	# what hold the vertex, weight, and material data. As it is now, there is always
	# one Mesh+SubMesh combo per item in the list, though it would also be possible
	# to have all meshes in the scene be SubMeshes of a single Mesh returned
	# by this function.

	meshes = []

	# This class serves as a kind of "temporary" vertex class that we will use
	# to create both the vertex list and the index list for later iteration. The
	# only reason we have to do this is due to the fact that Blender doesn't
	# pre-sanitize vertex data such that each vertex is TRULY unique, i.e. in 
	# location, normal, and UV coords.
	#
	# TODO: There are special methods (__hash__, __cmp__, and __eq__) we could
	# use instead for implicit comparison rather than having to call MakeKey()
	# directly. I'll need to research that a bit...
	class HashableVertex(object):
		def __init__(self, index, co, no, uvx, uvy):
			object.__init__(self)

			self.index = index
			self.co    = co
			self.no    = no
			self.uvco  = uvx, uvy

		# We will use the return value of this function as keys in a dictionary
		# to make sure we only have one occurance of each unique vertex.
		def MakeKey(self):
			return self.index, self.uvco[0], self.uvco[1]

	# A function to split our faces into triangles and convert the vertices into
	# our HasableVert class.
	def __triFaces(f, hasUV):
		nulluv = lambda n: [(None, None)] * n
	
		if len(f) == 4:
			verts = f.v[ : 3] + f.v[2 : ] + [f.v[0]]
			uvs   = hasUV and f.uv[ : 3] + f.uv[2 : ] + [f.uv[0]] or nulluv(6)
			
		else:
			verts = f.v
			uvs   = hasUV and f.uv or nulluv(3)

		if hasUV: uvs = [(faceuv[0], 1.0 - faceuv[1]) for faceuv in uvs]
		
		return [HashableVertex(
			facevert.index,
			facevert.co,
			facevert.no,
			faceuv[0],
			faceuv[1]
		) for facevert, faceuv in zip(verts, uvs)]
	
	# This will return two lists: the first is a list of indices (in triangles) of
	# faces. The second is the actual vertices the list refers to. These is still
	# data in the style of Blender; that is, nothing has been converted to Cal3D
	# specific formats yet.
	def __createIndexVertexLists(faces, hasUV):
		lookup     = {}
		vertices   = []
		indices    = []

		duplookup  = {}
		dupindices = {}

		# - A vertex needs to be appended (and then it's index) if it doesn't
		#   appear in the lookup dictionary.
		# - An index HAS to be appended.
		for vert in reduce(lambda x, y: x + y, [__triFaces(f, hasUV) for f in faces]):
			key = vert.MakeKey()

			# If we can't find the "key" in the lookup dictionary, add
			# it to the vertices list and put it's index back into the
			# lookup table.
			if not key in lookup:
				lookup[key] = i = len(vertices)

				vertices.append(vert)
			
			# Otherwise, just get the index to be appended to the indices
			# list, to be handled later.
			else:
				i = lookup[key]

			if not vert.index in duplookup:
				duplookup[vert.index] = vert

			else:
				if vert.uvco != duplookup[vert.index].uvco:
					dupindices[vert.index] = True
					dupindices[i] = True

			indices.append(i)

		return indices, vertices, dupindices

	def __createSubMeshes(mesh, objlist):
		for obj, data in __yieldBlenderObj("Mesh", objlist):
			# Put mesh into worldspace
			objmatrix   = Blender.Mathutils.Matrix(obj.getMatrix('worldspace'))
			localmatrix = Blender.Mathutils.Matrix(obj.getMatrix('localspace'))
			
			obj.setMatrix(Blender.Mathutils.Matrix().identity())
			
			data.transform(objmatrix, True)

			# Handle all of the materials associated with this Mesh.
			firstmaterial   = True
			submeshmaterial = None 
			
			for m in data.materials:
				if not m.name in bcobject.Material.MATERIALS:
					mapnames = []
					ambient  = [255] * 3
					
					diffuse = [
						m.R * 255,
						m.G * 255,
						m.B * 255,
						m.getRef() * 255
					]
					
					specular = [
						m.specR * 255,
						m.specG * 255,
						m.specB * 255,
						m.spec  * 127.5
					]

					ambient.append(m.getAmb() * 255)
					
					for mtex in m.getTextures():
						if mtex and mtex.tex.getType() == "Image":
							filename = mtex.tex.image.getFilename()
							filename = os.path.split(filename)[1]
							
							mapnames.append(filename)

					material = bcobject.Material(
						m.name,
						ambient,
						diffuse,
						specular,
						mapnames
					)
					
					if firstmaterial:
						submeshmaterial = material
					
					firstmaterial = False

			submesh = bcobject.SubMesh(mesh, submeshmaterial)
			
			ilist, vlist, dupdict = __createIndexVertexLists(
				data.faces,
				data.hasFaceUV()
			)

			# At this points we should have a list of indices and a list of
			# vertices which the index list refers to. We could iterate over
			# both sequences and create the bcobject.{Face,Vertex} objects
			# accordingly.

			vertices = [bcobject.Vertex(
				submesh,
				v.co,
				v.no,
				dupdict.has_key(v.index),
				[v.uvco] * len(submesh.material.mapnames)
			) for v in vlist]

			# We really need to have references to the actual vertices,
			# otherwise, changing the vertex order during LOD becomes much
			# too difficult.
			faces = []

			for v in zip(*[iter(ilist)] * 3):
				verts = vertices[v[0]], vertices[v[1]], vertices[v[2]]
				face  = bcobject.Face(submesh, *verts)
				
				faces.append(face)
			
			# TODO: *sigh*, this used to be so clean. :)
			# faces = [bcobject.Face(submesh, *v) for v in zip(*[iter(ilist)] * 3)]

			# Here we parse data.verts (again) to get the weight values
			# of each Vertex. For some reason, Blender doesn't guarantee
			# that the max weight will be 1.0, so we keep our own sum().
			#
			# TODO: This could probably be moved up into the first loops,
			# preventing another iteration of all the vertices here.
			for i, v in enumerate(vlist):
				weights = data.getVertexInfluences(v.index)
				total   = sum(w for b, w in weights)
			
				for b, w in weights:
					vertices[i].influences.append(bcobject.Influence(
						bcobject.Bone.BONES[b.replace(".", "_")],
						w / total
					))

			# Put object matrix and mesh back to normal
			obj.setMatrix(Blender.Mathutils.Matrix(localmatrix))

			data.transform(objmatrix.invert(), True)

			if bcconf.LOD:
				submesh.LOD()

	# Automatic choice. If any Groups exist, we export each Group as a mesh
	# and each Object in that group as a submesh. Otherwise, we just create
	# one mesh with all objects of the file as submeshes.
	#
	# TODO: Perhaps make this choice user-configurable?
	if len(Blender.Group.Get()):
		for group in Blender.Group.Get():
			mesh = bcobject.Mesh(group.name)
			
			meshes.append(mesh)
			
			__createSubMeshes(mesh, group.objects)
	
	else:
		mesh = bcobject.Mesh(Blender.Scene.GetCurrent().name)
		
		meshes.append(mesh)
		
		__createSubMeshes(mesh, Blender.Object.Get())

	return meshes

@blendercal.exception
def AnimationData():
	# This function demonstrates a new way of parsing and retrieving animation
	# data in Blender. With version 242 and above, users can call the the
	# Object.evaluatePose and Action.setActive methods to set the poses and read
	# the rotation data directly from the pose bones.
	
	animations = []

	for actionname, action in Blender.Armature.NLA.GetActions().iteritems():
		animation = bcobject.Animation(actionname)

		action.setActive(bcobject.Skeleton.ARMATURE)

		name2ipo = {}
		frames   = []
		
		# Grab the "times" at which this action has keyframes. There has
		# GOT to be a better way than this.
		for iponame, ipo in action.getAllChannelIpos().iteritems():
			for curve in ipo.getCurves():
				for point in curve.getPoints():
					frame = int(point.pt[0])
					
					frame not in frames and frames.append(frame)
			
				name2ipo[iponame] = ipo

		frames.sort()

		animation.duration = frames[-1]

		# Move the armature to the keyframe "times" and grab the data we
		# need to export our animations.
		for iponame, ipo in name2ipo.iteritems():
			bonename = iponame.replace(".", "_")
			bone     = bcobject.Bone.BONES[bonename]
			track    = bcobject.Track(animation, bone)
			
			animation.tracks[bonename] = track

			blenderbone = bcobject.Skeleton.ARMATURE.getPose().bones[iponame]
		
			# Before versions 242 you would build Loc/Quat values using
			# Ipo.evaluate(); no need for that now.
			for curframe in frames:
				bcobject.Skeleton.ARMATURE.evaluatePose(curframe)
				
				matrix = blenderbone.quat.toMatrix()
				
				matrix.resize4x4()
				
				matrix[3] = Blender.Mathutils.Vector(*blenderbone.loc).resize4D()
				matrix    = blendercal.MATRIX2GL(matrix * bone.local)
      
				bcobject.KeyFrame(
					track,
					(curframe - 1) / bcconf.ANIMFPS,
					matrix.translationPart(),
					matrix.toQuat()
				)

		animation.duration /= bcconf.ANIMFPS

		animations.append(animation)

	return animations

@blendercal.exception
def ExportData(filename, skeldata, meshdata, animdata, prefixfiles=False):
	dirname  = os.path.dirname(filename)
	basename = os.path.splitext(os.path.basename(filename))[0]
		
	try:
		os.makedirs(dirname)
	
	except OSError, e:
		pass

	cfg = file(os.path.join(dirname, "%s.cfg" % basename), "w")
	
	if prefixfiles:
		prefix = "%s_" % basename
	
	else:
		prefix = ""
	
	print >> cfg, "# Cal3D model exported from Blender with cal3d-export.py"
	print >> cfg, "skeleton=%s.xsf" % basename
	
	print >> file(os.path.join(dirname, "%s.xsf" % basename), "w"), skeldata
	
	for animation in animdata:
		if not animation.name.startswith("_"):
			animfile = "%s%s.xaf" % (prefix, animation.name)
				
			print >> file(os.path.join(dirname, animfile), "w"), animation
				
			print >> cfg, "animation=%s" % animfile

	for mesh in meshdata:
		if not mesh.name.startswith("_"):
			meshfile = "%s%s.xmf" % (prefix, mesh.name)
		
			print >> file(os.path.join(dirname, meshfile), "w"), mesh
			
			print >> cfg, "mesh=%s" % meshfile
	
	materials = bcobject.Material.MATERIALS.values()
	
	materials.sort(lambda a, b: cmp(a.id, b.id))
	
	for material in materials:
		matfile = "%s.xrf" % material.name
		
		print >> file(os.path.join(dirname, matfile), "w"), material
		
		print >> cfg, "material=%s" % matfile
