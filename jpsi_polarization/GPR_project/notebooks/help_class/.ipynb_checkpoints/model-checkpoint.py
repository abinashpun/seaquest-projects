"""
#
# Abinash Pun
# Imported from Dinupa's work
"""
import ROOT
from ROOT import TH1D, TCanvas

import numpy as np
from sklearn.gaussian_process import GaussianProcessRegressor
from sklearn.gaussian_process.kernels import RBF, Matern, RationalQuadratic, ExpSineSquared, DotProduct, WhiteKernel, ConstantKernel as C
from sklearn.preprocessing import StandardScaler, Normalizer

#from  help_class.model import model
class gpr_model:
    data_train = []#3D array 0:- bin center (X), 1:-bin value (Y), 2: bin error Y_error
    data = []
    def __init__(self):
        self

    #sb:[left_band_start, left_band_end, right_band_start, right_band_end],
    #data: 3D array 0:- bin center (X), 1:-bin value (Y), 2: bin error Y_error
    def set_sideband(self, sideband, data): 
        self.data = data
        mask = (((sideband[0] <= data[:,0]) & (data[:,0] <= sideband[1])) | ((sideband[2] <= data[:,0]) & (data[:,0] <= sideband[3])))
        self.data_train = data[mask]
        
    # use RBF kernel(only for now)
    # range1 && range2 are tuples
    def set_kernel(self, val1, range1, length_scale, range2):
        self.kernel = C(val1, range1)* RBF(length_scale, range2)
        
    def predict(self, niter):

        kernel = self.kernel
        data_train = self.data_train
        data = self.data
        M = GaussianProcessRegressor(kernel=kernel, alpha=data_train[:, 2]**2, n_restarts_optimizer=niter)
        M.fit(np.atleast_2d(data_train[:, 0]).T, data_train[:, 1])

        print("kernel : ", M.kernel_)
        print("score = ", M.score(np.atleast_2d(data[:, 0]).T, data[:, 1]))
        print(f"Log-likelihood: {M.log_marginal_likelihood(M.kernel_.theta):.3f}")
        
        y_pred, sigma = M.predict(np.atleast_2d(data[:, 0]).T, return_std=True)
        Y_pred, cov_matrix = M.predict(np.atleast_2d(data[:, 0]).T, return_cov=True)
        sigma2 = np.sum(cov_matrix, axis=0)
        gpr_error = np.sqrt(abs(sigma2))
        return y_pred,sigma, gpr_error
      