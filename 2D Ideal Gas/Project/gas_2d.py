#Object-oriented implementation of a hard-disks molecular dynamics simulations
from itertools import combinations
from numpy import pi
class Vector():
    '''2D vectors'''

    def __init__(self, i1,i2):
        '''Initialise vectors with x and y coordinates'''
        self.x = i1
        self.y = i2

    def __add__(self,other):
        '''Use + sign to implement vector addition'''
        return (Vector(self.x+other.x,self.y+other.y))

    def __sub__(self,other):
        '''Use - sign to implement vector "subtraction"'''
        return (Vector(self.x-other.x,self.y-other.y))

    def __mul__(self,number):
        '''Use * sign to multiply a vector by a scaler on the left'''
        return Vector(self.x*number,self.y*number)

    def __rmul__(self,number):
        '''Use * sign to multiply a vector by a scaler on the right'''
        return Vector(self.x*number,self.y*number)

    def __truediv__(self,number):
        '''Use / to multiply a vector by the inverse of a number'''
        return Vector(self.x/number,self.y/number)

    def __repr__(self):
        '''Represent a vector by a string of 2 coordinates separated by a space'''
        return '{x} {y}'.format(x=self.x, y=self.y)

    def copy(self):
        '''Create a new object which is a copy of the current.'''
        return Vector(self.x,self.y)

    def dot(self,other):
        '''Calculate the dot product between two 2D vectors'''
        return self.x*other.x + self.y*other.y

    def norm(self):
        '''Calculate the norm of the 2D vector'''
        return (self.x**2+self.y**2)**0.5

class Particle():
    def __init__(self, position, momentum, radius, mass):
        
        self.position = position
        self.momentum = momentum
        self.radius = radius
        self.mass = mass
    def velocity(self):
        return self.momentum/self.mass
    def copy(self):
        return Particle(self.position, self.momentum, self.mass, self.radius)
    def overlap(self, other_particle):
        if (self.position - other_particle.position).norm() < (self.radius + other_particle.radius):
            return True
        else:
            return False

class Simulation():
    def __init__(self, particles, box_length, dt):
        self.particles = particles
        self.box_length = box_length
        self.dt = dt
        self.trajectory = [self.record_state()]
        Area=0
        for i in particles:
            A=i.radius**2*pi
            Area=Area+A
        self.density=Area/(box_length**2)
    def apply_particle_collision(self, p1, p2):
        if (p2.position-p1.position).norm()!=0:
            c_axis=(p2.position-p1.position)/(p2.position-p1.position).norm()
            para_mom1 = p1.momentum.dot(c_axis)*c_axis
            para_mom2 = p2.momentum.dot(c_axis)*c_axis
            perp_mom1=p1.momentum-para_mom1
            perp_mom2=p2.momentum-para_mom2
            if p1.overlap(p2) == True:
                if (p2.momentum-p1.momentum).dot(c_axis) <0:
                    p2.momentum = ((((p2.mass-p1.mass)*para_mom2)+(2*p2.mass*para_mom1))/(p1.mass+p2.mass)) + perp_mom2
                    p1.momentum = ((((p1.mass-p2.mass)*para_mom1)+(2*p1.mass*para_mom2))/(p1.mass+p2.mass)) + perp_mom1
    def apply_box_collisions(self,particle):
        if ((particle.position.x > self.box_length and particle.momentum.x>0) or 
            (particle.position.x < 0 and particle.momentum.x<0)):
            particle.momentum.x = particle.momentum.x*-1
        elif ((particle.position.y > self.box_length and particle.momentum.y>0) or 
              (particle.position.y < 0 and particle.momentum.y<0)):
            particle.momentum.y = particle.momentum.y * -1
    def step(self):
        for i in range(len(self.particles)):
            self.particles[i].position = self.particles[i].position + self.particles[i].velocity()*self.dt
            self.apply_box_collisions(self.particles[i])
        for p1, p2 in combinations(self.particles,2):
            self.apply_particle_collision(p1,p2)
        self.trajectory = self.trajectory + [self.record_state()]
    def record_state(self):
        state = []
        for i in range(len(self.particles)):
            state = state + [self.particles[i].copy()]
        return state

#cut

