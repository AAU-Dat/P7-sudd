import unittest
from ..pctmc import *
from os import remove
from ..base.Set import *
from ..base.BW import BW, ComputeAlphaBetaHow
from ..with_stormpy.model_converter import loadPrism


def modelPCTMC_ped():
    labelling = ["red", "green"]
    transitions = [(1, 0, 5.0)]
    sync_trans = [(0, "b", 1, 0.5), (0, "r", 0, 10.0), (0, "g", 0, 10.0)]
    parameters = ["r", "g"]
    return createPCTMC(transitions, labelling, parameters, 0, {}, sync_trans)


def modelPCTMC_car2():
    labelling = ["red", "green"]
    transitions = []
    sync_trans = [
        (0, "g", 1, "g"),
        (1, "r", 0, "r"),
        (0, "b", 0, 10.0),
        (1, "b", 0, 2.0),
    ]
    parameters = ["r", "g"]
    return createPCTMC(transitions, labelling, parameters, 1, {}, sync_trans)


def modelPCTMC_car1():
    labelling = ["red", "green"]
    transitions = []
    sync_trans = [
        (0, "r", 1, "r"),
        (1, "g", 0, "g"),
        (0, "b", 0, 10.0),
        (1, "b", 0, 2.0),
    ]
    parameters = ["r", "g"]
    return createPCTMC(transitions, labelling, parameters, 0, {}, sync_trans)


def modelPCTMCCar_tl():
    labelling = ["c_red", "c_orange", "c_green"]
    transitions = [(1, 0, 2.0)]
    sync_trans = [(0, "nothing", 2, 0.5), (2, "button", 1, "p")]
    parameters = ["p"]
    return createPCTMC(transitions, labelling, parameters, 0, {}, sync_trans)


def modelPCTMCPed_tl():
    labelling = ["p_red", "p_green"]
    transitions = [(1, 0, 0.1)]
    sync_trans = [(0, "nothing", 0, 10.0), (0, "button", 1, "p/2")]
    parameters = ["p"]
    return createPCTMC(transitions, labelling, parameters, 0, {}, sync_trans)


m = loadPrism("jajapy/tests/materials/pctmc/tl.pm")


