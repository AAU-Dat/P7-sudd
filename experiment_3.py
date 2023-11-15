# -----------------------------------------------------------------------------
# JAJAPY SCALABILITY EVALUATION (PCTMCs)
# -----------------------------------------------------------------------------
# Experiment over the size of the model:
#
# We use Jajapy to estimate the parameters "mu1a","mu1b","mu2" and "kappa" of
# tandem queueing networks with an increase value of c.
# (see http://www.prismmodelchecker.org/casestudies/tandem.php)
#
# The training set always contains 100 sequences of 30 labels.
# For each value of c, we use ask Jajapy to estimate the parameters from
# a timed training set consiting of 100 sequences of 30 pairs dwell time-label
# and a non timed training set of 100 sequences of 30 labels.
# 
# To increase the quality of this experiement, we repeat each Jajapy execution
# several times.
#
# Execting this script will clear the terminal several time (don't worry).
# 
# Parameters:
# -----------
# max_c : maximal value for c.
# steps : at each iterations, we increase the value of c by `steps`.
# nb_rep: for each value of c, we execute Jajapy on `nb_rep` timed training
#		  sets and `nb_rep` non-timed training sets.
#
# Arguments:
# ----------
# --fastest:  max_c=19 , steps=7, nb_rep=3
# --quick:    max_c=25 , steps=2, nb_rep=5
# --full:     max_c=25 , steps=1, nb_rep=10
# default:    max_c=25 , steps=1, nb_rep=10 (full)
#
# Expected running time (for a VM with 4 CPUs at 4GHz):
# ---------------------------------------------------------
# fastest: 
#  quick : > 1 day
#   full : > 1 day
# -----------------------------------------------------------------------------
import sys
from jajapy import loadSet, loadPrism, loadPCTMC, BW
from numpy import zeros, array, mean, std, newaxis
from datetime import datetime
import matplotlib.pyplot as plt

min_c = 3

def experiment3(args):
	max_c = 19
	steps = 7
	nb_rep = 3
	if len(args)>1:
		if args[1] == '--quick':
			steps = 2
			nb_rep = 5
		elif args[1] == '--fastest':
			steps = 7
			nb_rep = 3
			max_c = 19
	execute(max_c,steps,nb_rep,args[1][2:])
	execute(max_c,steps,nb_rep,args[1][2:])

def execute(max_c,steps,nb_rep,arg):
	models_info,learnings_info_timed,learnings_info_nontimed = 	learn(max_c,steps,nb_rep)
	tabular(nb_rep,models_info,learnings_info_timed,learnings_info_nontimed)
	plot(models_info,learnings_info_timed,learnings_info_nontimed,arg)

def tabular(nb_rep,models_info,learnings_info_timed,learnings_info_nontimed):
	real_values = array([0.2,1.8,2.0,4.0])
	inter_line = "+----+-------+------+---------+-----------+-------+---------+-------+---------+"	
	print("+----+-------+------+---------------------+-----------------+-----------------+")
	print("|  c |  |S|  | |->| |   Running time (s)  |    ||delta||1   |   ||delta||inf  |")
	print("|    |       |      |   timed |  n-timed  | timed | n-timed | timed | n-timed |")
	print(inter_line)

	len_col = [len(i) for i in inter_line.split('+')]

	for i,mi in enumerate(models_info):
		c,nb_s,nn_z = mi
		print('| '+f'{str(c): >2}'+' | '+f'{str(nb_s): >5}'+' | '+f'{str(nn_z): >4}',end=' ')
		print('| '+f'{str(round(mean(learnings_info_timed[i][0]),3)): >7}',end=' ')
		print('| '+f'{str(round(mean(learnings_info_nontimed[i][0]),3)): >9}',end=' ')

		temp_timed = abs(learnings_info_timed[i][1:]-real_values[:,newaxis])
		print('| '+f'{str(round(temp_timed.sum()/nb_rep,3)): >5}',end=' ')
		temp_nontimed = abs(learnings_info_nontimed[i][1:]-real_values[:,newaxis])
		print('| '+f'{str(round(temp_nontimed.sum()/nb_rep,3)): >7}',end=' ')

		temp_timed = temp_timed.max(axis=0)
		print('| '+f'{str(round(temp_timed.sum()/nb_rep,3)): >5}',end=' ')
		temp_nontimed = temp_nontimed.max(axis=0)
		print('| '+f'{str(round(temp_nontimed.sum()/nb_rep,3)): >7}',end=' |\n')

		print(inter_line)

def createTandemPrismModel(c):
	# not used in the artefact
	f1 = open('tandem/tandem_'+str(c-1)+'.sm','r')
	f2 = open('tandem/tandem_'+str(c)  +'.sm','w')
	
	l = f1.readline()
	while l != 'const int c='+str(c-1)+'; // queue capacity\n':
		f2.write(l)
		l = f1.readline()
	f2.write('const int c='+str(c)+'; // queue capacity\n')
	
	l = f1.readline()
	while l != 'label "sc_'+str(c-1)+'" = sc = '+str(c-1)+';\n':
		f2.write(l)
		l = f1.readline()
	f2.write(l)
	f2.write('label "sc_'+str(c)+'" = sc = '+str(c)+';\n')
	f2.close()
	f1.close()

