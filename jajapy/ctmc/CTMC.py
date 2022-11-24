from numpy.random import exponential
from ast import literal_eval
from ..base.tools import resolveRandom, randomProbabilities
from ..base.Set import Set
from ..mc.MC import MC
from ..base.Model import Model
from math import exp, log
from random import randint
from numpy import array, zeros, dot, ndarray, vstack, hstack, newaxis, append
from sys import platform
from multiprocessing import cpu_count, Pool
from random import choices

class CTMC(Model):
	"""
	Class representing a CTMC.
	"""
	def __init__(self, matrix: ndarray, labeling: list, name: str ="unknown_CTMC") -> None:
		"""
		Creates an CTMC.

		Parameters
		----------
		matrix : ndarray
			A (N x N) ndarray (with N the nb of states).
			Represents the transition matrix.
			`matrix[s1][s2]` is the rate associated to the transition 
			from `s1` to `s2`.
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
			Default is "unknow_CTMC"
		"""
		self.alphabet = list(set(labeling))
		self.labeling = labeling

		if not 'init' in self.labeling:
			msg = "No initial state given: at least one"
			msg += " state should be labelled by 'init'."
			raise ValueError(msg)
		initial_state = [1.0/self.labeling.count("init") if i=='init' else 0.0 for i in self.labeling]

		super().__init__(matrix,initial_state,name)
		if len(labeling) != self.nb_states:
			raise ValueError("The length of labeling is not equal to the number of states")
		for s in range(self.nb_states):
			if self.e(s) == 0.0:
				raise ValueError("State "+str(s)+" doesn't have any leaving transition.")

	def e(self,s: int) -> float:
		"""
		Returns the exit rate of state ``s``, i.e. the sum of all the rates in
		this state.

		Returns
		-------
		s : int
			A state ID.
		float
			An exit rate.
		"""
		self._checkStateIndex(s)
		return sum(self.matrix[s])
	
	def l(self, s1:int, s2:int, obs:str) -> float:
		"""
		Returns the rate associated to the transition from state `s1`, seeing
		`obs`, to state `s2`.

		Parameters
		----------
		s1 : int
			A state ID.
		s2 : int
			A state ID.
		obs : str
			An observation.

		Returns
		-------
		float
			A rate.
		"""
		self._checkStateIndex(s1)
		self._checkStateIndex(s2)
		if self.labeling[s1] != obs:
			return 0.0
		return self.matrix[s1][s2]
	
	def lkl(self, s: int, t: float) -> float:
		"""
		Returns the likelihood of staying in `s` state for a duration `t`.

		Parameters
		----------
		s : int
			A state ID.
		t : float
			A waiting time.

		Returns
		-------
		float
			A Likelihood.
		"""
		if t < 0.0:
			return 0.0
		return self.e(s)*exp(-self.e(s)*t)
	
	def tau(self, s1:int, s2: int, obs: str) -> float:
		"""
		Returns the probability to move from `s1` to `s2` generating
		observation ``obs``.

		Parameters
		----------
		s1 : int
			A state ID.
		s2 : int
			A state ID.
		obs : str
			An observation.

		Returns
		-------
		float
			A probability.
		"""
		return self.l(s1,s2, obs)/self.e(s1)
	
	def expected_time(self, s:int) -> float:
		"""
		Returns the expected waiting time in `s`, i.e. the inverse of
		the exit rate.
		
		Parameters
		----------
		s : int
			A state ID.
		
		Returns
		-------
		float
			expected waiting time in this state.
		"""
		return 1/self.e(s)

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

	def _stateToString(self,state:int) -> str:
		res = "----STATE "+str(state)+"--"+self.labeling[state]+"----\n"
		res += "Exepected waiting time: "+str(self.expected_time(state))+'\n'
		den = self.e(state)
		for j in range(len(self.matrix[state])):
			if self.matrix[state][j]/den > 0.0001:
				res += "s"+str(state)+" -> s"
				res += str(j)+" : lambda = "+str(self.matrix[state][j])+'\n'
		return res
	
	def next(self,state: int) -> tuple:
		"""
		Return a state-observation pair according to the distributions 
		described by matrix
		
		Returns
		-------
		output : (int, str)
			A state-observation pair.
		"""
		exps = []
		for exp_lambda in self.matrix[state]:
			if exp_lambda <= 0.0:
				exps.append(1024)
			else:
				exps.append(exponential(1/exp_lambda))
		next_state= exps.index(min(exps))
		next_obs = self.labeling[state]
		return (next_obs, next_state, min(exps))

	def run(self,number_steps: int, timed: bool = False) -> list:
		"""
		Simulates a run of length ``number_steps`` of the model and return the
		sequence of observations generated. If ``timed`` it returns a list of
		pairs waiting time-observation.
		
		Parameters
		----------
		number_steps: int
			length of the simulation.
		timed: bool, optional
			Wether or not it returns also the waiting times. Default is False.

		Returns
		-------
		output: list of str
			trace generated by the run.
		"""
		output = []
		current = resolveRandom(self.initial_state)
		c = 0
		while c < number_steps:
			[symbol, next_state, time_spent] = self.next(current)
			output.append(symbol)
			if timed:
				output.append(time_spent)
			current = next_state
			c += 1
		output.append(self.labeling[current])
		return output
	
	def _computeAlphas_timed(self, sequence: list, times: int) -> float:
		obs_seq   = [sequence[i] for i in range(0,len(sequence),2)]
		times_seq = [sequence[i] for i in range(1,len(sequence),2)]
		len_seq = len(obs_seq)-1
		prev_arr = array(self.initial_state)
		for k in range(len_seq):
			new_arr = zeros(self.nb_states)
			for s in range(self.nb_states):
				p = array([self.l(ss,s,obs_seq[k])*exp(-self.e(ss)*times_seq[k]) for ss in range(self.nb_states) ])
				new_arr[s] = dot(prev_arr,p)
			prev_arr = new_arr
		last_arr = prev_arr * (array(self.labeling) == obs_seq[-1])
		return log(last_arr.sum())*times


	def logLikelihood(self,traces: Set) -> float:
		if traces.type == 0: # non-timed traces
			return super().logLikelihood(traces)
		else: # timed traces
			if platform != "win32" and platform != "darwin":
				p = Pool(processes = cpu_count()-1)
				tasks = []
				for seq,times in zip(traces.sequences,traces.times):
					tasks.append(p.apply_async(self._computeAlphas_timed, [seq, times,]))
				temp = [res.get() for res in tasks if res.get() != False]
			else:
				temp = [self._computeAlphas_timed(seq,times) for seq,times in zip(traces.sequences,traces.times)]
			return sum(temp)/sum(traces.times)

	def toMC(self, name: str="unknown_MC") -> MC:
		"""
		Returns the equivalent untimed MC.

		Parameters
		----------
		name : str, optional
			Name of the output model. By default "unknown_MC"

		Returns
		-------
		MC
			An equivalent untimed model.
		"""
		new_matrix = self.matrix
		for i in range(self.nb_states):
			new_matrix[i] /= self.e(i)

		return MC(new_matrix,self.labeling,name)

	def save(self,file_path:str):
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
		f.write("CTMC\n")
		f.write(str(self.labeling))
		f.write('\n')
		super()._save(f)

