'''
BlindBot MazeAgent meant to employ Propositional Logic,
Search, Planning, and Active Learning to navigate the
Maze Pitfall problem
'''

import time
import random
from maze_problem import *
from environment import *
from maze_clause import *
from maze_knowledge_base import *
from pathfinder import *
from queue import Queue

# [!] TODO: import your Part 1 here!

MOVES = ["U", "D", "L", "R"]

class MazeAgent:

    ##################################################################
    # Constructor
    ##################################################################

    def __init__ (self, env):
        self.env = env
        self.loc = env.get_player_loc()

        # The agent's maze can be manipulated as a tracking mechanic
        # for what it has learned; changes to this maze will be drawn
        # by the environment and is simply for visuals
        self.maze = env.get_agent_maze()

        # The agent's plan will be a queue storing the sequence of
        # actions that the environment will execute
        self.plan = Queue()
        # [!] TODO: Initialize any other knowledge-related attributes for
        # agent here, or any other record-keeping attributes you'd like

        self.goal = Environment.get_goal_loc(self.maze)
        self.safe_set = Environment.init_known_safe(self.maze)
        self.ubs_set = set()
        self.brz_set = set()
        self.wall_set = Environment.init_wall_set(self.maze)
        self.poss_pit_set = set()

        self.kb = MazeKnowledgeBase()

        # Add to KB the clause that one of the blocks around the goal must be free
        goal_adj = Environment.get_adjacent(self.goal)
        goal_kb = []
        for a in goal_adj:
            if a in self.ubs_set:
               temp_clause = (("P", a), False)
               goal_kb.append(temp_clause)
        goal_mc = MazeClause(goal_kb)
        self.kb.tell(goal_mc)

       # Add adj tiles to starting tile to safe set and ubs set
        start_adj = Environment.get_adjacent(self.loc)
        for a in start_adj:
            if a not in self.wall_set:
                self.ubs_set.add(a)
                safe_mc = Environment.safe_in_kb(a)
                self.kb.tell(safe_mc)

    ##################################################################
    # Methods
    ##################################################################

    # [!] TODO! Agent currently just runs straight up
    def think(self, perception):
        """
        think is parameterized by the agent's perception of the tile type
        on which it is now standing, and is called during the environment's
        action loop. This method is the chief workhorse of your MazeAgent
        such that it must then generate a plan of action from its current
        knowledge about the environment.

        :perception: A dictionary providing the agent's current location
        and current tile type being stood upon, of the format:
          {"loc": (x, y), "tile": tile_type}
        """
        # Agent simply moves randomly at the moment...
        # Do something that thinks about the perception!

    #Initialize need to know vars
        loc = perception['loc']
        x = loc[0]
        y = loc[1]
        print ('X ', x)
        t_type = perception['tile']
        goal_loc = self.goal
        mp = MazeProblem(self.maze)
        adj = Environment.get_adjacent(loc)

# If current tile is safe, pathfind to goal and put the path into the plan
# If path to goal is < 4, ignore all other criteria and just go to goal (possible pits but worth it)
# If current tile is in unknown but safe set, remove it
# Mark all adjacent tiles that are ? as unknown but safe
        if t_type == '.':
            path = pathfind(mp, loc, self.goal)
            if path[0] < 4:
                go_to_goal(path, self.plan)
            clear_plan(self.plan)
            self.ubs_set.discard(loc)
            self.safe_set.add(loc)
            for a in adj:
                if a not in self.wall_set and a not in self.safe_set and a not in self.brz_set:
                    self.ubs_set.add(a)
            rules = path[1]
            for r in rules:
                self.plan.put(r)

# If current type is B, clear path
# Check if goal is < 4 spots away, if so, go to goal with no regard for B or P
# Remove from unknown but safe set and add to breeze set
        if t_type == 'B':
            clear_plan(self.plan)
            path = pathfind(mp, loc, self.goal)
            if path[0] < 4:
                go_to_goal(path, self.plan)
            self.ubs_set.discard(loc)
            self.brz_set.add(loc)

            for a in adj:
                if a in self.poss_pit_set:
                    maze
                self.poss_pit_set.add(a)

# Pathfind to nearest unknown but safe square
            current_min = 1000
            closest = loc
            for element in self.ubs_set:
                check = pathfind(mp, loc, element)
                if check[0] < current_min:
                    current_min = check[0]
                    closest = element
            path = pathfind(mp, loc, closest)
            rules = path[1]
            for r in rules:
                self.plan.put(r)

        if self.ubs_set == set():
            clear_plan(self.plan)
            path = pathfind(mp, loc, self.goal)
            go_to_goal(path, self.plan)


        print ('UBS: ', self.ubs_set)
        print ('SAFE: ', self.safe_set)
        print ('BREEZE: ', self.brz_set)


    def get_next_move(self):
        """
        Returns the next move in the plan, if there is one, otherwise None
        [!] Do NOT modify this method
        """
        return None if self.plan.empty() else self.plan.get()


def _get_solution(node):
    """
    Returns a solution (a sequence of actions) from the given
    SearchTreeNode node, presumed to be a goal
    :node: A goal SearchTreeNode in the A* Search Tree
    """
    soln = []
    cum_cost = node.totalCost
    while node.parent is not None:
        soln.append(node.action)
        node = node.parent
    soln.reverse()
    return (cum_cost, soln)

def heuristic(state, goal):
    """
    Implements the Manhattan Distance Heuristic, which (given a state)
    provides the cell-distance to the nearest goal state
    :state: A maze location tuple
    :goal: A maze location tuple
    """
    return abs(state[0] - goal[0]) + abs(state[1] - goal[1])

def pathfind(problem, start, dest):
    """
    Given a MazeProblem, a start location tuple, and a destination tuple,
    returns the sequence of actions that takes the agent from the start
    to the destination via the A* algorithm
    :problem: A MazeProblem object
    :start: A maze location tuple
    :dest: A maze location tuple
    """
    # Setup
    frontier  = queue.PriorityQueue()
    closedSet = set()

    # Search!
    frontier.put(SearchTreeNode(start, None, None, 0, heuristic(start, dest)))
    while not frontier.empty():
        # Get front node of priority queue
        expanding = frontier.get()

        # Test for goal state
        if expanding.state == dest:
            return _get_solution(expanding)

        # Compute evaluation function for expanded node, f(n) = g(n) + h(n)
        evaluation = expanding.evaluation()

        # Add expanded node to closedSet
        closedSet.add(expanding.state)

        # Generate new nodes on frontier
        for (action, cost, nextState) in problem.transitions(expanding.state):
            childTotalCost = expanding.totalCost + cost
            childHeuristicCost = heuristic(nextState, dest)
            if nextState in closedSet:
                continue
            frontier.put(SearchTreeNode(nextState, action, expanding, childTotalCost, childHeuristicCost))

    # No solution
    return None

def clear_plan(plan):
    while plan.qsize() > 0:
        plan.get()

def go_to_goal(path, plan):
        rules = path[1]
        for r in rules:
            plan.put(r)
