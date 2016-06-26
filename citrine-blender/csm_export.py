#!BPY

"""
Name: 'Citrine Standard Mesh (.csm)'
Blender: 244
Group: 'Export'
Tooltip: 'Export Citrine Standard Mesh File'
"""

# Copyright (C) 2010 Braden Walters
#
# This software may be modified and distributed under the terms of the MIT
# license. See the LICENSE file for details.

import bpy
import Blender
from Blender import *
import os
import struct
import shutil

def ExportVertexFrame (obj, outfile):
	mesh = obj.getData (mesh = 1)

	vertices = []
	triangles = []

	#Get all vertices
	for vertex in mesh.verts:
		vertices.append ([vertex.co.x, vertex.co.z, vertex.co.y, 0.0, 0.0, 0.0, 0.0, 0.0])

	#Get all triangles
	for face in mesh.faces:
		#Apply texture coordinates to vertices if valid
		if mesh.faceUV:
			i = 0
			for vertex in face.verts:
				vertices[vertex.index][6] = face.uv[i][0]
				vertices[vertex.index][7] = face.uv[i][1]
				i += 1
		#Get first triangle in face
		triangles.append ([face.verts[0].index, face.verts[1].index, face.verts[2].index])
		#If this face is a quad, get the second triangle
		if len (face.verts) == 4:
			triangles.append ([face.verts[0].index, face.verts[2].index, face.verts[3].index])

	#Write amount of vertices and faces
	outfile.write (struct.pack ('>H', len (vertices)))
	outfile.write (struct.pack ('>H', len (triangles)))

	#Write all vertices
	for vertex in vertices:
		outfile.write (struct.pack ('>8f', vertex[0], vertex[1], vertex[2], vertex[3], vertex[4], vertex[5], vertex[6], vertex[7]))
	#Write all triangles
	for triangle in triangles:
		outfile.write (struct.pack ('>3H', triangle[0], triangle[1], triangle[2]))

def ExportVertexAnimation (obj, outfile):
	#Write base frame
	ExportVertexFrame (obj, outfile)

	#Write amount of animations (TODO: Add animation support)
	outfile.write (struct.pack ('>H', 0))

def GetArmatureAnimations (armature, scene):
	#Get actions
	actions = Blender.Armature.NLA.GetActions()

	animations = []

	#Get all actions
	for action in actions:
		#Set action as current action for armature
		actions[action].setActive (armature)

		frames = []

		#Get all frames
		current_frame = 1
		end_frame = actions[action].getFrameNumbers()[1]
		while current_frame < end_frame:
			#Set current frame
			scene.makeCurrent()
			Blender.Set ("curframe", current_frame)
			Blender.Window.Redraw()
			armaturepose = armature.getPose()

			bones = []

			#Get all posed bones
			for bone in armaturepose.bones.values():
				#If bone has a parent, get the relative transformation of the bone
				if bone.parent:
					parent_matrix = Blender.Mathutils.Matrix (bone.parent.poseMatrix)
					transformation = bone.poseMatrix * parent_matrix.invert()
				else:
					transformation = bone.poseMatrix

				#Get the parent index (starting at 1; 0 is none)
				has_parent = False
				parent_index = 1
				for i in armaturepose.bones.values():
					if bone.parent == i:
						has_parent = True
						break
					parent_index += 1

				#If bone does not have a parent, use 0 (none) as the parent index
				if has_parent == False:
					parent_index = 0

				bones.append ([transformation, parent_index])

			frames.append (bones)
			current_frame += 1
		animations.append ([action, frames])
	return animations

def ExportArmatureFrame (obj, frame, outfile):
	#Write amount of armatures
	outfile.write (struct.pack ('c', chr (len (frame))))
	#Write all armature data
	for bone in frame:
		matrix = bone[0]
		quaternion = matrix.toQuat()
		#Write rotation quaternion
		outfile.write (struct.pack ('>4f', quaternion[0], quaternion[1], quaternion[2], quaternion[3]))
		#Write translation
		outfile.write (struct.pack ('>3f', matrix[3][0], matrix[3][1], matrix[3][2]))
		#Write parent
		outfile.write (struct.pack ('c', chr (bone[1])))