def loadCTMC(file_path: str) -> MC:
	"""
	Load an CTMC saved into a text file.

	Parameters
	----------
	file_path : str
		Location of the text file.
	
	Returns
	-------
	output : CTMC
		The CTMC saved in `file_path`.
	"""
	f = open(file_path,'r')
	l = f.readline()[:-1] 
	if l != "CTMC":
		print("ERROR: this file doesn't describe an CTMC: it describes a "+l)
	labeling = literal_eval(f.readline()[:-1])
	name = f.readline()[:-1]
	initial_state = array(literal_eval(f.readline()[:-1]))
	matrix = literal_eval(f.readline()[:-1])
	matrix = array(matrix)
	f.close()
	return CTMC(matrix, labeling, name)

def CTMC_random(nb_states: int, alphabet: list, min_exit_rate_time : int,
				max_exit_rate_time: int, self_loop: bool = True,
				random_initial_state: bool=True) -> CTMC:
	"""
	Generates a random CTMC. All the rates will be between 0 and 1.
	All the exit rates will be integers.

	Parameters
	----------
	nb_states : int
		Number of states.
	alphabet : list of str
		List of observations.
	min_exit_rate_time: int
		Minimum exit rate for the states (included).
	max_exit_rate_time: int
		Maximum exit rate for the states (included).
	self_loop: bool, optional
		Wether or not there will be self loop in the output model.
		Default is True.
	random_initial_state: bool, optional
		If set to True we will start in each state with a random probability, otherwise we will always start in state 0.
		Default is True.
	
	Returns
	-------
	CTMC
		A pseudo-randomly generated CTMC.

	Examples
	--------
	>>> model = CTMC_random(2,['a','b'],1,5)
	>>> print(model)
	Name: CTMC_random_2_states
	Initial state: s2
	----STATE 0--a----
	Exepected waiting time: 2.0
	s0 -> s0 : lambda = 0.38461538461538464
	s0 -> s1 : lambda = 0.11538461538461539
	----STATE 1--b----
	Exepected waiting time: 4.0
	s1 -> s0 : lambda = 0.13636363636363635
	s1 -> s1 : lambda = 0.11363636363636363
	----STATE 2--init----
	Exepected waiting time: 1.0
	s2 -> s0 : lambda = 0.2
	s2 -> s1 : lambda = 0.8
	"""
	if nb_states < len(alphabet):
		print("WARNING: the size of the alphabet is higher than the",end=" ")
		print("number of states. Some labels will not be assigned to",end=" ")
		print("any states.")
	
	if 'init' in alphabet:
		msg =  "The label 'init' cannot be used: it is reserved for initial states."
		raise SyntaxError(msg)

	
	labeling = alphabet[:min(len(alphabet),nb_states)] + choices(alphabet,k=nb_states-len(alphabet))
	
	matrix = []
	for i in range(nb_states):
		if self_loop:
			random_probs = array(randomProbabilities(nb_states))
		else:
			p = randomProbabilities(nb_states-1)
			random_probs.insert(i,0.0)
		p = randomProbabilities(nb_states)
		av_waiting_time = randint(min_exit_rate_time,max_exit_rate_time)
		p = random_probs/av_waiting_time
		matrix.append(p)
	
	labeling.append("init")
	if random_initial_state:
		matrix.append(append(randomProbabilities(nb_states),0.0))
	else:
		matrix.append([1.0]+[0.0 for i in range(nb_states)])
	
	matrix = array(matrix)
	return CTMC(matrix, labeling,"CTMC_random_"+str(nb_states)+"_states")

