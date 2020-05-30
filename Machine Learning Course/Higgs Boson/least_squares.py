# -*- coding: utf-8 -*-
"""Exercise 3.

Least Square
"""

import numpy as np


def least_squares(y, tx):
    """calculate the least squares solution."""
    N = len(y)
    w = np.linalg.solve(tx.transpose().dot(tx), tx.transpose().dot(y))
    error = np.sum(y-tx.dot(w.transpose()))/N
    return [w, error]
