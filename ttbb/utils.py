import sys, os
import math
from tqdm import tqdm

def getVarlist():
    #list backup
    column = [ 'dR', 'M',
#		'pt1', 'eta1', 'phi1', 'e1',
#		'pt2', 'eta2', 'phi2', 'e2'
    ]
    return column

#def getVarlist():
#    #list backup
#    column = [ 'dR', 'M', 'njets', 'nbjets',
##        'addbjet1_pt', 'addbjet1_eta', 'addbjet1_phi', 'addbjet1_e',
##		'addbjet2_pt', 'addbjet2_eta', 'addbjet2_phi', 'addbjet2_e',
#		'pt1', 'eta1', 'phi1', 'e1',
#		'pt2', 'eta2', 'phi2', 'e2'
#    ]
#    return column

def getHistXlabel():
    #list backup
	dictionary = {
		'addbjet1_pt':'a', 'addbjet1_eta':'b', 'addbjet1_phi':'c', 'addbjet1_e':'d',
		'addbjet2_pt':'e', 'addbjet2_eta':'f', 'addbjet2_phi':'g', 'addbjet2_e':'h',
		'pt1':'i', 'eta1':'j', 'phi1':'k', 'e1':'l',
		'pt2':'m', 'eta2':'n', 'phi2':'o', 'e2':'p',
		'dR':'q', 'M':'r', 'njets':'s','nbjets':'t'
	}
	return dictionary



def getHistRange(var):

    histRange = []

    if 'dR' in var:
        histRange = [20, 0, 4]
    elif 'dEta' in var:
        histRange = [20, 0, 2.5]
    elif 'dPhi' in var:
        histRange = [20, 0, math.pi]
    elif 'Pt' in var:
        histRange = [20, 0, 400]
    elif ('Eta' in var) and (not 'dEta' in var):
        histRange = [20, -2.5, 2.5]
    elif ('Phi' in var) and (not 'dPhi' in var):
        histRange = [20, -math.pi, math.pi]
    elif ('mT' in var) or ('M' in var):
        histRange = [20, 0, 400]
    elif ('E' in var) and (not 'Et' in var) or ('e' in var):
        histRange = [20, 0, 400]
    elif 'csv' in var:
        histRange = [20, 0, 1]
    else:
        histRange = [20, 0, 10]
        print("Variable does not exist")

    return histRange

def printProgress(iteration, total, prefix = '', suffix = '', decimals = 1, barLength = 100):
    nEvent = str(iteration) + '/' + str(total)
    formatStr = "{0:." + str(decimals) + "f}"
    percent = formatStr.format(100*(iteration/float(total)))
    filledLength = int(round(barLength * iteration/float(total)))
    bar = '#'*filledLength + '-'*(barLength-filledLength)
    sys.stdout.write('\r%s |%s| %s%s %s %s' % (prefix, bar, percent, '%', suffix, nEvent)),
    if iteration == total:
        sys.stdout.write('\n')
    sys.stdout.flush()