def ExportArmatureAnimation (obj, scene, armature, outfile):
	mesh = obj.getData (mesh = 1)

	#Construct base frame
	base_frame = []
	for bone in armature.getData().bones.values():
		#If bone has a parent, get the relative transformation of the bone
		if bone.parent:
			parent_matrix = Blender.Mathutils.Matrix (bone.parent.matrix["ARMATURESPACE"])
			transformation = bone.matrix["ARMATURESPACE"] * parent_matrix.invert()
		else:
			transformation = bone.matrix["ARMATURESPACE"]

		#Get the parent index (starting at 1; 0 is none)
		has_parent = False
		parent_index = 1
		for i in armature.getData().bones.values():
			if bone.parent == i:
				has_parent = True
				break
			parent_index += 1

		#If bone does not have a parent, use 0 (none) as the parent index
		if has_parent == False:
			parent_index = 0

		base_frame.append ([transformation, parent_index])
	#Write base frame
	ExportArmatureFrame (obj, base_frame, outfile)

	vertices = []
	triangles = []
	animations = GetArmatureAnimations (armature, scene)

	#Get all vertices
	for vertex in mesh.verts:
		influences = mesh.getVertexInfluences (vertex.index)
		#Sum all influences (up to 4 supported), as Blender influence does not always sum to 1.0
		sum = 0.0
		i = 0
		for influence in influences:
			if i >= 4:
				break
			sum += influence[1] #Weight
			i += 1
		#Get influences
		vertex_influences = []
		i = 0
		for influence in influences:
			if i >= 4:
				break
			#Get influence bone indices
			bone_index = 1 #Starts at 1, as armature indices start at 1
			for bone in armature.getData().bones.values():
				if bone.name == influence[0]:
					break
				bone_index += 1
			#Add influence and bone index to list
			if sum != 0.0:
				vertex_influences.append ([influence[1] / sum, bone_index])
			else:
				vertex_influences.append ([influence[1], bone_index])
			i += 1
		#If there are less than 4 influences, fill in the rest
		while len (vertex_influences) < 4:
			vertex_influences.append ([0.0, 0])
		#Add vertex data to vertex list
		vertices.append ([vertex.co.x, vertex.co.z, vertex.co.y, 0.0, 0.0, 0.0, 0.0, 0.0, vertex_influences])

	#Get all triangles
	for face in mesh.faces:
		#Apply texture coordinates to vertices if valid
		if mesh.faceUV:
			i = 0
			for vertex in face.verts:
				vertices[vertex.index][6] = face.uv[i][0]
				vertices[vertex.index][7] = face.uv[i][1]
				i += 1
		#Get first triangle in face
		triangles.append ([face.verts[0].index, face.verts[1].index, face.verts[2].index])
		#If this face is a quad, get the second triangle
		if len (face.verts) == 4:
			triangles.append ([face.verts[0].index, face.verts[2].index, face.verts[3].index])

	#Write amount of vertices, triangles, and animations
	outfile.write (struct.pack ('>H', len (vertices)))
	outfile.write (struct.pack ('>H', len (triangles)))
	outfile.write (struct.pack ('>H', len (animations)))

	#Write all vertices
	for vertex in vertices:
		outfile.write (struct.pack ('>8f', vertex[0], vertex[1], vertex[2], vertex[3], vertex[4], vertex[5], vertex[6], vertex[7]))
		for influence in vertex[8]:
			outfile.write (struct.pack ('>f', influence[0]))
		for influence in vertex[8]:
			outfile.write (struct.pack ('c', chr (influence[1])))
	#Write all triangles
	for triangle in triangles:
		outfile.write (struct.pack ('>3H', triangle[0], triangle[1], triangle[2]))

	for animation in animations:
		#Write animation name
		outfile.write (struct.pack ('>H', len (animation[0])))
		outfile.write (animation[0])
		#Write frame rate
		outfile.write (struct.pack ('c', chr (10)))
		#Write amount of frames
		outfile.write (struct.pack ('>H', len (animation[1])))
		#Write frame
		for frame in animation[1]:
			ExportArmatureFrame (obj, frame, outfile)

def ExportCSM (filename):
	#Get active mesh in active scene
	scene = bpy.data.scenes.active
	obj = scene.objects.active
	mesh = obj.getData (mesh = 1)

	vertices = []
	triangles = []
	texture = None

	#Get texture
	if len (mesh.faces) > 0:
		for face in mesh.faces:
			try:
				texture = face.image
				#Define file path of texture output
				directory = os.path.dirname (filename) + '/'
				texture_filename = 'texture/' + texture.name
				#Create necessary directories if they do not exist
				if not os.path.exists (directory + os.path.dirname (texture_filename)):
					os.makedirs (directory + os.path.dirname (texture_filename))
				#Copy texture
				shutil.copyfile (texture.filename, directory + texture_filename)
				break
			except ValueError:
				pass

	#Get armature
	if obj.parent:
		armature = obj.parent
	else:
		armature = None

	#Open output file
	outfile = open (filename, 'wb')

	#Write identifier
	outfile.write ("CitrineSM")
	#Write file version
	outfile.write (struct.pack ('>c', chr (0)))
	outfile.write (struct.pack ('>c', chr (0)))
	#Write animation type
	if armature != None:
		outfile.write (struct.pack ('>c', chr (1)))
	else:
		outfile.write (struct.pack ('>c', chr (0)))
	#Write texture string size and string
	if texture != None:
		outfile.write (struct.pack ('>H', len (texture_filename)))
		outfile.write (texture_filename)
	else:
		outfile.write (struct.pack ('>H', 0))

	if armature != None:
		ExportArmatureAnimation (obj, scene, armature, outfile)
	else:
		ExportVertexAnimation (obj, outfile)

	outfile.close()

if __name__ == '__main__':
	Blender.Window.FileSelector (ExportCSM, "Export")
