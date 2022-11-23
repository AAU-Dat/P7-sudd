from .MC import MC, MC_random
from ..base.BW import *
from ..base.Set import Set
from numpy import log, newaxis, inf

class BW_MC(BW):
	"""
	class for general Baum-Welch algorithm on MC.
	"""
	def __init__(self):
		super().__init__()
	
	def fit(self, traces, initial_model=None, nb_states: int=None,
			random_initial_state: bool=False, output_file: str=None,
			epsilon: float=0.01, max_it: int = inf, pp: str='',
			verbose: bool = True, return_data: bool= False,
			stormpy_output: bool = True):
		"""
		Fits the model according to ``traces``.

		Parameters
		----------
		traces : Set or list or numpy.ndarray
			training set.
		initial_model : MC or stormpy.SparseCtmc, optional.
			first hypothesis. If not set it will create a random MC with
			``nb_states`` states. Should be set if ``nb_states`` is not set.
		nb_states: int
			If ``initial_model`` is not set it will create a random MC with
			``nb_states`` states. Should be set if ``initial_model`` is not set.
			Default is None.
		random_initial_state: bool
			If ``initial_model`` is not set it will create a random MC with
			random initial state according to this sequence of probabilities.
			Should be set if ``initial_model`` is not set.
			Default is False.
		output_file : str, optional
			if set path file of the output model. Otherwise the output model
			will not be saved into a text file.
		epsilon : float, optional
			the learning process stops when the difference between the
			loglikelihood of the training set under the two last hypothesis is
			lower than ``epsilon``. The lower this value the better the output,
			but the longer the running time. By default 0.01.
		max_it: int
			Maximal number of iterations. The algorithm will stop after `max_it`
			iterations.
			Default is infinity.
		pp : str, optional
			Will be printed at each iteration. By default ''
		verbose: bool, optional
			Print or not a small recap at the end of the learning.
			Default is True.
		return_data: bool, optional
			If set to True, a dictionary containing following values will be
			returned alongside the hypothesis once the learning is done.
			'learning_rounds', 'learning_time', 'training_set_loglikelihood'.
			Default is False.
		stormpy_output: bool, optional
			If set to True the output model will be a Stormpy sparse model.
			Default is True.

		Returns
		-------
		MC or stormpy.SparseDtmc
			The fitted MC.
			If `stormpy_output` is set to `False` or if stormpy is not available on
			the machine it returns a `jajapy.MC`, otherwise it returns a `stormpy.SparseDtmc`
		"""
		if type(traces) != Set:
			traces = Set(traces, t=0)
				
		alphabet = traces.getAlphabet()
		if not 'init' in alphabet:
			for s in range(len(traces.sequences)):
				traces.sequences[s].insert(0,'init')
		else:
			alphabet.remove("init")
		
		if not initial_model:
			if not nb_states:
				raise ValueError("Either nb_states or initial_model should be set")
			initial_model = MC_random(nb_states,alphabet,random_initial_state)

		return super().fit(traces, initial_model, output_file, epsilon, max_it, pp, verbose,return_data,stormpy_output)

	def _processWork(self,sequence,times):
		alpha_matrix = self.computeAlphas(sequence)
		beta_matrix  = self.computeBetas( sequence)
		proba_seq = alpha_matrix.T[-1].sum()
		if proba_seq == 0.0:
			return False
		####################
		den = (alpha_matrix.T[:-1]*beta_matrix.T[:-1]*times/proba_seq).sum(axis=0)			
		num = zeros(shape=(self.nb_states,self.nb_states))
		for s in range(self.nb_states):
			for ss in range(self.nb_states):
				p = array([self.h_tau(s,ss,o) for o in sequence])
				num[s,ss] = dot(alpha_matrix[s][:-1]*p*beta_matrix[ss][1:],times/proba_seq).sum()
		####################
		return [den,num, proba_seq,times]

	def _generateHhat(self,temp):
		den = array([i[0] for i in temp]).sum(axis=0)
		num = array([i[1] for i in temp]).sum(axis=0)
		lst_proba=array([i[2] for i in temp])
		lst_times=array([i[3] for i in temp])

		currentloglikelihood = dot(log(lst_proba),lst_times)

		for s in range(len(den)):
			if den[s] == 0.0:
				den[s] = 1.0
				num[s] = self.h.matrix[s]

		matrix = num/den[:, newaxis]
		return [MC(matrix,self.h.labeling),currentloglikelihood]
		