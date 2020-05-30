from numpy import arccos

class Vector:
	'Implements 3D vectors and their behaviour'
	def __init__(self,i1,i2,i3):
		self.x=i1
		self.y=i2
		self.z=i3
	def scale(self,a):
		x=self.x*a
		y=self.y*a
		z=self.z*a
		return Vector(x,y,z)
	def dot(self,u):
		a=self.x*u.x
		b=self.y*u.y
		c=self.z*u.z
		return a+b+c
	def norm(self):
		a=self.x**2
		b=self.y**2
		c=self.z**2
		return ((a)+(b)+(c))**0.5
	def __add__(self,v):
		x=self.x+v.x
		y=self.y+v.y
		z=self.z+v.z
		return Vector(x,y,z)
	def __sub__(self,v):
		x=self.x-v.x
		y=self.y-v.y
		z=self.z-v.z
		return Vector(x,y,z)
	def copy(self):
		return Vector(self.x,self.y,self.z)
	def __repr__(self):
		return str(self.x)+' '+str(self.y)+' '+str(self.z)
		
class Molecule:
	def __init__(self, atoms, positions, charges):
		self.symbols=atoms
		self.positions=positions
		self.charges=charges
	def bond_length(self, i1,i2):
		dist=self.positions[i1]-self.positions[i2]
		return dist.norm()
	def bond_angle(self,i1,i2,i3):
		bond1=self.positions[i2]-self.positions[i1]
		bond2=self.positions[i3]-self.positions[i1]
		angle=arccos((bond1.dot(bond2))/(bond1.norm()*bond2.norm()))
		return angle
	def dipole_moment(self):
		prods=Vector(0,0,0)
		for i in range(len(self.positions)):
			j=self.positions[i].scale(self.charges[i])
			prods=prods+j
		return prods