# $Id$

import Blender
import blendercal
import bcconf
import bcgui

# We use immutable sets in the LOD algorithm
# to identify unique edges and faces. Immutable
# sets are handy for identification because they're
# hashable and unordered.
from sets import ImmutableSet

CONCAT = lambda s, j="": j.join(str(v) for v in s)
STRFLT = lambda f: "%%.%df" % bcconf.FLOATPRE % f

class Cal3DObject(object):
	# The base class for all of the bcobject classes. Children of this class must
	# define a method called XML which should build and return an XML representation
	# of the object. Furthermore, children that pass a value in the constructor
	# for the magic parameter are treated as top-level XML files and preprend
	# the appropriate headers.
	def __init__(self, magic=None):
		self.__magic = magic

	def __repr__(self):
		ret = ""
		xml = self.XML().replace("\t", "").replace("#", " " * bcconf.XMLINDENT)

		if self.__magic:
			ret += """<?xml version="1.0"?>\n"""
			ret += """<HEADER MAGIC="%s" VERSION="1200"/>\n""" % self.__magic
			
		return ret + xml

	def __str__(self):
		return self.__repr__()

	def XML(self):
		raise AttributeError, "Children must define this method!"

class Material(Cal3DObject):
	MATERIALS  = {}
	
	def __init__(self, name, ambient=[255]*4, diffuse=[255]*4, specular=[255]*4, mapnames=None):
		Cal3DObject.__init__(self, "XRF")

		self.name      = name
		self.ambient   = ambient
		self.diffuse   = diffuse
		self.specular  = specular
		self.shininess = 1.0
		self.mapnames  = []
		self.id        = len(Material.MATERIALS)
		
		mapnames and self.mapnames.extend(mapnames)

		Material.MATERIALS[self.name] = self
		
	def XML(self):
		mapXML = ""
		for mapname in self.mapnames:
			mapXML += "#<MAP>" + mapname + "</MAP>\n"
		return """\
			<MATERIAL NUMMAPS="%s">
			#<AMBIENT>%s %s %s %s</AMBIENT>
			#<DIFFUSE>%s %s %s %s</DIFFUSE>
			#<SPECULAR>%s %s %s %s</SPECULAR>
			#<SHININESS>%s</SHININESS>
			%s</MATERIAL>
		""" % (
			len(self.mapnames),
			self.ambient[0],
			self.ambient[1],
			self.ambient[2],
			self.ambient[3],
			self.diffuse[0],
			self.diffuse[1],
			self.diffuse[2],
			self.diffuse[3],
			self.specular[0],
			self.specular[1],
			self.specular[2],
			self.specular[3],
			self.shininess,
			mapXML
		)

class Mesh(Cal3DObject):
	def __init__(self, name):
		Cal3DObject.__init__(self, "XMF")
	
		self.name      = name.replace(".", "_")
		self.submeshes = []

	def XML(self):
		return """\
			<MESH NUMSUBMESH="%s">
			%s</MESH>
		""" % (
			len(self.submeshes),
			CONCAT(self.submeshes)
		)

