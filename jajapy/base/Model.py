from math import  log
from .tools import resolveRandom
from .Set import Set
from numpy import array, where, append, dot, zeros, vsplit
from numpy.random import geometric
from multiprocessing import cpu_count, Pool
from sys import platform
from math import log

MC_ID   = 0
MDP_ID  = 1
HMM_ID  = 2
GOHMM_ID= 3
CTMC_ID = 4
PCTMC_ID= 5

class Model:
	"""
	Abstract class that represents a model.
	Is inherited by all the classes representing any model.
	Should not be instantiated itself!
	"""
	def __init__(self,matrix,initial_state,name: str) -> None:
		"""
		Creates an abstract model.

		Parameters
		----------
		matrix: ndarray or dict
			The transition matrix.

		initial_state: int or list of float
			determine which state is the initial one (then it's the id of the
			state), or what are the probability to start in each state
			(then it's a list of probabilities).

		name: str
			name of the model.
		"""
		self.nb_states = len(matrix)
		# initial_state can be a list of probability or an int
		if type(initial_state) == int:
			self.initial_state = array([0.0 for i in range(self.nb_states)])
			self.initial_state[initial_state] = 1.0
		else:
			if round(sum(initial_state)) != 1.0:
				msg = "Error: the sum of initial_state should be 1.0, here it's"
				msg+= str(sum(initial_state))
				raise ValueError(msg)
			self.initial_state = array(initial_state)
		self.matrix = matrix
		self.name = name
	
	def rename(self,name: str) -> None:
		"""
		Change the name of the model.

		Parameters
		----------
		name : str
			new name.
		"""
		self.name = name

	def next(self,state: int):
		"""
		overrided
		"""
		pass

	def _stateToString(self,state:int):
		"""
		overrided
		"""
		pass


	def _save(self,f) -> None:
		f.write(self.name)
		f.write('\n')
		f.write(str(self.initial_state.tolist()))
		f.write('\n')
		f.write(str(self.matrix.tolist()))
		f.write('\n')
		f.close()

	def tau(self,s1: int,s2: int,obs) -> float:
		"""
		overrided
		"""
		pass

	def pi(self,s: int) -> float:
		"""
		Return the probability of starting in state ``s``.

		Parameters
		----------
		s: int
			state ID.

		Returns
		-------
		output : float
			the probability of starting in state s.
		"""
		return self.initial_state[s]

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
		
		return output
	
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

	def logLikelihood(self,sequences: Set) -> float:
		"""
		Compute the average loglikelihood of a set.

		Parameters
		----------
		sequences: Set
			A set.
		
		Returns
		-------
		output: float
			loglikelihood of ``sequences`` under this model.
		
		Examples
		--------
		>>> model.logLikelihood(set1)
		-4.442498878506513
		"""
		if platform != "win32" and platform != "darwin":
			return self._logLikelihood_multiproc(sequences)
		else:
			return self._logLikelihood_oneproc(sequences)

	def _updateAlphaMatrix(self,sequence: list,
						   common: int,
						   alpha_matrix: list) -> array:
		"""
		Update the given alpha values for all the states for a new ``sequence``
		of observations. It keeps the alpha values for the ``common`` first
		observations of the ``sequence``. The idea is the following: if you
		have already computed the alpha values for a previous sequence and you
		want to compute the alpha values of a new sequence that starts with the
		same ``common`` observations you don't need to compute again the first
		``common`` alpha values for each states. If, on the other hand, you
		have still not computed any alpha values you can simply set ``common``
		to 0 and give an empty ``alpha_matrix`` which has the right size.
		The method ``initAlphaMatrix`` can generate such matrix.

		Parameters
		----------
		sequence: list of str
			the sequence of observations for which we compute the alpha values.
		common: int
			for each state, the first ``common`` alpha values will be keept
			unchanged.
		alpha_matrix: 2-D narray of float
			the ``alpha_matrix`` to update. Can be generated by the method
			``initAlphaMatrix``.

		Returns
		-------
		output: 2-D narray of float
			the alpha matrix containing all the alpha values for all the states
			for this sequence: ``alpha_matrix[s][t]`` is the probability of
			being in state ``s`` after seing the ``t-1`` first observation of
			``sequence``.
		"""
		diff_size = len(alpha_matrix)-1 - len(sequence)
		if diff_size < 0: # alpha_matrix too small
			n = zeros(-diff_size * self.nb_states).reshape(-diff_size,self.nb_states)
			alpha_matrix = append(alpha_matrix,n,axis=0)
		elif diff_size > 0: #alpha_matrix too big
			alpha_matrix = vsplit(alpha_matrix,[len(alpha_matrix)-diff_size,self.nb_states])[0]
		for k in range(common,len(sequence)):
			for s in range(self.nb_states):
				p = array([self.tau(ss,s,sequence[k]) for ss in range(len(self.matrix))])
				alpha_matrix[k+1,s] = dot(alpha_matrix[k],p)
		return alpha_matrix

	def _initAlphaMatrix(self,len_seq: int) -> array:
		"""
		Return a matrix with the correct size to be updated by the
		``updateAlphaMatrix`` method.

		Parameters
		----------
		len_seq: int
			length of the sequence for which we will compute the alpha values.

		Returns
		-------
		output: a matrix with the correct size to be updated by the updateAlphaMatrix method
		:rtype: 2-D narray of float
		"""
		zero_arr = zeros(shape=(len_seq*self.nb_states,))
		alpha_matrix = append(self.initial_state,zero_arr).reshape(len_seq+1,self.nb_states)
		return alpha_matrix

	def _logLikelihood_oneproc(self,sequences: Set) -> float:
		"""
		Compute the average loglikelihood of a set of sequences if
		multiprocessing is disable.

		Parameters
		----------
		sequences: Set
			A set.
		
		Returns
		-------
		output: float
			loglikelihood of ``sequences`` under this model.
		"""
		sequences_sorted = sequences.sequences[:]
		sequences_sorted.sort()
		loglikelihood = 0.0
		alpha_matrix = self._initAlphaMatrix(len(sequences_sorted[0]))
		for seq in range(len(sequences_sorted)):
			sequence = sequences_sorted[seq]
			times = sequences.times[sequences.sequences.index(sequence)]
			common = 0
			if seq > 0:
				while common < min(len(sequences_sorted[seq-1]),len(sequence)):
					if sequences_sorted[seq-1][common] != sequence[common]:
						break
					common += 1
					
			alpha_matrix = self._updateAlphaMatrix(sequence,common,alpha_matrix)
			if alpha_matrix[-1].sum() > 0:
				loglikelihood += log(alpha_matrix[-1].sum()) * times

		return loglikelihood / sum(sequences.times)

	def _logLikelihood_multiproc(self, sequences: Set) -> float:
		p = Pool(processes = cpu_count()-1)
		tasks = []
		for seq,times in zip(sequences.sequences,sequences.times):
			tasks.append(p.apply_async(self._computeAlphas, [seq, times,]))
		temp = [res.get() for res in tasks if res.get() != False]
		return sum(temp)/sum(sequences.times)

	def _computeAlphas(self,sequence: list, times: int) -> float:
		"""
		Compute the alpha values for ``sequence``.

		Parameters
		----------
		sequence: list of str
			Sequence of observations.
		times: int
			Number of times this sequence appears in the sample.

		Returns
		-------
		float
			loglikelihood of ``sequence`` multiplied by ``times``.
		"""
		len_seq = len(sequence)
		prev_arr = self.initial_state
		for k in range(len_seq):
			new_arr = zeros(self.nb_states)
			for s in range(self.nb_states):
				p = array([self.tau(ss,s,sequence[k]) for ss in range(self.nb_states)])
				new_arr[s] = dot(prev_arr,p)
			prev_arr = new_arr
		if prev_arr.sum() == 0.0:
			return 0.0
		return log(prev_arr.sum())*times
	
	def _checkStateIndex(self,s:int) -> None:
		if type(s) != int:
			raise TypeError('The parameter must be a valid state ID')
		elif s < 0:
			raise IndexError('The parameter must be a valid state ID')
		elif s >= self.nb_states:
			raise IndexError('This model contains only '+str(self.nb_states)+' states')