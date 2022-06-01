# jajapy

## Introduction
`jajapy` is a python library implementing the **Baum-Welch** algorithm on various kinds of Markov models.
**Warning** `jajapy` is still a WIP. 

## Main features
`jajapy` provides:
- BW algorithm for Hidden Markov Models [reference](https://web.ece.ucsb.edu/Faculty/Rabiner/ece259/Reprints/tutorial%20on%20hmm%20and%20applications.pdf)
- BW algorithm for Markov Chains
- BW algorithm for Gaussian Observation Hidden Markov Models [reference](http://www.inass.org/2020/2020022920.pdf)
- BW algorithm for Markov Decision Processes [reference](https://arxiv.org/abs/2110.03014)
- Active BW algorithm for Markov Decision Processes [reference](https://arxiv.org/abs/2110.03014)
- BW algorithm for CTMC
- BW algorithm for asynchronous parallel composition of CTMCs

Additionally, it provides other learning algorithms:
- Alergia, for Markov Chains [reference](https://www.researchgate.net/publication/2543721_Learning_Stochastic_Regular_Grammars_by_Means_of_a_State_Merging_Method/stats)
- IOAlergia, for Markov Decision Processes [reference](https://link.springer.com/content/pdf/10.1007/s10994-016-5565-9.pdf)

## Installation
Coming soon

## Requirements
- numpy
- scipy

## Get Started
Coming soon. For now check [demo.py](demo.py).

## TO DO
- unit tests
- generate the documentation. Add examples.
- upload it to Pypi
- link with sotrmpy, prism
- error management