class SubMesh(Cal3DObject):
	def __init__(self, mesh, material):
		Cal3DObject.__init__(self)
	
		self.material = material
		self.vertices = []
		self.faces    = []
		self.springs  = []
		self.mesh     = mesh

		self.num_lodsteps = 0
		
		mesh.submeshes.append(self)

		if not material: self.material = Material("Default")

	# These are all small classes for the creation of a very specific,
	# temporary data structure for LOD calculations. The entire submesh
	# will be temporarily copied into this structure, to allow more freedom
	# in on-the-fly refactorizations and manipulations.
	class LODVertex:
		"""We need to factor in some other information, compared
		to standard vertices, like edges and faces. On the other hand,
		we don't really need stuff like UVs when we do this. Doing another
		small, inner Vertex class for this, will hopefully not be
		seen as a total waste."""
		def __init__(self, origindex, loc, cloned):
			self.id = origindex
			self.loc = Blender.Mathutils.Vector(loc)
			self.edges = {}
			self.faces = {}

			self.cloned = cloned

			self.col_to = None
			self.col_from = None

			self.face_collapses = 0

			self.deathmarked = False

		def colto(self):
			if self.col_to:
				cvert = self.col_to
				while cvert.col_to:
					cvert = cvert.col_to
				return cvert
			else:
				return self

		def colfrom(self):
			if self.col_from:
				cvert = self.col_from
				while cvert.col_from:
					cvert = cvert.col_from
				return cvert
			else:
				return self

		def getid(self):
			return self.colto().id
		def getloc(self):
			return self.colto().loc

		def getfaces(self, facel = None):
			if not facel:
				facelist = []
			else:
				facelist = facel

			for face in self.faces.values():
				if (not face.dead) and (not facelist.__contains__(face)):
					facelist.append(face)

			if self.col_from:
				facelist = self.col_from.getfaces(facelist)

			return facelist

		def getedges(self, edgel = None):
			if not edgel:
				edgelist = []
			else:
				edgelist = edgel

			for edge in self.edges.values():
				if (not edge.dead) and (not edgelist.__contains__(edge)):
					edgelist.append(edge)

			if self.col_from:
				edgelist = self.col_from.getedges(edgelist)

			return edgelist

	class LODFace:
		def __init__(self, verts, fid):
			self.verts = verts
			vertset = ImmutableSet((self.verts[0].id, self.verts[1].id, self.verts[2].id))
			for vert in self.verts:
				vert.faces[self.getHashableSet()] = self

			self.id = fid

			self.edges = []
			self.RefactorArea()

			self.dead = False

		def replaceVert(self, replacev, withv):
			i = self.verts.index(replacev)
			self.verts[i] = withv