class PCTMCTestclass(unittest.TestCase):
    LOW_INITIAL_PARAMETER = 0.000025
    MID_INITIAL_PARAMETER = 0.5
    PRECISION = 7

    def test_PCTMC_instantiation(var):
        var.assertFalse(m.isInstantiated())
        for i in range(6):
            if i != 1:
                var.assertTrue(m.isInstantiated(i))
            else:
                var.assertFalse(m.isInstantiated(i))
        var.assertEqual(["p"], m.involvedParameters(1))
        var.assertEqual(["p"], m.involvedParameters(1, 2))
        m.instantiate(["p"], [1.0])
        var.assertTrue(m.isInstantiated())
        var.assertTrue(m.isInstantiated(1))
        var.assertEqual(m.transitionValue(1, 2), 1 / 2)
        var.assertEqual(m.parameterValue("p"), 1.0)

    def test_PCTMC_state(var):
        var.assertEqual(m.e(0), 5.0)
        var.assertEqual(m.e(2), 2.1)
        var.assertEqual(m.expected_time(0), 0.2)
        var.assertEqual(m.l(0, 2, "red"), 0.0)
        var.assertEqual(m.l(0, 1, "c_red_p_red"), 5.0)
        var.assertEqual(m.tau(0, 1, "c_red_p_red"), 1.0)
        var.assertEqual(m.tau(0, 2, "yellow"), 0.0)
        var.assertEqual(m.tau(2, 3, "c_orange_p_green"), 2.0 / 2.1)
        var.assertEqual(m.tau(2, 4, "c_orange_p_green"), 0.1 / 2.1)
        t = 1.0
        lkl = 2.1 * exp(-2.1 * t)
        var.assertEqual(m.lkl(2, t), lkl)
        var.assertEqual(m.lkl(0, -t), 0.0)

    def test_PCTMC_getLabel_getAlphabet(var):
        var.assertEqual(
            set(m.getAlphabet()),
            set(
                [
                    "c_red_p_red",
                    "c_green_p_red",
                    "c_orange_p_green",
                    "c_red_p_green",
                    "c_orange_p_red",
                    "init",
                ]
            ),
        )
        var.assertEqual(m.getLabel(2), "c_orange_p_green")

    def test_PCTMC_save_load_str(var):
        m.save("test_save.txt")
        mprime = loadPCTMC("test_save.txt")
        var.assertEqual(str(m), str(mprime))
        remove("test_save.txt")

    def test_PCTMC_Set(var):
        m.randomInstantiation()
        set1 = m.generateSet(50, 10, timed=True)
        var.assertEqual(set1.type, 4)
        set2 = m.generateSet(50, 1 / 4, "geo", 6, timed=True)
        set1.addSet(set2)
        var.assertEqual(set1.type, 4)
        set1.save("test_save.txt")
        set2 = loadSet("test_save.txt")
        var.assertEqual(set2.type, 4)
        var.assertTrue(set1.isEqual(set2))
        remove("test_save.txt")

    def test_PCTMC_logLikelihood(var):
        # m = loadPrism('jajapy/tests/materials/pctmc/tl.pm')
        m.instantiate(["p"], [1.0])
        set1 = Set(
            [
                [
                    "init",
                    "c_red_p_red",
                    "c_green_p_red",
                    "c_orange_p_green",
                    "c_red_p_green",
                    "c_red_p_red",
                ]
            ],
            [1],
        )
        l11 = m._logLikelihood_oneproc(set1)
        l12 = m._logLikelihood_multiproc(set1)
        var.assertAlmostEqual(l11, l12)
        var.assertAlmostEqual(l11, -0.048790164169432056)
        set2 = Set(
            [
                [
                    "init",
                    0.51245612,
                    "c_red_p_red",
                    0.20149229043430208,
                    "c_green_p_red",
                    0.8691033110137091,
                    "c_orange_p_green",
                    0.40391041817651124,
                    "c_red_p_green",
                    17.822666563218853,
                    "c_red_p_red",
                ],
                [
                    "init",
                    0.497854,
                    "c_red_p_red",
                    0.237901319423925,
                    "c_green_p_red",
                    6.906613946631418,
                    "c_orange_p_green",
                    0.6742555535289178,
                    "c_red_p_green",
                    5.788386627774087,
                    "c_red_p_red",
                ],
            ],
            [1, 1],
        )
        l21 = m.logLikelihood(set2)
        var.assertAlmostEqual(l21, -6.553342509457142)

    def test_PCTMC_parameters_learning_untimed(var):
        initial_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        initial_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.MID_INITIAL_PARAMETER, PCTMCTestclass.MID_INITIAL_PARAMETER, PCTMCTestclass.MID_INITIAL_PARAMETER, PCTMCTestclass.MID_INITIAL_PARAMETER])
        training_set = loadSet("jajapy/tests/materials/pctmc/untimed_training_set_PCTMC.txt")
        bw = BW()
        # bw.processes = 1  # uncomment this when debugging
        output_gotten = bw.fit_parameters(
            training_set, initial_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.LOG_SEMIRING
        )
        output_expected = {
            "mu1a": 0.19154272606453604,
            "mu1b": 1.7589871248425895,
            "mu2": 2.287450015307295,
            "kappa": 4.0504485338277245,
        }
        for key, _ in output_gotten.items():
            var.assertAlmostEqual(output_expected[key], output_gotten[key], PCTMCTestclass.PRECISION)

    def test_PCTMC_parameters_learning_timed(var):
        initial_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        initial_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.MID_INITIAL_PARAMETER, PCTMCTestclass.MID_INITIAL_PARAMETER, PCTMCTestclass.MID_INITIAL_PARAMETER, PCTMCTestclass.MID_INITIAL_PARAMETER])
        training_set = loadSet("jajapy/tests/materials/pctmc/timed_training_set_PCTMC.txt")
        bw = BW()
        # bw.processes = 1  # uncomment this when debugging
        output_gotten = bw.fit_parameters(
            training_set, initial_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.LOG_SEMIRING
        )
        output_expected = {
            "mu1a": 0.1867468928590901,
            "mu1b": 1.771859556215226,
            "mu2": 2.1383724969908315,
            "kappa": 4.051248447744933,
        }
        for key, _ in output_gotten.items():
            var.assertAlmostEqual(output_expected[key], output_gotten[key], PCTMCTestclass.PRECISION)

    def test_PCTMC_parameters_learning_untimed_all_methods_equal(var):
        # Arrange
        training_set = loadSet("jajapy/tests/materials/pctmc/untimed_training_set_PCTMC.txt")

        classic_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        classic_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        classic_bw = BW()

        element_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        element_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        element_bw = BW()

        matrix_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        matrix_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        matrix_bw = BW()

        add_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        add_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        add_bw = BW()

        log_semiring_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        log_semiring_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        log_semiring_bw = BW()

        # Act
        log_semiring_output = log_semiring_bw.fit_parameters(training_set, log_semiring_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.LOG_SEMIRING)

        classic_output = classic_bw.fit_parameters(training_set, classic_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.CLASSIC)

        element_output = element_bw.fit_parameters(training_set, element_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.ELEMENT)

        matrix_output = matrix_bw.fit_parameters(training_set, matrix_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.MATRIX)

        add_output = add_bw.fit_parameters(training_set, add_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.ADD)

        # Assert
        for key, _ in matrix_output.items():
            var.assertAlmostEqual(log_semiring_output[key], classic_output[key], PCTMCTestclass.PRECISION)
            var.assertAlmostEqual(log_semiring_output[key], element_output[key], PCTMCTestclass.PRECISION)
            var.assertAlmostEqual(log_semiring_output[key], matrix_output[key], PCTMCTestclass.PRECISION)
            var.assertAlmostEqual(log_semiring_output[key], add_output[key], PCTMCTestclass.PRECISION)

    def test_PCTMC_parameters_learningntimed_all_methods_equal(var):
        # Arrange
        training_set = loadSet("jajapy/tests/materials/pctmc/timed_training_set_PCTMC.txt")

        classic_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        classic_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        classic_bw = BW()

        element_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        element_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        element_bw = BW()

        matrix_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        matrix_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        matrix_bw = BW()

        add_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        add_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        add_bw = BW()

        log_semiring_model = loadPrism("jajapy/tests/materials/pctmc/tandem_3.sm")
        log_semiring_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER, PCTMCTestclass.LOW_INITIAL_PARAMETER])
        log_semiring_bw = BW()

        # Act
        log_semiring_output = log_semiring_bw.fit_parameters(training_set, log_semiring_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.LOG_SEMIRING)

        classic_output = classic_bw.fit_parameters(training_set, classic_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.CLASSIC)

        element_output = element_bw.fit_parameters(training_set, element_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.ELEMENT)

        matrix_output = matrix_bw.fit_parameters(training_set, matrix_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.MATRIX)

        add_output = add_bw.fit_parameters(training_set, add_model, ["mu1a", "mu1b", "mu2", "kappa"], compute_alpha_beta_how=ComputeAlphaBetaHow.ADD)

        # Assert
        for key, _ in matrix_output.items():
            var.assertAlmostEqual(log_semiring_output[key], classic_output[key], PCTMCTestclass.PRECISION)
            var.assertAlmostEqual(log_semiring_output[key], element_output[key], PCTMCTestclass.PRECISION)
            var.assertAlmostEqual(log_semiring_output[key], matrix_output[key], PCTMCTestclass.PRECISION)
            var.assertAlmostEqual(log_semiring_output[key], add_output[key], PCTMCTestclass.PRECISION)


if __name__ == "__main__":
    unittest.main()
