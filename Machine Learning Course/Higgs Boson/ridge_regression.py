# -*- coding: utf-8 -*-
"""Exercise 3.

Ridge Regression
"""

import numpy as np


def ridge_regression(y, tx, lambda_):
    """implement ridge regression."""
    N = len(y)
    w = np.linalg.solve(tx.T.dot(tx) + lambda_ * np.eye(tx.shape[1]), tx.T.dot(y))
    e = y-tx.dot(w.transpose())
    error = e.transpose().dot(e)/N
    return [w, error]
