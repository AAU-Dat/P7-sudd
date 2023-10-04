import numpy.random
from ast import literal_eval
from ..base.tools import resolveRandom, randomProbabilities
from ..base.Set import Set
from ..mc.MC import MC
from ..pctmc.PCTMC import PCTMC
from ..base.Base_MC import *
from math import exp, log
from random import randint
from numpy import array, zeros, dot, vstack, hstack, newaxis, delete, insert
from sys import platform
from multiprocessing import cpu_count, Pool
from sympy import sympify


class CTMC(Base_MC):
    """
    Class representing a CTMC.
    """

    def __init__(
        self,
        matrix: ndarray,
        labelling: list,
        name: str = "unknown_CTMC",
        synchronous_transitions: list = [],
    ) -> None:
        """
        Creates an CTMC.

        Parameters
        ----------
        matrix : ndarray
                A (N x N) ndarray (with N the nb of states).
                Represents the transition matrix.
                `matrix[s1][s2]` is the rate associated to the transition
                from `s1` to `s2`.
        labelling: list of str
                A list of N observations (with N the nb of states).
                If `labelling[s] == o` then state of ID `s` is labelled by `o`.
                Each state has exactly one label.
        name : str, optional
                Name of the model.
                Default is "unknow_CTMC"
        synchronous_transitions: list, optional.
                This is useful only for synchronously composing this CTMC with
                another one.
                List of (source_state <int>, action <str>, dest_state <int>, rate <float>).
                Default is an empty list.
        """
        self.model_type = CTMC_ID
        self.synchronous_transitions = synchronous_transitions

        super().__init__(matrix, labelling, name)
        for s in range(self.nb_states):
            synchronous_transitions_source = [i[0] for i in synchronous_transitions]
            if self.e(s) == 0.0 and s not in synchronous_transitions_source:
                print(
                    "WARNING: State " + str(s) + " doesn't have any leaving transition."
                )

    def e(self, s: int) -> float:
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

    def l(self, s1: int, s2: int, obs: str) -> float:
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
        if self.labelling[s1] != obs:
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
        return self.e(s) * exp(-self.e(s) * t)

    def tau(self, s1: int, s2: int, obs: str) -> float:
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
        return self.l(s1, s2, obs) / self.e(s1)

    def expected_time(self, s: int) -> float:
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
        return 1 / self.e(s)

    def _stateToString(self, state: int) -> str:
        res = "----STATE " + str(state) + "--" + self.labelling[state] + "----\n"
        res += "Exepected waiting time: " + str(self.expected_time(state)) + "\n"
        den = self.e(state)
        for j in range(len(self.matrix[state])):
            if self.matrix[state][j] / den > 0.0001:
                res += "s" + str(state) + " -> s"
                res += str(j) + " : lambda = " + str(self.matrix[state][j]) + "\n"
        return res

    def next(self, state: int) -> tuple:
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
                exps.append(numpy.random.exponential(1 / exp_lambda))
        next_state = exps.index(min(exps))
        next_obs = self.labelling[state]
        return (next_obs, next_state, min(exps))

    def run(self, number_steps: int, timed: bool = False) -> list:
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
        output.append(self.labelling[current])
        return output

    def _computeAlphas_timed(self, sequence: list, times: int) -> float:
        obs_seq = [sequence[i] for i in range(0, len(sequence), 2)]
        times_seq = [sequence[i] for i in range(1, len(sequence), 2)]
        len_seq = len(obs_seq) - 1
        prev_arr = array(self.initial_state)
        for k in range(len_seq):
            new_arr = zeros(self.nb_states)
            for s in range(self.nb_states):
                p = array(
                    [
                        self.l(ss, s, obs_seq[k]) * exp(-self.e(ss) * times_seq[k])
                        for ss in range(self.nb_states)
                    ]
                )
                new_arr[s] = dot(prev_arr, p)
            prev_arr = new_arr
        last_arr = prev_arr * (array(self.labelling) == obs_seq[-1])
        try:
            res = log(last_arr.sum()) * times
        except ValueError:
            return False
        return res

    def logLikelihood(self, traces: Set) -> float:
        """
        Computes the loglikelihood of `traces` under this model.

        Parameters
        ----------
        traces : Set
                a set of traces.

        Returns
        -------
        float
                the loglikelihood of `traces` under this model.
        """
        if traces.type == 0:  # non-timed traces
            return super().logLikelihood(traces)
        else:  # timed traces
            if platform != "win32" and platform != "darwin":
                p = Pool(processes=cpu_count() - 1)
                tasks = []
                for seq, times in zip(traces.sequences, traces.times):
                    tasks.append(
                        p.apply_async(
                            self._computeAlphas_timed,
                            [
                                seq,
                                times,
                            ],
                        )
                    )
                temp = [res.get() for res in tasks if res.get() != False]
            else:
                temp = [
                    self._computeAlphas_timed(seq, times)
                    for seq, times in zip(traces.sequences, traces.times)
                ]
                while False in temp:
                    temp.remove(False)
            if len(temp) == 0:
                return -9999
            return sum(temp) / sum(traces.times)

    def toMC(self, name: str = "unknown_MC") -> MC:
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

        return MC(new_matrix, self.labelling, name)

    def save(self, file_path: str) -> None:
        """Save the model into a text file.

        Parameters
        ----------
        file_path : str
                path of the output file.

        Examples
        --------
        >>> model.save("my_model.txt")
        """
        f = open(file_path, "w")
        f.write("CTMC\n")
        super()._save(f)

    def savePrism(self, file_path: str) -> None:
        """
        Save this model into `file_path` in the Prism format.

        Parameters
        ----------
        file_path : str
                Path of the output file.
        """
        f = open(file_path, "w")
        f.write("ctmc\n\n")
        super()._savePrism(f)

    def _savePrismMatrix(self, f) -> None:
        for s1 in range(self.nb_states):
            if (self.matrix[s1] != 0).any():
                res = "\t[] s=" + str(s1) + " ->"
                for s2 in where(self.matrix[s1] != 0.0)[0]:
                    res += " " + str(self.matrix[s1, s2]) + ":(s'=" + str(s2) + ") +"
                res = res[:-2] + ";\n"
                f.write(res)


