from ..base.tools import resolveRandom, randomProbabilities, checkProbabilities
from ..base.Model import Model
from ast import literal_eval
from numpy import ndarray, array, zeros
from random import choices

class MC(Model):
	def __init__(self, matrix: ndarray, labeling: list, initial_state, name: str ="unknown_MC") -> None:
		"""
		Creates an MC.

		Parameters
		----------
		matrix : ndarray
			A (N x N) ndarray (with N the nb of states).
			Represents the transition matrix.
			`matrix[s1][s2]` is the probability of moving from `s1` to `s2`.
		labeling: list of str
			A list of N observations (with N the nb of states).
			If `labeling[s] == o` then state of ID `s` is labelled by `o`.
			Each state has exactly one label.
		initial_state : int or list of float
			Determine which state is the initial one (then it's the id of the
			state), or what are the probability to start in each state (then it's
			a list of probabilities).
		name : str, optional
			Name of the model.
			Default is "unknow_MC"
		"""
		self.alphabet = list(set(labeling))
		self.labeling = labeling
		super().__init__(matrix,initial_state,name)
		if len(labeling) != self.nb_states:
			raise ValueError("The length of labeling is not equal to the number of states")
		for i in range(self.nb_states):
			if not checkProbabilities(matrix[i]):
				msg = "The probability to take a transition from state "
				msg+= str(i)+" should be 1.0, here it's "+str(matrix[i].sum())
				raise ValueError(msg)
	
	def getLabel(self,state: int) -> str:
		"""
		Returns the label of `state`.

		Parameters
		----------
		state : int
			a state ID

		Returns
		-------
		str
			a label

		Example
		-------
		>>> model.getLabel(2)
		'Label-of-state-2'
		"""
		self._checkStateIndex(state)
		return self.labeling[state]
	
	def getAlphabet(self) -> list:
		"""
		Returns the alphabet of this model.

		Returns
		-------
		list of str
			The alphabet of this model
		
		Example
		-------
		>>> model.getAlphabet()
		['a','b','c','d','done']
		"""
		return self.alphabet
		
	def tau(self,s1: int, s2: int, obs: str) -> float:
		"""
		Returns the probability of moving from state `s1` to `s2` seeing label `obs`.
		(i.e. if `s1` is not labelled with `obs` the probability is 0.0).

		Parameters
		----------
		s1: int
			source state ID.
		s2: int
			destination state ID.
		obs: str
			seen label.
		
		Returns
		-------
		float
			probability of moving from state `s1` to `s2` seeing label `obs`.
		
		Example
		-------
		>>> model.tau(0,1,'a')
		0.6
		>>> model.getLabel(0)
		'a'
		>>> model.tau(0,1,'b')
		0.0
		>>> model.getLabel(1)
		'b'
		"""
		self._checkStateIndex(s1)
		self._checkStateIndex(s2)
		if obs != self.labeling[s1]:
			return 0.0
		return self.matrix[s1][s2]
	
	def a(self,s1: int,s2: int) -> float:
		"""
		Returns the probability of moving from state `s1` to state `s2`.

		Parameters
		----------
		s1 : int
			ID of the source state.		
		s2 : int
			ID of the destination state.
		
		Returns
		-------
		float
			Probability of moving from state `s1` to state `s2`.
		
		Example
		-------
		>>> model.a(0,1)
		0.6
		"""
		self._checkStateIndex(s1)
		self._checkStateIndex(s2)
		return self.matrix[s1][s2]

	def next(self,state: int) -> tuple:
		"""
		Return a state-observation pair according to the distributions described by `matrix`

		Returns
		-------
		output : (int, str)
			A state-observation pair.
			
		Example
		-------
		>>> model.next(0)
		(1,'a')
		>>> model.getLabel(0)
		'a'
		>>> model.next(0)
		(1,'a')
		>>> model.next(0)
		(2,'a')
		>>> model.a(0,1)
		0.6
		>>> model.a(0,2)
		0.4
		"""
		c = resolveRandom(self.matrix[state])
		return (c, self.labeling[state])

	def run(self, number_steps: int, current: int = -1) -> list:
		"""
		Simulates a run of length ``number_steps`` of the model and return the
		sequence of observations generated.
		
		Parameters
		----------
		number_steps: int
			length of the simulation.
		current : int, optional.
			If current it set, it starts from the state `current`.
			Otherwise it starts from an initial state.

		Returns
		-------
		output: list of str
			trace generated by the run.
		"""
		output = []
		if current == -1:
			current = resolveRandom(self.initial_state)

		while len(output) < number_steps:
			[next_state, symbol] = self.next(current)
			output.append(symbol)
			current = next_state
		
		output.append(self.labeling[current])
		return output

	def save(self, file_path:str):
		"""Save the model into a text file.

		Parameters
		----------
		file_path : str
			path of the output file.
		
		Examples
		--------
		>>> model.save("my_model.txt")
		"""
		f = open(file_path, 'w')
		f.write("MC\n")
		f.write(str(self.labeling))
		f.write('\n')
		super()._save(f)

	def _stateToString(self,state:int) -> str:
		res = "----STATE "+str(state)+"--"+self.labeling[state]+"----\n"
		for j in range(len(self.matrix[state])):
			if self.matrix[state][j] > 0.0001:
				res += "s"+str(state)+" -> s"+str(j)+" : "+str(self.matrix[state][j])+'\n'
		return res

