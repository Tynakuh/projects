from pybrain.structure import LinearLayer, SigmoidLayer, TanhLayer, BiasUnit
from pybrain.structure import FeedForwardNetwork
from pybrain.structure import FullConnection
from pybrain.tools.customxml.networkwriter import NetworkWriter
from pybrain.tools.customxml.networkreader import NetworkReader
import numpy as np
import random
import time
import threading
import thread
from pybrain.utilities import threaded
from simulation import Simulation

MUTATE_CHANCE = 1.00
GEN = 0
POP_SIZE = 100
STEPS = 80
LAST_REWARD = 0.0
sim = None
totalRewards = np.array([])

def createRandomNetwork():
    n = FeedForwardNetwork()
    inLayer = LinearLayer(3)
    hiddenLayer = TanhLayer(6)
    outLayer = TanhLayer(2)
    bias = BiasUnit('bias')
    n.addInputModule(inLayer)
    n.addModule(hiddenLayer)
    n.addOutputModule(outLayer)
    n.addModule(bias)
    in_to_hidden = FullConnection(inLayer,hiddenLayer)
    hidden_to_out = FullConnection(hiddenLayer,outLayer)
    bias_to_hidden = FullConnection(bias, hiddenLayer)
    bias_to_out = FullConnection(bias, outLayer)
    n.addConnection(in_to_hidden)
    n.addConnection(hidden_to_out)
    n.addConnection(bias_to_hidden)
    n.addConnection(bias_to_out) 
    n.sortModules()
    rand_gen_seq = np.random.uniform(-1,1,60)
    n._setParameters(rand_gen_seq)
    return n

def generateRandomNetworks(size):
    netList = np.ndarray(shape=(size,2),dtype = object) #netList[n][0] = Network, netList[n][1] = Reward 
    count = 0
    while (count < (netList.size/netList[0].size)):
        netList[count][0]  = createRandomNetwork()
        netList[count][1] = float(0)
        count = count + 1
    return netList


def crossover(parent):
    parents = parent.copy()
    cross_prob = 0.0+parents[0][1]/(0.0+parents[0][1]+parents[1][1])
    p1genome = parents[0][0].params
    p2genome = parents[1][0].params
    for x in range(0,p1genome.size): #for each gene in the chromosone crossover they have a % probability (based upon reward) to exchange genes to create a child
        if (random.random()<=cross_prob):
            temp = p1genome[x] 
            p1genome[x] = p2genome[x]
            p2genome[x] = temp
    parents[0][0]._setParameters(p1genome)
    parents[1][0]._setParameters(p2genome)
    return parents
    
def generateChildren(parent):
    networks = np.ndarray(shape=(POP_SIZE,2),dtype = 'object')
    for i in range(0,POP_SIZE): #Modify to take second child aswell if desired.
        networks[i][0] = crossover(parent)[0][0].copy()
        networks[i][1] = float(0)
    return networks

def mutate(netList): #Randomly mutate networks by mutate chance
    for count in range(10,POP_SIZE): #Start from 10, don't mutate elites.
        if (random.random() <= MUTATE_CHANCE):
            print count
            temp = netList[count][0].params
            mutated = random.randrange(0,temp.size-1)
            temp[mutated] = random.uniform(-1,1)
            netList[count][0]._setParameters(temp)
    return netList
    

def evaluate(network):
    global sim
    totalReward = 0.0
    for i in range(0,STEPS):
        totalReward = totalReward + step(network)
    sim.stop()
    print totalReward
    time.sleep(2)
    sim = None
    startSim()
    time.sleep(3)
    return totalReward

def step(network):
    global sim
    actions = network.activate(getSensors())
    print actions
    if (actions[0] > 0):
        sim.setLeft(True)
    else:
        sim.setLeft(False)
    if (actions[1] > 0):
        sim.setRight(True)
    else:
        sim.setRight(False)
    time.sleep(0.2)
    rewards = getRewards(getSensors())
    return rewards

def pickAndGen(netList):
    newList = np.ndarray(shape=(POP_SIZE,2),dtype = 'object')
    for count2 in  range(0,10,1): #Keep top 10, elitism.
        newList[count2][0] = netList[count2][0].copy()
        print newList[count2][0].params
        newList[count2][1] = 0.0
    print "NEXT"
    for count in  range(11,POP_SIZE,2):
        parents = np.ndarray(shape=(2,2),dtype = 'object')
        parent1 = pickParent(netList).copy()
        parent2 = pickParent(netList).copy()
        parents[0] = parent1
        parents[1] = parent2
        temp = crossover(parents).copy()
        parents[0][0] = temp[0][0]
        print parents[0][0]
        parents[0][1] = 0.0
        parents[1][0] = temp[1][0]
        parents[1][1] = 0.0
        newList[count-1] = parents[0]
        newList[count] = parents[1]
    return newList
                       
def pickParent(netList):
    rand = random.random()
    for count in range(0,POP_SIZE):
        value = netList[count][1]
        if(rand<value):
            return netList[count]
        else:
            rand = rand - value
    
def getRewards(array):
    return pow(2,(array[1]+array[0])/100)

def startLearn(netList):
    global totalRewards
    loopTotal = 0.0
    for count in range(0,POP_SIZE):
        reward = evaluate(netList[count][0]) #Evaluate each neural network for their reward
        netList[count][1] = reward
        loopTotal += reward
    netList = (netList[netList[:,1].argsort()])[::-1] #Sort by second column in descending order
    saveNetworks(netList)
    for count in range(0,POP_SIZE):
        netList[count][1] = netList[count][1]/loopTotal
    #totalRewards = np.append(totalRewards,netList[0][1])
    #saveNetworks(netList[:2].copy())
    netList = pickAndGen(netList)
    #netList = generateChildren(netList[:2]) #take top two
    print netList[0][0]
    netList = mutate(netList)
    return netList

def main():
    netList = generateRandomNetworks(POP_SIZE)
    startSim()
    time.sleep(2)
    while 1:
        netList = startLearn(netList)
        print 'LOOP'
    rb.stop()

def saveNetworks(netList):
    global GEN
    global MUTATE_CHANCE
    f = open('networkfinaltwo' +str(GEN)+ '.txt','w')
    for count in range(0,POP_SIZE):
        f.write(str(netList[count][0].params) + str(netList[count][1]))
    #f.write(str(netList[0][0].params)+'\n'+str(netList[1][0].params)+'\n' + str(totalRewards))
    f.close()
    f = open('bestnet.txt','w')
    f.write(str(netList[0][0].params))
    f.close()
    GEN= GEN+1
    MUTATE_CHANCE = MUTATE_CHANCE - 0.01 #annealing
    return

@threaded()
def startSim():
    global sim
    sim = Simulation()
    sim.start()

def getSensors():
    return np.array([sim.getX(),sim.getY(),sim.getAng()])
main()
