import stormpy as st
from numpy import zeros, array, newaxis, reshape, vstack, concatenate, hstack, newaxis
from ..mc import MC
from ..mdp import MDP
from ..ctmc import CTMC
from copy import deepcopy
from io import StringIO
from contextlib import redirect_stderr

def stormpyModeltoJajapy(h,actions_name:list = []):
	"""
	Given a tormpy.SparseCtmc, stormpy.SparseDtmc,
	or a stormpy.SparseMdp, it returns the equivalent jajapy model.
	The output object will be a jajapy.MC, jajapy.MDP or a jajapy.CTMC,
	depending on the input.

	Parameters
	----------
	h : stormpy.SparseCtmc, stormpy.SparseDtmc or stormpy.SparseMdp
		The model to convert.
	
	actions_name : list of str, optional.
		If the model is an MDP, the name of the actions in the output
		model will be the one in this list. Otherwise they will be
		`a0,a1,a2,...`.

	Returns
	-------
	jajapy.MC, jajapy.CTMC or jajapy.MDP
		The same model in jajapy format.
	"""
	if type(h) == st.SparseDtmc:
		ty = 0
	elif type(h) == st.SparseCtmc:
		ty = 1
	elif type(h) == st.SparseMdp:
		ty = 2
	else:
		raise TypeError(str(type(h))+' cannot be translated to Jajapy model.')
	
	labeling = [None for _ in range(len(h.states))]
	if ty == 2:
		actions = []
		for s in h.states:
			for a in s.actions:
				if len(actions_name) <= int(str(a)):
					actions.append('a'+str(a))
				else:
					actions.append(actions_name[int(str(a))])
	
		actions = list(set(actions))
		matrix = zeros((len(h.states),len(actions),len(h.states)))
	else:
		matrix = zeros((len(h.states),len(h.states)))


	add_init_state = None
	for si,s in enumerate(h.states):
		c = si
		if len(s.labels) == 0:
			labeling[si] = "empty"
		elif 'init' in s.labels and len(s.labels) > 1:
			temp = list(s.labels)
			temp.remove("init")
			labeling.append("init")
			labeling[si] = ','.join(list(temp))
			add_init_state = c
		else:
			labeling[si] = ','.join(list(s.labels))

		for a in s.actions:
			for t in a.transitions:
				dest = t.column
				if ty == 2:
					matrix[c][int(str(a))][dest] = t.value()
				else:
					matrix[c][dest] = t.value()
		#if ty == 1:
		#	matrix[c] *= h.exit_rates[si]
	
	if add_init_state != None:
		matrix = vstack((matrix,matrix[add_init_state]))
		if ty == 2:
			matrix = concatenate((matrix,zeros((matrix.shape[0],matrix.shape[1],1))),axis=2)
		else:
			matrix = hstack((matrix,zeros(len(matrix))[:,newaxis]))
	
	if ty == 0:
		return MC(matrix, labeling)
	elif ty == 1:
		return CTMC(matrix, labeling)
	else:
		return MDP(matrix,labeling,actions)

def jajapyModeltoStormpy(h):
	"""
	Given a jajapy.MC, a jajapy.CTMC or a jajapy.MDP,
	it returns the equivalent stormpy sparse model.
	The output object will be a stormpy.SparseCtmc, stormpy.SparseDtmc,
	or a stormpy.SparseMdp depending on the input.

	Parameters
	----------
	h : jajapy.MC, jajapy.CTMC or jajapy.MDP
		The model to convert.

	Returns
	-------
	stormpy.SparseCtmc, stormpy.SparseDtmc or stormpy.SparseMdp
		The same model in stormpy format.
	"""
	if type(h) == MDP:
		return MDPtoStormpy(h)
	elif type(h) == CTMC:
		return CTMCtoStormpy(h)
	elif type(h) == MC:
		return MCtoStormpy(h)
	else:
		raise TypeError(str(type(h))+' cannot be embedded to a stormpy sparse model.')

