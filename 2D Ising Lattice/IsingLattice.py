import numpy as np

class IsingLattice:

    E = 0.0
    E2 = 0.0
    M = 0.0
    M2 = 0.0


    def __init__(self, n_rows, n_cols):
        self.n_rows = n_rows
        self.n_cols = n_cols
        self.lattice = np.random.choice([-1,1], size=(n_rows, n_cols))
        self.n_cycles = 0
        self.list = []
        self.avgE = False
        self.newn=0

    def energy(self):
        "Return the total energy of the current lattice configuration."
        energy = -np.sum(np.multiply(self.lattice, np.roll(self.lattice, (1,0)))) - np.sum(np.multiply(self.lattice, np.roll(self.lattice, (1,1))))
        return energy

    def magnetisation(self):
        "Return the total magnetisation of the current lattice configuration."
        magnetisation = np.sum(self.lattice)
        return magnetisation

    def montecarlostep(self, T):
        self.n_cycles = self.n_cycles + 1
        # complete this function so that it performs a single Monte Carlo step
        energy0 = self.energy()
        #the following two lines will select the coordinates of the random spin for you
        random_i = np.random.choice(range(0, self.n_rows))
        random_j = np.random.choice(range(0, self.n_cols))
        self.lattice[random_i][random_j] = -self.lattice[random_i][random_j]
        energy1 = self.energy()
        deltaE = energy1-energy0
        #the following line will choose a random number in the range [0,1) for you
        random_number = np.random.random()

        
        self.list = self.list + [self.energy()]
        weight = 8
        threshold = weight * self.n_rows * self.n_cols
        if self.n_cycles > threshold and self.avgE == False:
            s1 = np.mean(np.array(self.list[(self.n_cycles - 3*threshold):int(self.n_cycles - 2*threshold)]))
            s2 = np.mean(np.array(self.list[int(self.n_cycles - 2*threshold):int(self.n_cycles - threshold)]))
            s3 = np.mean(np.array(self.list[int(self.n_cycles - threshold):self.n_cycles]))
            if np.std(np.array([s1,s2,s3])) < 0.01*T:
                self.avgE = True
                
        if deltaE>0 and random_number > np.exp(-deltaE/T):
            self.lattice[random_i][random_j] = - self.lattice[random_i][random_j]
            if self.avgE == False:
                self.list.append(energy0)
            magnetisation=self.magnetisation()
            if self.avgE == True or self.n_cycles > 80000:
                self.newn+=1
                self.E = self.E + float(energy0)
                self.E2 = self.E2 + float(energy0**2)
                self.M = self.M + float(magnetisation)
                self.M2 = self.M2 + float(magnetisation**2)
            return float(energy0), float(magnetisation)
                    
        else:
            self.list.append(energy1)
            magnetisation=self.magnetisation()
            if self.avgE == True or self.n_cycles > 80000:
                self.newn+=1
                self.E = self.E + float(energy1)
                self.E2 = self.E2 + float(energy1**2)
                self.M = self.M + float(magnetisation)
                self.M2 = self.M2 + float(magnetisation**2)
            return float(energy1), float(magnetisation)

        
    def statistics(self):
        # complete this function so that it calculates the correct values for the averages of E, E*E (E2), M, M*M (M2), and returns them
        print(self.n_cycles-self.newn)
        return (self.E/self.newn, self.E2/self.newn, self.M/self.newn, self.M2/self.newn)