#		def Refactor(self):
#			self.RefactorArea()

		def RefactorArea(self):
			crossp = Blender.Mathutils.CrossVecs(self.verts[1].getloc() - self.verts[2].getloc(),
							     self.verts[0].getloc() - self.verts[2].getloc())
			self.area = (1./2.)*((crossp.x**2 + crossp.y**2 + crossp.z**2)**(1./2.))

		def getHashableSet(self):
			return ImmutableSet((self.verts[0].id, self.verts[1].id, self.verts[2].id))

	class LODEdge:
		"""Extra, inner class used for the temporary LOD datastructure"""
		def __init__(self, v1, v2):
			self.v1 = v1
			self.v2 = v2
			vertset = ImmutableSet((self.v1.id, self.v2.id))
			self.v1.edges[vertset] = self
			self.v2.edges[vertset] = self

			# Get faces common for both v1 and v2
			self.faces = []
			#for key in filter(self.v1.faces.__contains__, self.v2.faces):
			#	face = self.v1.faces[key]
				#self.faces[ImmutableSet((face.verts[0].id, face.verts[1].id, face.verts[2].id))] = face
			#	self.faces.append(face)

			self.collapsed_faces = {}

			self.RefactorLength()

			self.dead = False

		def getOtherVert(self, vertex):
			if vertex == self.v1:
				return self.v2
			elif vertex == self.v2:
				return self.v1

		def Refactor(self):
			self.RefactorLength()
			self.RefactorWeight()

		def RefactorLength(self):
			self.length = (self.v2.getloc() - self.v1.getloc()).length

		def RefactorWeight(self):
			# Determine which vert to collapse to which,
			# using jiba's method of basing this decision on
			# The number of edges connected to each vertex
			# I.e.: Collapse the edge with least amount of edges.
			# The order of the vertices in v1, v2 do not matter in
			# any other respect, so we simply use this order, and
			# say we collapse v1 into v2.
			if len(self.v1.getedges()) > len(self.v2.getedges()):
				self.v1, self.v2 = self.v2, self.v1

			# Get total area of faces surrounding edge
			area = 0
			for face in self.faces:
				area += face.area

			proportional_area = area / avg_area

			proportional_length = self.length / avg_length

			# Get dot products (angle sharpness) of edges connected to v1
			edgeverts_factor = 0

			self_vec = self.v2.getloc() - self.v1.getloc()
			self_vec.normalize()
			for edge in self.v1.edges.values():
				if edge != self:
					edgevert = edge.getOtherVert(self.v1)
					edge_vec = edgevert.getloc() - self.v1.getloc()
					edge_vec.normalize()
					edgeverts_factor += (1 - Blender.Mathutils.DotVecs(self_vec, edge_vec))/2

			# Get dot products of edges connected to v2. Wohoo, copy-paste!
			self_vec = self.v1.getloc() - self.v2.getloc()
			self_vec.normalize()
			for edge in self.v2.edges.values():
				if edge != self:
					edgevert = edge.getOtherVert(self.v2)
					edge_vec = edgevert.getloc() - self.v2.getloc()
					edge_vec.normalize()
					edgeverts_factor += (1 - Blender.Mathutils.DotVecs(self_vec, edge_vec))/2

			# Error metric, or magic formula. Whatever you like to call it.
			# This calculates the weight of the edge, based on the
			# information we have now gathered. We can change this at
			# any time to try and get better results.
			self.weight = proportional_area * proportional_length * edgeverts_factor
			#self.weight = proportional_length

			return self.weight

		def getHashableSet(self):
			return ImmutableSet((self.v1.id, self.v2.id))

		def collapse(self):
			if self.v1.col_to or self.v2.col_to:
				return False
			if self.v1.cloned or self.v2.cloned:
				return False
			if len(self.faces) < 2:
				return False
			self.dead = True

			# Mark all faces as dead and the two
			# collapsed edges as dead
			for face in filter(self.v1.getfaces().__contains__, self.v2.getfaces()):
				# If not dead, add to dict of faces to collapse with this edge
				if not face.dead:
					self.collapsed_faces[face.getHashableSet()] = face
					self.v1.face_collapses += 1
					face.dead = True
				# Mark collapsed edges as dead. Edges that don't share
				# a vertex with this edge's v2 dies.
				for edge in face.edges:
					if (edge.v1 != self.v2) and (edge.v2 != self.v2):
						edge.dead = True


#			for face in self.faces:
#				face.dead = True
#				for edge in face.edges:
#					if (edge.v1 != self.v2) and (edge.v2 != self.v2):
#						edge.dead = True
#						self.v1.face_collapses += 1
			# Refactor area of all non-dead faces on vertex 1
			for face in self.v1.getfaces():
				if not face.dead:
					face.RefactorArea()

			# Refactor lengths and weights of all non-dead
			# edges on vertex 1
			for edge in self.v1.getedges():
				if not edge.dead:
					edge.Refactor()

			self.v2.colfrom().col_from = self.v1
			self.v1.col_to = self.v2

			return True
			

	def LOD(self):
		global avg_area, avg_length
		progressbar = bcgui.Progress(10)

		# Step one. Build temporary data structure suited for weight calculations.
		# Vertices are the only ones that can be/needs to be ordered.
		# Faces and edges are dicts, with Immutable Sets (containing Vertex indices) as keys.
		LODverts = []
		LODfaces = {}
		LODedges = {}

		# Create vertices
		progressbar.setup(len(self.vertices), "Creating LODverts")
		for vertex in self.vertices:
			progressbar.increment()
			LODverts.append(self.LODVertex(vertex.id, vertex.loc, vertex.cloned))

		# Create faces
		num_faces = 0
		avg_area = 0
		total_area = 0
		progressbar.setup(len(self.faces), "Creating LODfaces")
		for face in self.faces:
			progressbar.increment()
			lface = self.LODFace([LODverts[face.vertices[0].id], LODverts[face.vertices[1].id], LODverts[face.vertices[2].id]], num_faces)
			LODfaces[lface.getHashableSet()] = lface
			total_area += lface.area
			num_faces += 1
		if num_faces:
			avg_area = total_area / float(num_faces)

		# Create edges
		num_edges = 0
		avg_length = 0
		total_length = 0
		progressbar.setup(len(LODfaces), "Creating LODedges")
		for lodface in LODfaces.values():
			progressbar.increment()
			#Create the three edges from this face
			for e in [(0, 1), (0, 2), (1, 2)]:
				imset = ImmutableSet((lodface.verts[e[0]].id, lodface.verts[e[1]].id))
				if not LODedges.has_key(imset):
					#Create edge
					lodedge = self.LODEdge(lodface.verts[e[0]],
							       lodface.verts[e[1]])
					LODedges[imset] = lodedge
					lodface.edges.append(lodedge)
					lodedge.faces.append(lodface)
					total_length += lodedge.length
					num_edges += 1
				else:
					lodedge = LODedges[imset]
					lodface.edges.append(lodedge)
					lodedge.faces.append(lodface)

		if num_edges:
			avg_length = total_length / float(num_edges)
