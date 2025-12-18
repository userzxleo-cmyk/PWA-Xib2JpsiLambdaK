from typing import List
import numpy as np

class Node:
    def __init__(self, variables: List[np.array], weights: np.array, dimension: int, min_significance: float):
        self.split_indices(variables, weights, dimension, min_significance)
        if self.splitable:
            left_indices = variables[dimension] <= self.split
            right_indices = np.logical_not(left_indices)

            left_variables = [ variable[left_indices] for variable in variables ]
            right_variables = [ variable[right_indices] for variable in variables ]
            left_weights = weights[left_indices]
            right_weights = weights[right_indices]
            next_dimension = (dimension + 1) % len(variables)

            self.less = Node(left_variables, left_weights, next_dimension, min_significance)
            self.greater = Node(right_variables, right_weights, next_dimension, min_significance)

    def split_indices(self, variables: List[np.array], weights: np.array, dimension: int, min_significance: float):

        sorted_indices = np.argsort(variables[dimension])
        sorted_weights = weights[sorted_indices]
        weights_squared = np.square(sorted_weights)

        scan = np.cumsum(sorted_weights)
        inverted_scan = scan[-1] - scan

        scan_w2 = np.cumsum(weights_squared)
        inverted_scan_w2 = scan_w2[-1] - scan_w2

        left_significance = scan / np.sqrt(scan_w2)
        right_significance = inverted_scan / np.sqrt(inverted_scan_w2)

        considered_cut_points = np.logical_and(left_significance >= min_significance, right_significance >= min_significance)

        # Cannot split further
        if not(np.any(considered_cut_points)):
            self.splitable = False
        else:
            # significance = left_significance * right_significance
            asymmetry = (left_significance - right_significance) / (left_significance + right_significance)
            metric = 1 - np.square(asymmetry)

            # Ignore cut points where both samples are below significance threshold by setting them to 0
            to_ignore = np.logical_not(considered_cut_points)
            metric[to_ignore] = 0

            split_index = np.argmax(metric)
            split_value = variables[dimension][sorted_indices][split_index] # less than or equal

            self.split = split_value
            self.split_dim = dimension
            self.splitable = True

class KDTree:
    def __init__(self, variables: List[np.array], weights: np.array, min_significance: float):
        self.tree = Node(variables, weights, 0, min_significance)