def loadCTMC(file_path: str) -> CTMC:
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
    f = open(file_path, "r")
    l = f.readline()[:-1]
    if l != "CTMC":
        print("ERROR: this file doesn't describe an CTMC: it describes a " + l)
    labelling = literal_eval(f.readline()[:-1])
    name = f.readline()[:-1]
    initial_state = array(literal_eval(f.readline()[:-1]))
    matrix = literal_eval(f.readline()[:-1])
    matrix = array(matrix)
    f.close()
    return CTMC(matrix, labelling, name)


def CTMC_random(
    nb_states: int,
    labelling: list,
    min_exit_rate_time: int,
    max_exit_rate_time: int,
    self_loop: bool = True,
    random_initial_state: bool = True,
    sseed: int = None,
) -> CTMC:
    """
    Generates a random CTMC. All the rates will be between 0 and 1.
    All the exit rates will be integers.

    Parameters
    ----------
    nb_states : int
            Number of states.
    labelling : list of str
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
    sseed : int, optional
            the seed value.

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
    if sseed != None:
        seed(sseed)
        numpy.random.seed(sseed)

    matrix = zeros((nb_states + 1, nb_states))
    for i in range(nb_states):
        if self_loop:
            matrix[i] = array(randomProbabilities(nb_states))
        else:
            p = randomProbabilities(nb_states - 1).tolist()
            p = insert(p, i, 0.0)
            matrix[i] = array(p)
        av_waiting_time = randint(min_exit_rate_time, max_exit_rate_time)
        matrix[i] /= av_waiting_time

    if random_initial_state:
        matrix[-1] = randomProbabilities(nb_states)
    else:
        matrix[-1] = array([1.0] + [0.0 for i in range(nb_states - 1)])

    matrix = hstack((matrix, zeros(len(matrix))[:, newaxis]))

    labelling = labelsForRandomModel(nb_states, labelling)

    seed()
    numpy.random.seed()
    return CTMC(matrix, labelling, "CTMC_random_" + str(nb_states) + "_states")


def createCTMC(
    transitions: list,
    labelling: list,
    initial_state,
    name: str = "unknown_CTMC",
    synchronous_transitions: list = [],
) -> CTMC:
    """
    An user-friendly way to create a CTMC.

    Parameters
    ----------
    transitions : [ list of tuples (int, int, float)]
            Each tuple represents a transition as follow:
            (source state ID, destination state ID, rate).
    labelling: list of str
            A list of N observations (with N the nb of states).
            If `labelling[s] == o` then state of ID `s` is labelled by `o`.
            Each state has exactly one label.
    initial_state : int or list of float
            Determine which state is the initial one (then it's the id of the
            state), or what are the probability to start in each state (then it's
            a list of probabilities).
    name : str, optional
            Name of the model.
            Default is "unknow_CTMC"
    synchronous_transitions: list, optional.
            This is useful only for synchronously composing this CTMC with
            another one.
            List of (source_state <int>, action <str>, dest_state <int>, rate <float>).
            Default is an empty list.

    Returns
    -------
    CTMC
            the CTMC describes by `transitions`, `labelling`, and `initial_state`.

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
    if "init" in labelling:
        msg = "The label 'init' cannot be used: it is reserved for initial states."
        raise SyntaxError(msg)

    states = [i[0] for i in transitions] + [i[1] for i in transitions]
    states += [i[0] for i in synchronous_transitions] + [
        i[2] for i in synchronous_transitions
    ]
    states = list(set(states))
    states.sort()
    nb_states = len(states)

    if nb_states > len(labelling):
        raise ValueError(
            "All states are not labelled (the labelling list is too small)."
        )
    elif nb_states < len(labelling):
        print("WARNING: the labelling list is bigger than the number of states")

    res = zeros((nb_states, nb_states))
    for t in transitions:
        res[states.index(t[0])][states.index(t[1])] = t[2]

    labelling.append("init")
    res = vstack((res, zeros(len(res))))
    res = hstack((res, zeros(len(res))[:, newaxis]))
    if type(initial_state) == int:
        res[-1][initial_state] = 1.0
    else:
        if type(initial_state) == ndarray:
            initial_state = initial_state.tolist()
        res[-1] = array(initial_state + [0.0])
    return CTMC(res, labelling, name, synchronous_transitions)