#		print total_length
#		print avg_length


		# Step two. Calculate initial weights of all edges.
		progressbar.setup(len(LODedges), "Calculating weights")
		for edge in LODedges.values():
			progressbar.increment()
			edge.RefactorWeight()
#			print edge.weight

		# Order edges in list after weights
		LODedgelist = LODedges.values()
		LODedgelist.sort(self.compareweights)
		weight = LODedgelist[0].weight

		percentage = len(LODedgelist) * 0.6

		count = 0
		collapse_list = []

		progressbar.setup(percentage, "Calculating LOD")
		while count < percentage:
			edge = LODedgelist.pop(0)
			if not edge.dead:
				if edge.collapse():
					LODedgelist.sort(self.compareweights)
					collapse_list.append((edge.v1, edge.collapsed_faces))
			count += 1
			progressbar.increment()

		self.num_lodsteps = len(collapse_list)
		newvertlist = []
		newfacelist = []
		# The list should be in reverse order, with the most
		# important ones first.
		collapse_list.reverse()

		for vertex, faces in collapse_list:
			vertex.col_to = self.vertices[vertex.col_to.id]
		
		for vertex in LODverts:
			if not vertex.col_to:
				cvert = self.vertices[vertex.id]
				cvert.id = len(newvertlist)
				newvertlist.append(cvert)

		for face in LODfaces.values():
			if not face.dead:
				newfacelist.append(self.faces[face.id])
 
		for vertex, faces in collapse_list:
			for face in faces.values():
				newfacelist.append(self.faces[face.id])
			cvert = self.vertices[vertex.id]
			cvert.id = len(newvertlist)
			cvert.collapse_to = vertex.col_to
			cvert.num_faces = vertex.face_collapses
			newvertlist.append(cvert)

		self.vertices = newvertlist
		self.faces = newfacelist

	def compareweights(self, x, y):
		result = x.weight - y.weight
		if result < 0:
			return -1
		elif result > 0:
			return 1
		else:
			return 0

	def XML(self):
		return """\
			#<SUBMESH
			##NUMVERTICES="%s"
			##NUMFACES="%s"
			##MATERIAL="%s"
			##NUMLODSTEPS="%s"
			##NUMSPRINGS="%s"
			##NUMTEXCOORDS="%s"
			#>
			%s%s%s#</SUBMESH>
		""" % (
			len(self.vertices),
			len(self.faces),
			self.material.id,
			self.num_lodsteps,
			len(self.springs),
			len(self.material.mapnames),
			CONCAT(self.vertices),
			CONCAT(self.springs),
			CONCAT(self.faces)
		)

#class Progress:
#	self.progress = 0.0
#	self.

class Map(Cal3DObject):
	def __init__(self, uv):
		Cal3DObject.__init__(self)

		self.uv = Blender.Mathutils.Vector(uv)

	def XML(self):
		return "###<TEXCOORD>%s %s</TEXCOORD>\n" % (
			STRFLT(self.uv.x),
			STRFLT(self.uv.y)
		)