def _buildStateLabeling(h):
	state_labeling = st.storage.StateLabeling(h.nb_states)
	for o in h.getAlphabet():
		state_labeling.add_label(o)
	for s in range(h.nb_states):
		state_labeling.add_label_to_state(h.labeling[s],s)
	return state_labeling

def MDPtoStormpy(h):
	"""
	Given a jajapy.MDP, it returns the equivalent stormpy sparse model.
	The output object will be a stormpy.SparseMdp.

	Parameters
	----------
	h : jajapy.MDP
		The model to convert.

	Returns
	-------
	stormpy.SparseMdp
		The same model in stormpy format.
	"""
	state_labeling = _buildStateLabeling(h)
	nb_actions = len(h.getActions())
	transition_matrix = h.matrix
	transition_matrix = reshape(transition_matrix.flatten(),(h.nb_states*nb_actions,h.nb_states))
	transition_matrix =  st.build_sparse_matrix(transition_matrix,[nb_actions*i for i in range(h.nb_states)])
	choice_labeling = st.storage.ChoiceLabeling(h.nb_states*nb_actions)
	for ia,a in enumerate(h.getActions()):
		choice_labeling.add_label(a)
		choice_labeling.add_label_to_choice(a,ia)
	reward_models = {}
	action_reward = [-1.0 for _ in range(len(transition_matrix))]
	reward_models["nb_executed_actions"] = st.SparseRewardModel(optional_state_action_reward_vector = action_reward)
	components = st.SparseModelComponents(transition_matrix=transition_matrix,
										  state_labeling=state_labeling,
										  reward_models=reward_models)
	components.choice_labeling = choice_labeling
	mdp = st.storage.SparseMdp(components)
	return mdp

def MCtoStormpy(h):
	"""
	Given a jajapy.MC, it returns the equivalent stormpy sparse model.
	The output object will be a stormpy.SparseDtmc.

	Parameters
	----------
	h : jajapy.MC
		The model to convert.

	Returns
	-------
	stormpy.SparseDtmc
		The same model in stormpy format.
	"""
	state_labeling = _buildStateLabeling(h)
	transition_matrix = h.matrix
	transition_matrix =  st.build_sparse_matrix(transition_matrix)
	components = st.SparseModelComponents(transition_matrix=transition_matrix,
										  state_labeling=state_labeling)
	mc = st.storage.SparseDtmc(components)
	return mc

def CTMCtoStormpy(h):
	"""
	Given a jajapy.CTMC, it returns the equivalent stormpy sparse model.
	The output object will be a stormpy.SparseCtmc.

	Parameters
	----------
	h : jajapy.CTMC
		The model to convert.

	Returns
	-------
	stormpy.SparseCtmc
		The same model in stormpy format.
	"""
	state_labeling = _buildStateLabeling(h)
	transition_matrix = deepcopy(h.matrix)
	e = array([h.e(s) for s in range(h.nb_states)])
	transition_matrix /= e[:,newaxis]
	transition_matrix =  st.build_sparse_matrix(transition_matrix)
	components = st.SparseModelComponents(transition_matrix=transition_matrix,
										  state_labeling=state_labeling,
										  rate_transitions=True)
	components.exit_rates = e
	ctmc = st.storage.SparseCtmc(components)
	return ctmc

def loadPrism(path: str):
	"""
	Load the model described in file `path` under Prism format.
	Remark: this function uses the stormpy parser for Prism file.

	Parameters
	----------
	path : str
		Path to the Prism model to load.

	Returns
	-------
	jajapy.MC or jajapy.CTMC or jajapy.MDP
		A jajapy model equivalent to the model described in `path`.
	"""
	text_trap = StringIO()
	with redirect_stderr(text_trap):
		try:
			prism_program = st.parse_prism_program(path,False)
		except RuntimeError:
			prism_program = st.parse_prism_program(path,True)
	stormpy_model = st.build_model(prism_program)	
	jajapy_model = stormpyModeltoJajapy(stormpy_model)
	return jajapy_model