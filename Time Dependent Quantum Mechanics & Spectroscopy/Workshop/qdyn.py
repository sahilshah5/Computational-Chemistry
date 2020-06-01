#This file contains functions to display and propagate a wavefunctions in 1D

import numpy as np
from numpy.fft import fft,ifft,fftshift,ifftshift

import matplotlib.pyplot as plt
import matplotlib.animation as animation

def animate_dynamics(x,dyn,dt,V_array,magnitude=True,real=True,imaginary=False,V_ylim=(0,1),psi_ylim=(-1,1),frame_delay=200,**options):
    """Generate an animation of the wavefunction evolving with time overlaped with the potential.
    
    Mandatory arguments
    x: 1D array of equally spaced points in space.
    dyn: 2D array with wavefunction values at several instants in time.
    dt: time step in atomic units.
    V_array: 1D array with the values of the potential a the points x.
    
    Optional keyword arguments
    magnitude = bool: show magnitude of the wavefunction (default:True).
    real = bool: show real part of the wavefunction (default=True).
    imaginary = bool: show imaginary part of the wavefunction (default=False).
    V_ylim = (num,num): set y axis limits for the potential plot.
    psi_ylim = (num,num): set y axis limits for the wavefunction plot.
    frame_delay = num: set the delay between frames in milliseconds (default:200).
    
    Any other keywords will be passed as plot options (you can use for example
    xlim=(num,num)).
    """
    
    fig=plt.figure()
    #create axes for potential plot
    potplot=plt.gca()
    potplot.plot(x,V_array,color="gray")
    potplot.set(xlabel="$x/a_0$",ylabel="$V/E_h$",ylim=V_ylim,**options)

    # create another axes set for the wavefunction sharing the x axis
    psiplot=potplot.twinx()
    
    def dyn_plot(step,sim_array):
        psiplot.cla() # clear current axes
        if magnitude:
            psiplot.plot(x,np.abs(sim_array[step]),color="black",label="$|\\Psi|$")
        if real:
            psiplot.plot(x,np.real(sim_array[step]),color="blue",label="$Re(\\Psi)$")
        if imaginary:
            psiplot.plot(x,np.imag(sim_array[step]),color="red",label="$Im(\\Psi)$")
        psiplot.text(0.1,0.9,"t="+str(step*dt)+"au",transform = psiplot.transAxes)
        psiplot.set(ylabel="$\\psi$",ylim=psi_ylim,**options)
        plt.legend(loc="upper right")
        return psiplot

    return animation.FuncAnimation(fig,dyn_plot,frames=len(dyn),interval=frame_delay,fargs=(dyn,))

def kinetic_prop(psi_x,m,dx,dt):
    """Calculate the kinetic part of the split-operator propagator: e**(-j*K*dt),
    where K is the kinetic energy operator.
    
    Arguments
    psi_x: array with wavefunction on which the propagator is operating.
           It has the values of the wavefunction in a 1D space grid.
    m: mass of the system.
    dx: spacing of the points in the space grid.
    dt: time step.
    
    The function works by doing the Fourier transform of psi_x, applying K on the
    momentum representation and transforming back via inverse Fourier transform.
    """
    
    # Fourier transform the entry vector
    psi_k=fftshift(fft(psi_x,norm="ortho"))
    
    # calculate resolution in k
    n=len(psi_x)
    dk=2*np.pi/(n*dx)
    
    # operate the exponential kinetic operator on psi_k
    k_sq=(np.linspace(np.floor(-n/2+1),int(n/2),n)*dk)**2
    expK_k=np.exp(-1j*(k_sq*dt)/(2*m))
    
    prop_psi_k=expK_k*psi_k
    
    # transform back to space representation
    prop_psi_x=ifft(ifftshift(prop_psi_k),norm="ortho")
    
    return prop_psi_x

def propagator(x_grid,psi,m,dt,V_func,*args):
    """Propagate the 1D wavefunction over one time step.
    
    Arguments
    x_grid: array with a 1D grid of equally spaced points in space.
    psi: array with the values of the wavefunction in x_grid.
    m: the mass of the system.
    dt: the time step.
    V_func: the name of the function that defines the potential
    *args: if V_func(x_grid,a,b) requires for example 2 parameters
           a and b besides the position x_grid, these should be passed
           in order after the function name.
           
    The function implements a split-operator procedure in a
    Fourier pseudospectral basis.
    """
    
    # set potential part of the propagator
    expV=np.exp(-1j*V_func(x_grid,*args)*dt/2)
    
    # operate with potential part first
    psi2=expV*psi
    
    # operate with kinetic part
    psi3=kinetic_prop(psi2,m,x_grid[1]-x_grid[0],dt)
    
    # operate with potential part again
    psi4=expV*psi3
    
    return psi4