class Vertex(Cal3DObject):
	# An interesting note about this class is that we keep Blender objects
	# as self.loc and self.normal. Of note is how we "wrap" the existing
	# instances with our own copies, since I was experiencing bugs where
	# the existing ones would go out of scope.
	def __init__(self, submesh, loc, normal, cloned, uvs):
		Cal3DObject.__init__(self)
	
		self.loc        = Blender.Mathutils.Vector(loc)
		self.normal     = Blender.Mathutils.Vector(normal)
		self.maps       = []
		self.influences = []
		self.submesh    = submesh
		self.id         = len(submesh.vertices)
		self.cloned     = cloned
		self.collapse_to = None
		self.num_faces = 0
		
		# If one UV is None, the rest will also be None.
		if len(uvs) and (uvs[0][0] != None):
			self.maps.extend(Map(uv) for uv in uvs)
		
		submesh.vertices.append(self)

	def XML(self):
		loc    = blendercal.VECTOR2GL(self.loc)
		normal = blendercal.VECTOR2GL(self.normal)
		unset  = lambda t: "###<!-- %s unset -->\n" % t

		collapse = ""
		# Note: collapse_to is an index, and _can_ be 0
		if self.collapse_to != None:
			collapse = """\
			###<COLLAPSEID>%s</COLLAPSEID>
			###<COLLAPSECOUNT>%s</COLLAPSECOUNT>
		""" % (
			str(self.collapse_to.id),
			str(self.num_faces)
		)

		return """\
			##<VERTEX ID="%s" NUMINFLUENCES="%s">
			###<POS>%s %s %s</POS>
			###<NORM>%s %s %s</NORM>
			%s%s%s##</VERTEX>
		""" % (
			self.id,
			len(self.influences),
			STRFLT(loc.x),
			STRFLT(loc.y),
			STRFLT(loc.z),
			STRFLT(normal.x),
			STRFLT(normal.y),
			STRFLT(normal.z),
			collapse,
			len(self.maps) and CONCAT(self.maps) or unset("UV Coords"),
			self.influences and CONCAT(self.influences) or unset("Influences")
		)

class Influence(Cal3DObject):
	def __init__(self, bone, weight):
		Cal3DObject.__init__(self)
	
		self.bone   = bone
		self.weight = weight
  
	def XML(self):
		return """###<INFLUENCE ID="%s">%s</INFLUENCE>\n""" % (
			self.bone.id,
			STRFLT(self.weight)
		)
 
class Face(Cal3DObject):
	def __init__(self, submesh, v1, v2, v3):
		Cal3DObject.__init__(self)
	
		self.vertices = (v1, v2, v3)
		self.submesh  = submesh

		submesh.faces.append(self)
  
	def XML(self):
		return """##<FACE VERTEXID="%s %s %s"/>\n""" % (self.vertices[0].id, self.vertices[1].id, self.vertices[2].id)

class Skeleton(Cal3DObject):
	ARMATURE = None

	def __init__(self):
		Cal3DObject.__init__(self, "XSF")

		self.bones = []
  
	def XML(self):
		return """\
			<SKELETON NUMBONES="%s">
			%s</SKELETON>
		""" % (
			len(self.bones),
			CONCAT(self.bones)
		)

