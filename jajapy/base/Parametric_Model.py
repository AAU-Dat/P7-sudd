from numpy import ndarray, array, isnan, where, zeros, nan
from numpy.random import geometric
from ast import literal_eval
from copy import deepcopy
from ..base.Set import Set

class Parametric_Model:
	"""
	Abstract class that represents parametric MC/CTMC/MDP.
	Is inherited by PMC, PCTMC and PMDP.
	Should not be instantiated itself!
	"""
	def __init__(self, matrix: ndarray, labeling: list,
				 transition_str: list, parameter_values: list,
				 parameter_indexes: list, parameter_str: list,
				 name: str) -> None:
		"""
		Creates an abstract model for PMC or PCTMC.

		Parameters
		----------
		matrix : ndarray
			Represents the transition matrix.
			matrix[i,j] is the index, in `transition_str`, of the symbolic
			value of the transition from `i` to `j`.
		labeling: list of str
			A list of N observations (with N the nb of states).
			If `labeling[s] == o` then state of ID `s` is labelled by `o`.
			Each state has exactly one label.
		transition_str: list of str
			Contains the symbolic value for each transition.
		parameter_values: list of float
			Contains the value for each parameter.
			`parameter_values[i]` is the instantiation for parameter `i`.
			If the ith parameter is not instantiated, `parameter_values[i] == nan`.
		parameter_indexes: list of ndarray
			Contains the indexes of each transition using each parameter.
			`parameter_indexes[i] = array([[0,1],[2,1]])` means that parameter `i`
			is used by the transition from state 0 to state 1 and from state 2 to state 1.
		parameter_str: list
			Contains the name of each parameter.
			`parameter_str[i]` is the name of parameter `i`.
			Parameter `i` doesn't have a name if `parameter_str[i]==None`.
		name : str, optional
			Name of the model.
		"""
		self.labeling = labeling
		self.alphabet = list(set(labeling))
		self.nb_states = len(matrix)
		
		if not 'init' in self.labeling:
			msg = "No initial state given: at least one"
			msg += " state should be labelled by 'init'."
			raise ValueError(msg)
		initial_state = [1.0/self.labeling.count("init") if i=='init' else 0.0 for i in self.labeling]

		if len(parameter_indexes) != len(parameter_values) or len(parameter_values) != len(parameter_str):
			raise ValueError("Length of parameter_indexes, parameter_values and parameter_str must be equal.")
		self.nb_parameters = len(parameter_indexes)
		if max(matrix.flatten()) >= len(transition_str):
			msg = "Transition "+str(max(matrix.flatten()))+" found in matrix while length "
			msg+= "of transition_str is "+str(len(transition_str))
			raise ValueError(msg)
		if min(matrix.flatten()) < 0:
			raise ValueError("Transition "+str(min(matrix.flatten()))+" found in matrix")

		#for p in parameter_indexes:
		#	for i in p:
		#		if min(i) < 0:
		#			raise ValueError("State "+str(min(p.flatten()))+" found in parameter_indexes")
		#		if max(i) >= self.nb_states:
		#			msg = "State "+str(max(i))+" found in parameter_indexes while there "
		#			msg+= "are only "+str(self.nb_states)+" states."
		#			raise ValueError(msg)

		if type(initial_state) == int:
			self.initial_state = array([0.0 for i in range(self.nb_states)])
			self.initial_state[initial_state] = 1.0
		else:
			if round(sum(initial_state)) != 1.0:
				msg = "Error: the sum of initial_state should be 1.0, here it's"
				msg+= str(sum(initial_state))
				raise ValueError(msg)
			self.initial_state = array(initial_state)
		
		if len(self.labeling) != self.nb_states:
			msg = "The length of labeling ("+str(len(labeling))+") is not equal "
			msg+= "to the number of states("+str(self.nb_states)+")"
			raise ValueError(msg)
		
		self.name = name
		self.transition_str = transition_str
		self.parameter_values = parameter_values
		self.parameter_indexes= parameter_indexes
		self.parameter_str = parameter_str
		self.matrix = matrix
		self.transition_values = zeros(len(transition_str))
		self._evaluateAll()
		

	def isInstantiated(self,state:int = None) -> bool:
		"""
		Checks if all the parameters are instantiated.
		If `state` is set, checks if all the transitions leaving this state
		are instantiated.

		Returns
		-------
		bool
			True if all the parameters are intantiated.
		"""
		if type(state) == type(None):
			return not (isnan(self.parameter_values)==True).any()
		else:
			self._checkStateIndex(state)
			return not (isnan(array([self.parameter_values[i] for i in self.involvedParameters(state)]))==True).any()

	def transitionValue(self,i:int,j:int):
		return self.transition_values[self.matrix[i,j]]

	def transitionStr(self,i:int,j:int):
		val = self.transition_str[self.matrix[i,j]]
		while '$' in val:
			s = val.index('$')
			e = val.index('$',s+1)
			val = val[:s]+self.parameter_str[int(val[s+1:e])]+val[e+1:]
		return val
	
	def parameterValue(self, p:str)-> float:
		if not p in self.parameter_str:
			return nan
		return self.parameter_values[self.parameter_str.index(p)]

	def instantiate(self,parameters: list, values: list) -> ndarray: 
		"""
		Set all the parameters in `parameters` to the values `values`.

		Parameters
		----------
		parameters : list of string
			List of all parameters to set. This list must contain parameter
			names.
		values : list of float
			List of values. `parameters[i]` will be set to `values[i]`.
		"""
		new_values = deepcopy(self.parameter_values)
		for s,v in zip(parameters,values):
			if s in self.parameter_str:
				new_values[self.parameter_str.index(s)] = v
			else:
				raise ValueError("parameter "+s+" doesn't exist in this model.")
		return new_values

	def evaluateExpression(self,string:str,parameter_values=None) -> float:
		if parameter_values == None:
			parameter_values = self.parameter_values
		while '$' in string:
			start = string.index('$')
			end = string.index('$',start+1)
			string = string[:start]+ str(parameter_values[int(string[start+1:end])])+string[end+1:]
		return eval(string)

	def evaluateTransition(self,i:int,j:int,parameter_values=None) -> float:
		return self.evaluateExpression(self.transition_str[self.matrix[i,j]],parameter_values)
	
	def _evaluateAll(self,params_id=None) -> None:
		done = [0]
		for i in range(self.nb_states):
			for j in range(self.nb_states):
				to_do = self.matrix[i,j]
				if not to_do in done:
					if params_id == None:
						self.transition_values[to_do] = self.evaluateTransition(i,j)
						done.append(to_do)
					elif params_id & set(self.involvedParameters(i,j)):
						self.transition_values[to_do] = self.evaluateTransition(i,j)
						done.append(to_do)


	def involvedParameters(self,i: int,j: int = -1) -> list:
		"""
		Returns the parameters involved in the transition from i to j.
		If `j` is not set, it returns the parameters involved in all the 
		transitions leaving `i`.

		Parameters
		----------
		i : int
			source state.
		j : int, optional
			destination state

		Returns
		-------
		list of int
			list of parameter IDs.
		"""
		if j == -1:
			j = range(self.nb_states)
		else:
			j =  [j]
		res = []
		for jj in j:
			string = self.transition_str[self.matrix[i,jj]]
			while '$' in string:
				start = string.index('$')
				end = string.index('$',start+1)
				res.append(int(string[start+1:end]))
				string = string[end+1:]
		return list((set(res)))

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
	
	def __str__(self) -> str:
		res = "Name: "+self.name+'\n'
		if 1.0 in self.initial_state:
			res += "Initial state: s"+str(where(self.initial_state==1.0)[0][0])+'\n'
		else:
			res += "Initial state: "
			for i in range(len(self.matrix)):
				if self.initial_state[i] >= 0.001:
					res += 's'+str(i)+': '+str(round(self.initial_state[i],3))+', '
			res = res[:-2]+'\n'
		for i in range(self.nb_states):
			res += self._stateToString(i)+'\n'
		res += '\n'
		return res

	def generateSet(self, set_size: int, param, distribution=None, min_size=None, timed: bool=False) -> Set:
		"""
		Generates a set (training set / test set) containing ``set_size`` traces.

		Parameters
		----------
		set_size: int
			number of traces in the output set.
		param: a list, an int or a float.
			the parameter(s) for the distribution. See "distribution".
		distribution: str, optional
			If ``distribution=='geo'`` then the sequence length will be
			distributed by a geometric law such that the expected length is
			``min_size+(1/param)``.
			If distribution==None param can be an int, in this case all the
			seq will have the same length (``param``), or ``param`` can be a
			list of int.
			Default is None.
		min_size: int, optional
			see "distribution". Default is None.
		timed: bool, optional
			Only for timed model. Generate timed or non-timed traces.
			Default is False.
		
		Returns
		-------
		output: Set
			a set (training set / test set).
		
		Examples
		--------
		>>> set1 = model.generateSet(100,10)
		>>> # set1 contains 100 traces of length 10
		>>> set2 = model.generate(100, 1/4, "geo", min_size=6)
		>>> # set2 contains 100 traces. The length of the traces is distributed following
		>>> # a geometric distribution with parameter 1/4. All the traces contains at
		>>> # least 6 observations, hence the average length of a trace is 6+(1/4)**(-1) = 10.
		"""
		seq = []
		val = []
		for i in range(set_size):
			if distribution == 'geo':
				curr_size = min_size + int(geometric(param))
			else:
				if type(param) == list:
					curr_size = param[i]
				elif type(param) == int:
					curr_size = param

			if timed:
				trace = self.run(curr_size,timed)
			else:
				trace = self.run(curr_size)

			if not trace in seq:
				seq.append(trace)
				val.append(0)

			val[seq.index(trace)] += 1

		return Set(seq,val)

	def _save(self, f) -> None:
		f.write(self.name)
		f.write('\n')
		f.write(str(self.matrix))
		f.write('\n')
		f.write(str(self.labeling))
		f.write('\n')
		f.write(str(self.parameter_values))
		f.write('\n')
		f.write(str(self.parameter_indexes))
		f.write('\n')
		f.write(str(self.parameter_str))
		f.write('\n')
		f.write(str(self.transition_str))
		f.write('\n')
		f.close()

	def toStormpy(self):
		"""
		Returns the equivalent stormpy sparse model.
		The output object will be a stormpy.SparseDtmc.

		Returns
		-------
		stormpy.SparseDtmc
			The same model in stormpy format.
		"""
		#TODO

	def _checkStateIndex(self,s:int) -> None:
		if type(s) != int:
			raise TypeError('The parameter must be a valid state ID')
		elif s < 0:
			raise IndexError('The parameter must be a valid state ID')
		elif s >= self.nb_states:
			raise IndexError('This model contains only '+str(self.nb_states)+' states')

def loadParametricModel(f):
	name = literal_eval(f.readline()[:-1])
	matrix = literal_eval(f.readline()[:-1])
	labeling = literal_eval(f.readline()[:-1])
	parameter_values = literal_eval(f.readline()[:-1])
	parameter_indexes = literal_eval(f.readline()[:-1])
	parameter_str = literal_eval(f.readline()[:-1])
	transition_str = literal_eval(f.readline()[:-1])
	return matrix,labeling,parameter_values,parameter_indexes,parameter_str,transition_str,name