def loadMC(file_path: str) -> MC:
	"""
	Load an MC saved into a text file.

	Parameters
	----------
	file_path : str
		Location of the text file.
	
	Returns
	-------
	output : MC
		The MC saved in `file_path`.
	
	Examples
	--------
	>>> model = loadMC("my_model.txt")
	"""
	f = open(file_path,'r')
	l = f.readline()[:-1] 
	if l != "MC":
		msg = "ERROR: this file doesn't describe an MC: it describes a "+l
		raise ValueError(msg)
	labeling = literal_eval(f.readline()[:-1])
	name = f.readline()[:-1]
	initial_state = array(literal_eval(f.readline()[:-1]))
	matrix = literal_eval(f.readline()[:-1])
	matrix = array(matrix)
	f.close()
	return MC(matrix, labeling, initial_state, name)

def MC_random(nb_states: int, alphabet: list, random_initial_state: bool=False) -> MC:
	"""
	Generate a random MC.

	Parameters
	----------
	number_states : int
		Number of states.
	alphabet : list of str
		List of observations.
	random_initial_state: bool, optional
		If set to True we will start in each state with a random probability,
		otherwise we will always start in state 0.
		Default is False.
	
	Returns
	-------
	MC
		A pseudo-randomly generated MC.

	Examples
	--------
	>>> model = MC_random(2,['a','b'],False)
	>>> print(model)
	Name: MC_random_2_states
	Initial state: s0
	----STATE 0--a----
	s0 -> s0 : 0.625
	s0 -> s1 : 0.375
	----STATE 1--b----
	s1 -> s0 : 0.9
	s1 -> s1 : 0.1
	"""
	if nb_states < len(alphabet):
		print("WARNING: the size of the alphabet is higher than the",end=" ")
		print("number of states. Some labels will not be assigned to",end=" ")
		print("any states.")
	
	labeling = alphabet[:min(len(alphabet),nb_states)] + choices(alphabet,k=nb_states-len(alphabet))
	
	matrix = []
	for _ in range(nb_states):
		p = randomProbabilities(nb_states)
		matrix.append(p)

	matrix = array(matrix)

	if random_initial_state:
		init = randomProbabilities(nb_states)
	else:
		init = 0
	return MC(matrix, labeling, init,"MC_random_"+str(nb_states)+"_states")

def createMC(transitions: list, labeling: list, initial_state, name: str ="unknown_MC") -> MC:
	"""
	An user-friendly way to create a MC.

	Parameters
	----------
	transitions : [ list of tuples (int, int, float)]
		Each tuple represents a transition as follow: 
		(source state ID, destination state ID, probability).
	labeling: list of str
		A list of N observations (with N the nb of states).
		If `labeling[s] == o` then state of ID `s` is labelled by `o`.
		Each state has exactly one label.
	initial_state : int or list of float
		Determine which state is the initial one (then it's the id of the
		state), or what are the probability to start in each state (then it's
		a list of probabilities).
	name : str, optional
		Name of the model.
		Default is "unknow_MC"
	
	Returns
	-------
	MC
		the MC describes by `transitions`, `labeling`, and `initial_state`.
	
	Examples
	--------
	>>> model = createMC([(0,1,1.0),(1,0,0.6),(1,1,0.4)],['b','a'],0,"My_MC")
	>>> print(model)
	Name: My_MC
	Initial state: s0
	----STATE 0--b----
	s0 -> s1 : 1.0
	----STATE 1--a----
	s1 -> s0 : 0.6
	s1 -> s1 : 0.4
	"""
	states = list(set([i[0] for i in transitions]+[i[1] for i in transitions]))
	states.sort()
	nb_states = len(states)
	
	if nb_states > len(labeling):
		raise ValueError("ERROR: all states are not labelled (the labeling list is too small).")
	elif nb_states < len(labeling):
		print("WARNING: the labeling list is bigger than the number of states")

	res = zeros((nb_states,nb_states))
	for t in transitions:
		res[states.index(t[0])][states.index(t[1])] = t[2]
	
	return MC(res, labeling, initial_state, name)