def createCTMC(transitions: list, labeling: list, initial_state, name: str ="unknown_CTMC") -> CTMC:
	"""
	An user-friendly way to create a CTMC.

	Parameters
	----------
	transitions : [ list of tuples (int, int, float)]
		Each tuple represents a transition as follow: 
		(source state ID, destination state ID, rate).
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
		Default is "unknow_CTMC"
	
	Returns
	-------
	CTMC
		the CTMC describes by `transitions`, `labeling`, and `initial_state`.
	
	Examples
	--------
	>>> model = createCTMC([(0,1,1.0),(1,0,0.3),(1,1,0.2)],['b','a'],0,"My_MC")
	>>> print(model)
	Name: My_MC
	Initial state: s0
	----STATE 0--b----
	Exepected waiting time: 1.0
	s0 -> s1 : lambda = 1.0
	----STATE 1--a----
	Exepected waiting time: 2.0
	s1 -> s0 : lambda = 0.3
	s1 -> s1 : lambda = 0.2
	"""
	if 'init' in labeling:
		msg =  "The label 'init' cannot be used: it is reserved for initial states."
		raise SyntaxError(msg)
	
	states = list(set([i[0] for i in transitions]+[i[1] for i in transitions]))
	states.sort()
	nb_states = len(states)
	
	if nb_states > len(labeling):
		raise ValueError("All states are not labelled (the labeling list is too small).")
	elif nb_states < len(labeling):
		print("WARNING: the labeling list is bigger than the number of states")

	res = zeros((nb_states,nb_states))
	for t in transitions:
		res[states.index(t[0])][states.index(t[1])] = t[2]
	
	labeling.append('init')
	res = vstack((res,zeros(len(res))))
	res = hstack((res,zeros(len(res))[:,newaxis]))
	if type(initial_state) == int:
		res[-1][initial_state] = 1.0
	else:
		if type(initial_state) == ndarray:
			initial_state = initial_state.tolist()
		res[-1] = array(initial_state+[0.0])

	return CTMC(res, labeling, name)