def generateSet(c,timed=True):
	m = loadPrism("examples/materials/tandem_3.sm")
	m.instantiate(["mu1a","mu1b","mu2","kappa"],[0.2,1.8,2.0,4.0])
	return m.generateSet(100,30,timed=timed)

def learn(max_c,steps,nb_rep):
	models_info = [] #contains the infos about the model: (c,nb_states,nb_non_zero_trans)

	for k in range(min_c,max_c,steps):
		m = loadPrism("examples/materials/tandem_3.sm")
		nb_s = str(m.nb_states-1)
		nn_z = str((m.matrix.flatten() != 0).sum() - 1 )
		models_info.append((k,nb_s,nn_z))


	learnings_info_timed = zeros((1+((max_c-min_c)//steps),5,nb_rep))
	learnings_info_nontimed = zeros((1+((max_c-min_c)//steps),5,nb_rep))

	for timed in [True,False]:
		k = min_c
	
		while k < max_c:
			ts1 = generateSet(k,timed)

			for r in range(nb_rep):
				m = loadPrism("examples/materials/tandem_3.sm")
				start_time = datetime.now()
				output_val, learning_data = BW().fit_nonInstantiatedParameters(ts1,m,min_val=0.1, max_val=5.0, return_data=True, pp=str(timed)+" "+str(k)+" "+str(r+1))
				running_t = datetime.now()-start_time
				
				if timed:
					learnings_info_timed[(k-min_c)//steps][0][r] = running_t.total_seconds()
					learnings_info_timed[(k-min_c)//steps][1][r] = output_val['mu1a']
					learnings_info_timed[(k-min_c)//steps][2][r] = output_val['mu1b']
					learnings_info_timed[(k-min_c)//steps][3][r] = output_val['mu2']
					learnings_info_timed[(k-min_c)//steps][4][r] = output_val['kappa']
				else:
					learnings_info_nontimed[(k-min_c)//steps][0][r] = running_t.total_seconds()
					learnings_info_nontimed[(k-min_c)//steps][1][r] = output_val['mu1a']
					learnings_info_nontimed[(k-min_c)//steps][2][r] = output_val['mu1b']
					learnings_info_nontimed[(k-min_c)//steps][3][r] = output_val['mu2']
					learnings_info_nontimed[(k-min_c)//steps][4][r] = output_val['kappa']

			k += steps

	return models_info,learnings_info_timed,learnings_info_nontimed

def plot(models_info,learnings_info_timed,learnings_info_nontimed,arg):
	real_values = array([0.2,1.8,2.0,4.0])
	x = [i[1] for i in models_info]
	rt = [[],[]]
	rt_err = [[],[]]
	delta1 = [[],[]]
	delta1_err = [[],[]]
	deltainf = [[],[]]
	deltainf_err = [[],[]]
	for i in range(len(x)):
		rt[0].append(mean(learnings_info_timed[i][0]))
		rt[1].append(mean(learnings_info_nontimed[i][0]))
		rt_err[0].append(std(learnings_info_timed[i][0]))
		rt_err[1].append(std(learnings_info_nontimed[i][0]))

		temp_timed = abs(learnings_info_timed[i][1:]-real_values[:,newaxis])
		temp_nontimed = abs(learnings_info_nontimed[i][1:]-real_values[:,newaxis])
		delta1[0].append(mean(temp_timed.sum(axis=1)))
		delta1[1].append(mean(temp_nontimed.sum(axis=1)))
		delta1_err[0].append(std(temp_timed.sum(axis=1)))
		delta1_err[1].append(std(temp_nontimed.sum(axis=1)))

		temp_timed = temp_timed.max(axis=0)
		temp_nontimed = temp_nontimed.max(axis=0)
		deltainf[0].append(mean(temp_timed))
		deltainf[1].append(mean(temp_nontimed))
		deltainf_err[0].append(std(temp_timed))
		deltainf_err[1].append(std(temp_nontimed))

	fig, axs = plt.subplots(2,2,figsize=(11,11))
	colors = ['r','b']
	labels = ['timed','non-timed']
	ax2 = plt.subplot(212)
	ax0 = plt.subplot(221)
	ax1 = plt.subplot(222)
	for k in range(2):
		ax0.errorbar(x,delta1[k], yerr=delta1_err[k], fmt='o',linewidth=2,capsize=4,c=colors[k], label=labels[k], alpha=0.5)
		ax1.errorbar(x,deltainf[k], yerr=deltainf_err[k], fmt='o',linewidth=2,capsize=4,c=colors[k], label=labels[k], alpha=0.5)
		ax2.errorbar(x,rt[k],yerr=rt_err[k], fmt='-o',linewidth=2,capsize=4,c=colors[k], label=labels[k], alpha=0.5)

	ax0.set_ylabel(r'$L_1$ norm')
	ax0.set_xlabel("Number of states")

	ax1.set_ylabel(r'$L_\infty$ norm')
	ax1.set_xlabel("Number of states")

	ax2.set_ylabel("Running time (s)")
	ax2.set_xlabel("Number of states")

	ax0.legend()
	ax1.legend()
	ax2.legend()
	plt.rcParams['figure.figsize'] = [3, 2]
	#plt.show()
	plt.savefig('tandem_experiment'+arg+'.png')


if __name__ == '__main__':
	experiment3(sys.argv)