class Bone(Cal3DObject):
	BONES = {}

	def __init__(self, skeleton, parent, bone, armamat):
		Cal3DObject.__init__(self)

		absmat = bone.matrix["ARMATURESPACE"] * armamat

		self.parent   = parent
		self.name     = bone.name.replace(".", "_")
		self.invert   = Blender.Mathutils.Matrix(absmat).invert()
		self.local    = (parent and (absmat * self.parent.invert)) or absmat
		self.children = []
		self.skeleton = skeleton
		self.id       = len(skeleton.bones)

		if self.parent:
			self.parent.children.append(self)
		
		skeleton.bones.append(self)
		
		Bone.BONES[self.name] = self

	def XML(self):
		# TRANSLATION and ROTATION are relative to the parent bone.
		# They are virtually useless since the animations (.XAF .CAF)
		# will always override them.
		#
		# LOCALTRANSLATION and LOCALROTATION are the invert of the cumulated
		# TRANSLATION and ROTATION (see above). It is used to calculate the
		# delta between an animated bone and the original non animated bone.
		# This delta will be applied to the influenced vertexes.
		#
		# Negate the rotation because blender rotations are clockwise
		# and cal3d rotations are counterclockwise
		
		local     = blendercal.MATRIX2GL(self.local)
		localloc  = local.translationPart()
		localrot  = local.toQuat()
		invert    = blendercal.MATRIX2GL(self.invert)
		invertloc = invert.translationPart()
		invertrot = invert.toQuat()

		return """\
			#<BONE ID="%s" NAME="%s" NUMCHILD="%s">
			##<TRANSLATION>%s %s %s</TRANSLATION>
			##<ROTATION>%s %s %s -%s</ROTATION>
			##<LOCALTRANSLATION>%s %s %s</LOCALTRANSLATION>
			##<LOCALROTATION>%s %s %s -%s</LOCALROTATION>
			##<PARENTID>%s</PARENTID>
			%s#</BONE>
		""" % (
			self.id,
			self.name,
			len(self.children),
			STRFLT(localloc.x),
			STRFLT(localloc.y),
			STRFLT(localloc.z),
			STRFLT(localrot.x),
			STRFLT(localrot.y),
			STRFLT(localrot.z),
			STRFLT(localrot.w),
			STRFLT(invertloc.x),
			STRFLT(invertloc.y),
			STRFLT(invertloc.z),
			STRFLT(invertrot.x),
			STRFLT(invertrot.y),
			STRFLT(invertrot.z),
			STRFLT(invertrot.w),
			self.parent and "%d" % self.parent.id or "-1",
			"".join("##<CHILDID>%s</CHILDID>\n" % c.id for c in self.children)
		)

class Animation(Cal3DObject):
	def __init__(self, name, duration=0.0):
		Cal3DObject.__init__(self, "XAF")
	
		self.name     = name.replace(".", "_")
		self.duration = duration
		self.tracks   = {}
  
	def XML(self):
		return """\
			<ANIMATION DURATION="%s" NUMTRACKS="%s">
			%s</ANIMATION>
		""" % (
			self.duration,
			len(self.tracks),
			CONCAT(self.tracks.values())
		)
		
class Track(Cal3DObject):
	def __init__(self, animation, bone):
		Cal3DObject.__init__(self)

		self.bone      = bone
		self.keyframes = []
		self.animation = animation
		
		animation.tracks[bone.name] = self
    
	def XML(self):
		return """\
			#<TRACK BONEID="%s" NUMKEYFRAMES="%s">
			%s#</TRACK>
		""" % (
			self.bone.id,
			len(self.keyframes),
			CONCAT(self.keyframes)
		)
		
class KeyFrame(Cal3DObject):
	def __init__(self, track, time, loc, rot):
		Cal3DObject.__init__(self)
	
		self.time  = time
		self.loc   = Blender.Mathutils.Vector(loc)
		self.rot   = Blender.Mathutils.Quaternion(rot)
		self.track = track
		
		track.keyframes.append(self)
  
	def XML(self):
		return """\
			##<KEYFRAME TIME="%s">
			###<TRANSLATION>%s %s %s</TRANSLATION>
			###<ROTATION>%s %s %s -%s</ROTATION>
			##</KEYFRAME>
		""" % (
			STRFLT(self.time),
			STRFLT(self.loc.x),
			STRFLT(self.loc.y),
			STRFLT(self.loc.z),
			STRFLT(self.rot.x),
			STRFLT(self.rot.y),
			STRFLT(self.rot.z),
			STRFLT(self.rot.w)
		)
