#!BPY

"""
Name: 'Citrine Standard Environment (.cse)'
Blender: 244
Group: 'Export'
Tooltip: 'Export Citrine Standard Environment File'
"""

# Copyright (C) 2010 Braden Walters
#
# This software may be modified and distributed under the terms of the MIT
# license. See the LICENSE file for details.

import bpy
import Blender
from Blender import *
import os
import csm_export
from csm_export import *
import struct

def ExportCSE (filename):
	scene = bpy.data.scenes.active

	objects = []

	#Iterate through all objects in scene
	for object in scene.objects:
		#Skip objects that are not mesh, lamps, or cameras
		if type (object.getData()) != Blender.Types.NMeshType and type (object.getData()) != Blender.Types.LampType and type (object.getData()) != Blender.Types.CameraType:
			continue

		#Swap y and z rotations
		euler = object.matrix.toEuler()
		swapped_euler = euler.copy()
		swapped_euler.y = euler.z
		swapped_euler.z = -euler.y
		rotation_matrix = swapped_euler.toMatrix()
		#Swap y and z scale
		scale = [object.matrix.scalePart()[0], object.matrix.scalePart()[2], object.matrix.scalePart()[1]]
		#Swap y and z translations
		translation = [object.matrix[3][0], object.matrix[3][2], object.matrix[3][1]]
		obj = [object.name, object.type, rotation_matrix.toQuat(), object.matrix.scalePart(), translation]

		if object.type == 'Mesh':
			#Determine file path of CSM output
			directory = os.path.dirname (filename) + '/'
			csm_filename = 'meshdata/' + object.getData().name + '.csm'
			#Create necessary directories if they do not exist
			if not os.path.exists (directory + os.path.dirname (csm_filename)):
				os.makedirs (directory + os.path.dirname (csm_filename))
			#Add mesh properties to object properties
			obj.append (csm_filename)
			#Make this mesh active so ExportCSM uses it
			scene.objects.active = object
			ExportCSM (directory + csm_filename)
		if object.type == 'Lamp':
			#Default to positional light
			obj.append (chr (1))
			#Add light intensity to object properties
			obj.append (object.getData().energy / 10.0)
			#Add light color to object properties
			obj.append (object.getData().col)

		objects.append (obj)

	#Open output file
	outfile = open (filename, 'wb')

	#Write identifier
	outfile.write ("CitrineSE")
	#Write file version
	outfile.write (struct.pack ('>c', chr (0)))
	outfile.write (struct.pack ('>c', chr (0)))
	#Write amount of content
	outfile.write (struct.pack ('>I', len (objects)))

	for object in objects:
		#Write name string size and string
		outfile.write (struct.pack ('>H', len (object[0])))
		outfile.write (object[0])

		#Write type
		if object[1] == 'Empty':
			outfile.write (struct.pack ('c', chr (0)))
		elif object[1] == 'Mesh':
			outfile.write (struct.pack ('c', chr (1)))
		elif object[1] == 'Camera':
			outfile.write (struct.pack ('c', chr (2)))
		elif object[1] == 'Lamp':
			outfile.write (struct.pack ('c', chr (3)))

		#Write rotation
		rotation = object[2]
		outfile.write (struct.pack ('>4f', rotation[0], rotation[1], rotation[2], rotation[3]))

		#Write scale
		scale = object[3]
		outfile.write (struct.pack ('>3f', scale[0], scale[1], scale[2]))

		#Write translation
		translation = object[4]
		outfile.write (struct.pack ('>3f', translation[0], translation[1], translation[2]))

		#Write mesh-specific data if valid
		if object[1] == 'Mesh':
			outfile.write (struct.pack ('>H', len (object[5])))
			outfile.write (object[5])
		#Write light-specific data if valid
		if object[1] == 'Lamp':
			outfile.write (struct.pack ('c', object[5]))
			outfile.write (struct.pack ('>f', object[6]))
			outfile.write (struct.pack ('>3f', object[7][0], object[7][1], object[7][2]))

	outfile.close()

if __name__ == '__main__':
	Blender.Window.FileSelector (ExportCSE, "Export")
