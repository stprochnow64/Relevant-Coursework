'''
Specifies a MazeProblem as parameterized by a given grid maze,
assuming that an agent's legal actions can move them one tile in
any cardinal direction
'''

from environment import *

class MazeProblem:

    ##################################################################
    # Class Constants
    ##################################################################

    # Static COST_MAP for maze components and the cost to move onto them
    # Any component not listed assumed to have a cost of 1
    # HINT: You can add block types to this!
    COST_MAP = {Environment.PIT_BLOCK: Environment.PIT_PENALTY, Environment.SAFE_BLOCK: Environment.MOV_PENALTY, Environment.LKLY_BLOCK: Environment.LKLY_PENALTY, Environment.WRN_BLOCK: Environment.WRN_PENALTY}


    ##################################################################
    # Constructor
    ##################################################################

    def __init__(self, maze):
        """
        Constructs a new pathfinding problem from a maze
        :maze: a list of list of strings containing maze elements
        """
        self.maze = maze


    ##################################################################
    # Methods
    ##################################################################

    def transitions(self, state):
        """
        Given some state s, the transitions will be represented as a list of tuples
        of the format:
        [(action1, cost_of_action1, result(action1, s)), ...]
        For example, if an agent is at state (1, 1), and can only move right and down
        into clear tiles (.), then the transitions for that s = (1, 1) would be:
        [("R", 1, (2, 1)), ("D", 1, (1, 2))]
        :state: A maze location tuple
        """
        s = state
        possible = [("U", (s[0], s[1]-1)), ("D", (s[0], s[1]+1)), ("L", (s[0]-1, s[1])), ("R", (s[0]+1, s[1]))]
        return [(s[0], self.cost(s[1]), s[1]) for s in possible if self.maze[s[1][1]][s[1][0]] != Environment.WALL_BLOCK]

    def cost(self, state):
        """
        Returns the cost of moving onto the given state, and employs
        the MazeProblem's COST_MAP
        :state: A maze location tuple
        """
        cm = MazeProblem.COST_MAP
        cell = self.maze[state[1]][state[0]]
        return cm[cell] if cell in cm else 1
