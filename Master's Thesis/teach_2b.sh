#!/bin/bash

export PATH=$PATH:~/QUIP/build/linux_x86_64_gfortran
export OMP_NUM_THREADS=$NSLOTS

GP_FILE=gp-merig-cc-2b.xml
AT_FILE=me-rigid-shortaug3-gscc.xyz

teach_sparse at_file=$AT_FILE descriptor_str={ \
    distance_2b cutoff=10.0 covariance_type=ARD_SE n_sparse=50 sparse_method=uniform Z1=6 Z2=6 theta_fac=0.2 delta=0.0005 resid_name=resid only_inter=T:\
    distance_2b cutoff=6.0 covariance_type=ARD_SE n_sparse=50 sparse_method=uniform Z1=1 Z2=6 theta_fac=0.2 delta=0.0005 resid_name=resid only_inter=T:\
    distance_2b cutoff=6.0 covariance_type=ARD_SE n_sparse=50 sparse_method=uniform Z1=1 Z2=1 theta_fac=0.2 delta=0.0005 resid_name=resid only_inter=T:} \
    default_sigma={0.00001 0.0 0.0} sparse_jitter=1e-10 gp_file=$GP_FILE do_copy_at_file=F sparse_separate_file=F energy_parameter_name=ediff_cc force_parameter_name=none e0=0.0