def synchronousCompositionCTMCs(
    m1: CTMC, m2: CTMC, name: str = "unknown_composition"
) -> PCTMC:
    """
    Returns the synchornous compotision of `m1` and `m2`.
    The output model is a PCTMC since some parameters are shared
    by multiple transitions.

    Parameters
    ----------
    m1 : CTMC
            First CTMC to compose with.
    m2 : CTMC
            Second CTMC to compose with.

    Returns
    -------
    PCTMC
            Synchronous composition of `m1` and `m2`.
    """
    m1_init = [i for i, li in enumerate(m1.labelling) if li == "init"]
    m2_init = [i for i, li in enumerate(m2.labelling) if li == "init"]
    m1_nb_states = m1.nb_states - len(m1_init)
    m2_nb_states = m2.nb_states - len(m2_init)
    nb_states = m1_nb_states * m2_nb_states
    m1_sids = [
        i - m1.labelling[:i].count("init")
        for i, li in enumerate(m1.labelling)
        if li != "init"
    ]
    m2_sids = [
        i - m1.labelling[:i].count("init")
        for i, li in enumerate(m2.labelling)
        if li != "init"
    ]

    matrix = zeros((nb_states, nb_states), dtype="uint8")
    labelling = []
    p_v = {}
    p_str = []
    p_i = []
    t_e = [sympify("0.0")]

    def get_state_index(s1, s2):
        s2 = m2_sids.index(s2)
        s1 = m1_sids.index(s1)
        return s1 * m2_nb_states + s2

    def add_in_matrix(trans, s1, s2, model, add_index=[]):
        if model == 1:
            for i in m2_sids:
                x, y = get_state_index(s1, i), get_state_index(s2, i)
                matrix[x, y] = trans
                for ind in add_index:
                    p_i[ind].append([x, y])
        elif model == 2:
            for i in m1_sids:
                x, y = get_state_index(i, s1), get_state_index(i, s2)
                matrix[x, y] = trans
                for ind in add_index:
                    p_i[ind].append([x, y])

    for s1 in m1_sids:  # labelling
        for s2 in m2_sids:
            labelling.append(m1.labelling[s1] + "," + m2.labelling[s2])

    for i in m1_sids:  # m1 transitions
        for j in m1_sids:
            if m1.matrix[i, j] != 0.0:
                p_str.append("p_" + str(i) + "_" + str(j))
                p_v["p_" + str(i) + "_" + str(j)] = m1.matrix[i, j]
                p_i.append([])
                t_e.append(sympify(p_str[-1]))
                add_in_matrix(
                    len(t_e) - 1, s1=i, s2=j, model=1, add_index=[len(p_i) - 1]
                )
    for i in m2_sids:  # m2 transitions
        for j in m2_sids:
            if m2.matrix[i, j] != 0.0:
                p_str.append("q_" + str(i) + "_" + str(j))
                p_v["q_" + str(i) + "_" + str(j)] = m2.matrix[i, j]
                p_i.append([])
                t_e.append(sympify(p_str[-1]))
                add_in_matrix(
                    len(t_e) - 1, s1=i, s2=j, model=2, add_index=[len(p_i) - 1]
                )

    for i in m1_sids:  # self loops
        if m1.matrix[i, i] != 0.0:
            for j in m2_sids:
                if m2.matrix[j, j] != 0.0:
                    matrix[get_state_index(i, j), get_state_index(i, j)] = len(t_e)
                    t_e.append(
                        sympify(
                            "p" + str(i) + "_" + str(i) + "+q" + str(j) + "_" + str(j)
                        )
                    )

    for si, ai, di, pi in m1.synchronous_transitions:  # synchronous transitions
        for sj, aj, dj, pj in m2.synchronous_transitions:
            if ai == aj:
                t_e.append(
                    t_e[matrix[get_state_index(si, sj), get_state_index(di, dj)]]
                    + sympify(pi * pj)
                )
                matrix[get_state_index(si, sj), get_state_index(di, dj)] = len(t_e) - 1

    labelling.append("init")
    matrix = vstack((matrix, zeros(nb_states, dtype="uint8")))
    matrix = hstack((matrix, zeros(nb_states + 1, dtype="uint8")[:, newaxis]))
    m1_init_trans = zeros(m1_nb_states)
    for i in m1_init:
        tmp = [m1.matrix[i][j] for j in m1_sids]
        for j in range(m1_nb_states):
            if m1_init_trans[j] * tmp[j] > 0.0:
                m1_init_trans[j] *= tmp[j]
            else:
                m1_init_trans[j] += tmp[j]
    m2_init_trans = zeros(m2_nb_states)
    for i in m2_init:
        tmp = [m2.matrix[i][j] for j in m2_sids]
        for j in range(m2_nb_states):
            if m2_init_trans[j] * tmp[j] > 0.0:
                m2_init_trans[j] *= tmp[j]
            else:
                m2_init_trans[j] += tmp[j]
    for i, si in enumerate(m1_init_trans):
        if si > 0.0:
            p_str.append("initp_" + str(i))
            p_v["initp_" + str(i)] = si
            p_i.append([])
    for i, si in enumerate(m2_init_trans):
        if si > 0.0:
            p_str.append("initq_" + str(i))
            p_v["initq_" + str(i)] = si
            p_i.append([])
    for i, si in enumerate(m1_init_trans):
        for j, sj in enumerate(m2_init_trans):
            if si * sj <= 0.0:
                matrix[-1][get_state_index(i, j)] = 0
            else:
                matrix[-1][get_state_index(i, j)] = len(t_e)
                t_e.append(sympify("initp_" + str(i) + "*initq_" + str(j)))
                p_i[p_str.index("initp_" + str(i))].append(
                    [len(matrix) - 1, get_state_index(i, j)]
                )
                p_i[p_str.index("initq_" + str(j))].append(
                    [len(matrix) - 1, get_state_index(i, j)]
                )

    i = 0
    while i < len(matrix):  # removing unreachable states
        if (matrix.T[i] == 0).all() == True and labelling[i] != "init":
            nb_states -= 1
            matrix = delete(matrix, i, 0)
            matrix = delete(matrix, i, 1)
            labelling = labelling[:i] + labelling[i + 1 :]
            for j in range(len(p_i)):
                k = 0
                while k < len(p_i[j]):
                    if p_i[j][k][0] == i or p_i[j][k][1] == i:
                        p_i[j].remove(p_i[j][k])
                    else:
                        if p_i[j][k][0] > i:
                            p_i[j][k][0] -= 1
                        if p_i[j][k][1] > i:
                            p_i[j][k][1] -= 1
                        k += 1
            i = -1
        i += 1

    return PCTMC(matrix, labelling, t_e, p_v, p_i, p_str, name)
